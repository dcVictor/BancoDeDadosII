#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// UFFS - CIÊNCIA DA COMPUTAÇÃO - CAMPUS CHAPECÓ
// ESTUDANTE: VICTOR NEYMAR DE CONTO
// COMPONENTE CURRICULAR: BANCO DE DADOS II

typedef struct TableMetadata {
    int id;
    char logical_name[20];
    char physical_filename[20];
} TableMetadata;

typedef struct AttributeMetadata {
    int id;
    char name[20];
    char type;
    char optional;
    int size;
} AttributeMetadata;


int calculate_record_size(AttributeMetadata *attributes, int attribute_count) {
    int total_size = 0;
    // Soma o tamanho de cada atributo para obter o tamanho total da linha.
    for (int i = 0; i < attribute_count; i++) {
        total_size += attributes[i].size;
    }
    return total_size;
}

void print_records_from_file(const char *physical_filename, AttributeMetadata *attributes, int attribute_count) {
    FILE *data_file = fopen(physical_filename, "rb");
    if (!data_file) {
        printf("Erro ao abrir o arquivo: %s\n", physical_filename);
        return;
    }

    int record_size_bytes = calculate_record_size(attributes, attribute_count);
    if (record_size_bytes == 0) {
        printf("Tamanho do registro e zero.\n");
        fclose(data_file);
        return;
    }

    // Aloca um buffer único para ler cada registro do arquivo de uma só vez.
    void *record_buffer = malloc(record_size_bytes);
    if (!record_buffer) {
        printf("Falha ao alocar memoria\n");
        fclose(data_file);
        return;
    }

    // Lê o arquivo registro por registro até o final.
    while (fread(record_buffer, record_size_bytes, 1, data_file) == 1) {
        int current_offset = 0;
        for (int i = 0; i < attribute_count; i++) {
            AttributeMetadata current_attribute = attributes[i];

            // Pedaços do buffer são copiados para variáveis do tipo correto.
            switch (current_attribute.type) {
                case 'I': {
                    int value_int;
                    memcpy(&value_int, record_buffer + current_offset, sizeof(int));
                    printf("%d", value_int);
                    current_offset += sizeof(int);
                    break;
                }
                case 'D': {
                    double value_double;
                    memcpy(&value_double, record_buffer + current_offset, sizeof(double));
                    printf("%.4f", value_double);
                    current_offset += sizeof(double);
                    break;
                }
                case 'F': {
                    float value_float;
                    memcpy(&value_float, record_buffer + current_offset, sizeof(float));
                    printf("%.4f", value_float);
                    current_offset += sizeof(double); 
                    break;
                }
                case 'S': {
                
                    char *value_string = malloc(current_attribute.size + 1);
                    memcpy(value_string, record_buffer + current_offset, current_attribute.size);
                    value_string[current_attribute.size] = '\0'; // Garante que a string termina corretamente.
                    printf("%s", value_string);
                    free(value_string);
                    current_offset += current_attribute.size;
                    break;
                }
                default: {
                    printf("Tipo desconhecido");
                    break;
                }
            }
        
            if (i < attribute_count - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    free(record_buffer);
    fclose(data_file);
}


AttributeMetadata* get_attributes_for_table(FILE *attributes_file, TableMetadata target_table, int *out_attribute_count) {
    rewind(attributes_file);
    AttributeMetadata *attribute_list = NULL;
    AttributeMetadata current_attribute;

    while (fread(&current_attribute, sizeof(AttributeMetadata), 1, attributes_file) == 1) {
     
        if (current_attribute.id == target_table.id) {
           
            (*out_attribute_count)++;
            attribute_list = realloc(attribute_list, (*out_attribute_count) * sizeof(AttributeMetadata));
            if (!attribute_list) {
                printf("Falha na alocação de memoria para a lista de atributos.\n");
                return NULL;
            }
            attribute_list[(*out_attribute_count) - 1] = current_attribute;
        }
    }
    return attribute_list;
}


TableMetadata* find_table_by_name(char* table_logical_name, FILE* tables_file) {
    if (!table_logical_name || !tables_file) return NULL;

    TableMetadata current_table;
    rewind(tables_file);

    while (fread(&current_table, sizeof(TableMetadata), 1, tables_file) == 1) {
        if (strcmp(current_table.logical_name, table_logical_name) == 0) {
            // Aloca memoria para retornar uma copia da tabela encontrada.
            TableMetadata *found_table = malloc(sizeof(TableMetadata));
            if (!found_table) {
                printf("Falha na alocação de memoria ao encontrar a tabela.\n");
                return NULL;
            }
            *found_table = current_table;
            return found_table;
        }
    }
    return NULL; // Retorna NULL se a tabela não for encontrada.
}


void process_table_read(char* table_logical_name, TableMetadata target_table, FILE* attributes_file) {
    int attribute_count = 0;
    AttributeMetadata *attribute_list = get_attributes_for_table(attributes_file, target_table, &attribute_count);
    
    if (!attribute_list || attribute_count == 0) {
        printf("Nenhum atributp encontrado para a tabela '%s'.\n", table_logical_name);
        free(attribute_list);
        return;
    }

    // Imprime o cabeçalho com os nomes das colunas.
    for(int i = 0; i < attribute_count; i++){
        printf("%s%s", attribute_list[i].name, (i < attribute_count - 1) ? ", " : "");
    }
    printf("\n");

    // Imprime os registros de dados do arquivo físico.
    print_records_from_file(target_table.physical_filename, attribute_list, attribute_count);
    
    // Libera a memória alocada para a lista de atributos.
    free(attribute_list);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <nome_da_tabela>\n", argv[0]);
        return 1;
    }

    char *table_to_find = argv[1];
    
    // Tenta abrir os arquivos de dicionario de dados.
    FILE *tables_file = fopen("table.dic", "rb");
    if (!tables_file) {
        printf("Não foi possível abrir o dicionario de tabelas.\n");
        return 1;
    }

    FILE *attributes_file = fopen("att.dic", "rb");
    if (!attributes_file) {
        printf("Não foi possível abrir o dicionario de atributos'.\n");
        fclose(tables_file);
        return 1;
    }
     
    // encontrar a tabela e processar a leitura.
    TableMetadata *target_table_metadata = find_table_by_name(table_to_find, tables_file);
    
    if (target_table_metadata == NULL) {
        printf("Tabela '%s' não encontrada no dicionario.\n", table_to_find);
    } else {
        process_table_read(table_to_find, *target_table_metadata, attributes_file);
        free(target_table_metadata); // Libera a memória alocada por find_table_by_name.
    }

    // Fecha os arquivos abertos.
    fclose(tables_file);
    fclose(attributes_file);

    return 0;
}