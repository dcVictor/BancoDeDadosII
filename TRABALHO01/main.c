#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct t_attribute {
    char name[16];
    char type; 
    int size;
};
typedef struct t_attribute attribute;

void read_data_from_file(attribute *attributes, int number_attributes, FILE *file) {
    for (int i = 0; i < number_attributes; i++) {
        attribute attr = attributes[i];
        void *buffer = malloc(attr.size);

        fread(buffer, attr.size, 1, file);

        if (attr.type == 'i') {
            printf("%s: %d | ", attr.name, *(int*)buffer);
        } else if (attr.type == 'f') {
            printf("%s: %.2f | ", attr.name, *(float*)buffer);
        } else if (attr.type == 'c') {
            printf("%s: %.*s | ", attr.name, attr.size, (char*)buffer);
        }

        free(buffer);
    }
    printf("\n");
}



int main() {
    FILE *arquivo = fopen("customer.dat", "rb");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    attribute estrutura_arq;
    int n_atr;
    void *buffer = malloc(10);

    read_data_from_file(&estrutura_arq, arquivo, n_atr);
   
    fclose(arquivo);
    return 0;
}
