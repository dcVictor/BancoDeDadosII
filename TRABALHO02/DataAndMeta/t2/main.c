#include "metadata.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Compile with: %s <file_name>\n", argv[0]);
        return 1;
    }

    FILE *meta_table_fptr;
    FILE *meta_attribute_fptr;
    char *file_name = argv[1];
    
    //abre os arquivos principais para uso
    meta_table_fptr = fopen("table.dic", "rb");
    if (!meta_table_fptr) {
        printf("Error opening tables metadata.");
        return 1;
    }

    meta_attribute_fptr = fopen("att.dic", "rb");
    if (!meta_attribute_fptr) {
        printf("Error opening attributes metadata.");
        fclose(meta_table_fptr);
        return 1;
    }
     
    //faz a lógica principal
    Metadata_table *found = find(file_name, meta_table_fptr);
    if (found == NULL) {
        printf("Table not found\n");
    } else {
        read(file_name, *found, meta_attribute_fptr);
        free(found);
    }

    fclose(meta_table_fptr);
    fclose(meta_attribute_fptr);

    return 0;
}
