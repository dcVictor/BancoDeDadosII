#ifndef METADATA_H
#define METADATA_H

#include <stdio.h>

typedef struct Metadata_table {
    int id;
    char logic_name[20];
    char physical_name[20];
} Metadata_table;

typedef struct Metadata_attributes {
    int id;
    char name[20];
    char type;
    char optional;
    int size;
} Metadata_attributes;

//usados para a criaçnao do buffer, utilizando atributos para encontrar o tamanho total.
int calculate_record_buffer_size(Metadata_attributes *attrs, int attr_count);
void* create_record_buffer(const char *filename, int record_size, Metadata_attributes *attrs, int attr_count);
//
void read_records(const char *filename, Metadata_attributes *attrs, int attr_count); //abre o arquivo físico e ler os itens presentes
Metadata_attributes* read_metadata(FILE *meta_attribute_fptr, Metadata_table buffer, int *attr_count); //prepara e encontra os metadados necessários
Metadata_table* find(char* file_name, FILE* meta_table_fptr); //verifica se existe uma tabela com o nome procurado
void read(char* file_name, Metadata_table buffer, FILE* meta_attribute_fptr); //lê os metadados e depois or "records"

#endif 
