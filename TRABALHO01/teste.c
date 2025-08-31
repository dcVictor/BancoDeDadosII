#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ATTR 10

typedef struct {
    char name[16]; // 15 chars + \0
    char type;     // S, C, I, F
    int size;      // tamanho em bytes
} Attribute;

int main() {
    FILE *f = fopen("customer.dat", "rb");
    if (!f) {
        perror("Erro ao abrir customer.dat");
        return 1;
    }

    Attribute attrs[MAX_ATTR];
    int num_attrs = 0;

    // === Lê o cabeçalho ===
    for (int i = 0; i < MAX_ATTR; i++) {
        Attribute a;
        fread(a.name, 1, 15, f);
        a.name[15] = '\0';
        fread(&a.type, 1, 1, f);
        fread(&a.size, sizeof(int), 1, f);

        if (a.name[0] == '#') break; // fim dos atributos
        attrs[num_attrs++] = a;
    }

    // === Imprime cabeçalho da tabela ===
    for (int i = 0; i < num_attrs; i++) {
        printf("%-15s", attrs[i].name);
    }
    printf("\n");
    for (int i = 0; i < num_attrs; i++) {
        for (int j = 0; j < 15; j++) printf("-");
    }
    printf("\n");

    // === Lê e imprime os registros ===
    while (1) {
        int ok = 1;
        for (int i = 0; i < num_attrs; i++) {
            Attribute a = attrs[i];
            void *buffer = malloc(a.size);

            if (fread(buffer, a.size, 1, f) != 1) {
                free(buffer);
                ok = 0;
                break;
            }

            if (a.type == 'I') {
                printf("%-15d", *(int *)buffer);
            } else if (a.type == 'F') {
                printf("%-15.2f", *(float *)buffer);
            } else if (a.type == 'C') {
                printf("%-15c", *(char *)buffer);
            } else if (a.type == 'S') {
                char tmp[256];
                int len = (a.size < 255) ? a.size : 255;
                memcpy(tmp, buffer, len);
                tmp[len] = '\0';  // garante string terminada
                printf("%-15s", tmp);
            }

            free(buffer);
        }
        if (!ok) break;
        printf("\n");
    }

    fclose(f);
    return 0;
}
