#include "metadata.h"
#include <stdlib.h>
#include <string.h>

int calculate_record_buffer_size(Metadata_attributes *attrs, int attr_count) {
    int record_size = 0;
    for (int i = 0; i < attr_count; i++) {
        record_size += attrs[i].size;
    }
    return record_size;
}

void* create_record_buffer(const char *filename, int record_size, Metadata_attributes *attrs, int attr_count) {
    void *record = malloc(record_size);
    if (!record) {
        return NULL;
    }
    return record;
}

void read_records(const char *filename, Metadata_attributes *attrs, int attr_count) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error opening data file");
        return;
    }

    int record_size = calculate_record_buffer_size(attrs, attr_count);
    if (record_size == 0) {
        printf("Error.");
        return;
    }
    void *record = create_record_buffer(filename, record_size, attrs, attr_count);

    while (fread(record, record_size, 1, fp) == 1) {
        int offset = 0;
        for (int i = 0; i < attr_count; i++) {
            Metadata_attributes attribute = attrs[i];
            switch (attribute.type) {
                case 'I': {
                    int x;
                    memcpy(&x, record + offset, sizeof(int));
                    printf("%d", x);
                    offset += sizeof(int);
                    break;
                }
                case 'D': {
                    double x;
                    memcpy(&x, record + offset, sizeof(double));
                    printf("%.4f", x);
                    offset += sizeof(double);
                    break;
                }
                case 'F': { //arquivo passado (22.dat) possui campo de tamanho 8 bytes para uma float, causando confusão na leitura e precisando dessa gambiarra aqui.
                    float x;
                    memcpy(&x, record + offset, sizeof(float));
                    printf("%.4f", x);
                    offset += sizeof(double); 
                    break;
                }
                case 'S': {
                    char *x = malloc(attribute.size + 1);
                    memcpy(x, record + offset, attribute.size);
                    x[attribute.size] = '\0';
                    printf("%s", x);
                    free(x);
                    offset += attribute.size;
                    break;
                }
                default: {
                    printf("Error printing");
                }
            }
            if (i != attr_count - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
    fclose(fp);
}

Metadata_attributes* read_metadata(FILE *meta_attribute_fptr, Metadata_table buffer, int *attr_count) {
    rewind(meta_attribute_fptr);
    Metadata_attributes *metadata_list = NULL;
    Metadata_attributes buffer_attributes;

    while (fread(&buffer_attributes, sizeof(buffer_attributes), 1, meta_attribute_fptr) == 1) {
        if (buffer_attributes.id == buffer.id) {
            metadata_list = realloc(metadata_list, ((*attr_count) + 1) * sizeof(Metadata_attributes));
            if (!metadata_list) {
                printf("Memory allocation failed\n");
                return NULL;
            }
            printf("<%s, %c, %i, %i>\n", buffer_attributes.name, buffer_attributes.type, buffer_attributes.optional, buffer_attributes.size);
            metadata_list[*attr_count] = buffer_attributes;
            (*attr_count)++;
        }
    }
    return metadata_list;
}

Metadata_table* find(char* file_name, FILE* meta_table_fptr) {
    if (!file_name || !meta_table_fptr) return NULL;

    Metadata_table temp;
    Metadata_table *result = NULL;

    rewind(meta_table_fptr);
    while (fread(&temp, sizeof(temp), 1, meta_table_fptr) == 1) {
        if (strcmp(temp.logic_name, file_name) == 0) {
            result = malloc(sizeof(Metadata_table));
            if (!result) {
                printf("Memory allocation failed\n");
                return NULL;
            }
            *result = temp;
            break;
        }
    }
    return result;
}

void read(char* file_name, Metadata_table buffer, FILE* meta_attribute_fptr) {
    int count = 0;
    Metadata_attributes *metadata_list = read_metadata(meta_attribute_fptr, buffer, &count);
    if (!metadata_list || count == 0) {
        return;
    }

    for(int i = 0; i < count; i++){
        printf("%s ", metadata_list[i].name);
    }
    printf("%s\n", metadata_list[count].name);

    read_records(buffer.physical_name, metadata_list, count);
}
