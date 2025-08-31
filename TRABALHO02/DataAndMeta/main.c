#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ATTR 50
#define NAME_LEN 20

typedef struct {
    int id;
    char logical[NAME_LEN + 1];
    char physical[NAME_LEN + 1];
} Table;

typedef struct {
    int table_id;
    char name[NAME_LEN + 1];
    char type; // 'S' string, 'I' int, 'D' double
    int size;
} Attribute;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nome_tabela>\n", argv[0]);
        return 1;
    }

    char *tableName = argv[1];
    FILE *ftable = fopen("table.dic", "rb");
    if (!ftable) {
        perror("Erro ao abrir table.dic");
        return 1;
    }

    Table table;
    int found = 0;

    // Procurar tabela pelo nome lógico
    while (fread(&table.id, sizeof(int), 1, ftable) == 1) {
        fread(table.logical, sizeof(char), NAME_LEN, ftable);
        table.logical[NAME_LEN] = '\0';
        fread(table.physical, sizeof(char), NAME_LEN, ftable);
        table.physical[NAME_LEN] = '\0';

        if (strcmp(table.logical, tableName) == 0) {
            found = 1;
            break;
        }
    }
    fclose(ftable);

    if (!found) {
        printf("Tabela '%s' nao encontrada.\n", tableName);
        return 1;
    }

    // Ler atributos correspondentes à tabela
    FILE *fatt = fopen("att.dic", "rb");
    if (!fatt) {
        perror("Erro ao abrir att.dic");
        return 1;
    }

    Attribute attrs[MAX_ATTR];
    int numAttrs = 0;

    while (fread(&attrs[numAttrs].table_id, sizeof(int), 1, fatt) == 1) {
        fread(attrs[numAttrs].name, sizeof(char), NAME_LEN, fatt);
        attrs[numAttrs].name[NAME_LEN] = '\0';

        fread(&attrs[numAttrs].type, sizeof(char), 1, fatt);
        char optional;
        fread(&optional, sizeof(char), 1, fatt); // opcionalidade, ignorar
        fread(&attrs[numAttrs].size, sizeof(int), 1, fatt);

        if (attrs[numAttrs].table_id == table.id) {
            numAttrs++;
        }
        if (numAttrs >= MAX_ATTR) break;
    }
    fclose(fatt);

    // Abrir arquivo de dados físico
    FILE *fdata = fopen(table.physical, "rb");
    if (!fdata) {
        perror("Erro ao abrir arquivo de dados físico");
        return 1;
    }

    // Imprimir cabeçalho
    for (int i = 0; i < numAttrs; i++) {
        printf("%-20s", attrs[i].name);
    }
    printf("\n");

    // Ler registros
    while (!feof(fdata)) {
        int readAny = 0;
        for (int i = 0; i < numAttrs; i++) {
            readAny = 1;
            if (attrs[i].type == 'S') {
                char *buf = malloc(attrs[i].size + 1);
                if (!buf) { perror("malloc"); exit(1); }
                if (fread(buf, 1, attrs[i].size, fdata) != attrs[i].size) {
                    free(buf);
                    readAny = 0;
                    break;
                }
                buf[attrs[i].size] = '\0';
                printf("%-20s", buf);
                free(buf);
            } else if (attrs[i].type == 'I') {
                int val;
                if (fread(&val, sizeof(int), 1, fdata) != 1) { readAny = 0; break; }
                printf("%-20d", val);
            } else if (attrs[i].type == 'D') {
                double val;
                if (fread(&val, sizeof(double), 1, fdata) != 1) { readAny = 0; break; }
                printf("%-20.2f", val);
            }
        }
        if (!readAny) break;
        printf("\n");
    }

    fclose(fdata);
    return 0;
}
