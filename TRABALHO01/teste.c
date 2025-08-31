#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ATTR 10

typedef struct {
    char name[16]; // 15 + '\0'
    char type;     // S, C, I, F
    int size;
} Attribute;

int main(void) {
    FILE *f = fopen("customer.dat", "rb");
    if (!f) { perror("Erro ao abrir customer.dat"); return 1; }

    Attribute attrs[MAX_ATTR];
    int num_attrs = 0;

    /* ===== ler cabeçalho ===== */
    for (int i = 0; i < MAX_ATTR; i++) {
        unsigned char name_raw[15];
        if (fread(name_raw, 1, 15, f) != 15) break;

        if (name_raw[0] == (unsigned char)'#') {
            /* sentinela: o primeiro byte '#' foi lido; 
               os próximos 14 bytes pertencem ao início do primeiro registro,
               portanto retrocedemos 14 bytes para começar leitura do registro
               no byte logo após '#' */
            if (fseek(f, -14, SEEK_CUR) != 0) {
                perror("fseek");
                fclose(f);
                return 1;
            }
            break;
        }

        /* se não for '#', lemos type e size */
        char type;
        int size;
        if (fread(&type, 1, 1, f) != 1) { perror("read type"); fclose(f); return 1; }
        if (fread(&size, sizeof(int), 1, f) != 1) { perror("read size"); fclose(f); return 1; }

        memcpy(attrs[num_attrs].name, name_raw, 15);
        attrs[num_attrs].name[15] = '\0';
        attrs[num_attrs].type = type;
        attrs[num_attrs].size = size;
        num_attrs++;
    }

    /* imprimir cabeçalho */
    for (int i = 0; i < num_attrs; i++) printf("%-15s", attrs[i].name);
    printf("\n");

    /* ===== ler registros ===== */
    while (1) {
        int ok = 1;
        for (int i = 0; i < num_attrs; i++) {
            Attribute a = attrs[i];
            unsigned char *buf = malloc(a.size);
            if (!buf) { perror("malloc"); fclose(f); return 1; }

            if (fread(buf, a.size, 1, f) != 1) {
                free(buf);
                ok = 0;
                break;
            }

            if (a.type == 'S') {
                /* string fixa: copiar e garantir terminação */
                char *tmp = malloc(a.size + 1);
                memcpy(tmp, buf, a.size);
                tmp[a.size] = '\0';
                /* cortar na primeira posição não-printable ou '\0' */
                int j = 0;
                while (tmp[j] && isprint((unsigned char)tmp[j])) j++;
                tmp[j] = '\0';
                printf("%-15s", tmp);
                free(tmp);
            } else if (a.type == 'C') {
                printf("%-15c", (char)buf[0]);
            } else if (a.type == 'I') {
                int val = 0;
                if (a.size == 4) {
                    memcpy(&val, buf, 4); /* assume little-endian */
                } else if (a.size == 2) {
                    short s; memcpy(&s, buf, 2); val = s;
                } else {
                    /* fallback: montar little-endian manual até 4 bytes */
                    val = 0;
                    for (int b = 0; b < a.size && b < 4; b++) val |= (buf[b] << (8*b));
                }
                printf("%-15d", val);
            } else if (a.type == 'F') {
                if (a.size == 4) {
                    float fv; memcpy(&fv, buf, 4); printf("%-15.2f", fv);
                } else if (a.size == 8) {
                    double dv; memcpy(&dv, buf, 8); printf("%-15.2lf", dv);
                } else {
                    printf("%-15s", "<F?>");
                }
            } else {
                printf("%-15s", "<?>");
            }

            free(buf);
        }
        if (!ok) break;
        printf("\n");
    }

    fclose(f);
    return 0;
}
