#include <stdio.h>
#include <stdlib.h>

#define MAX_ATRIBUTOS 10

typedef struct { 
    char nome_atributo[15];
    char tipo;
    int  tamanho;
} Atributo;


int main (int argc, char* argv[]){

    if (argc < 2) {
       printf("Por favor, forneça o nome do arquivo: ./main <arquivo_dados>\n");
       return 0;
    }

    char *nome_arquivo = argv[1]; 


    FILE *arquivo_dados = fopen (nome_arquivo, "r");
    
    if (arquivo_dados == NULL) { 
        printf("Erro na abertura do arquivo\n");
        exit(1);

    }else{
        printf("Sucesso na abertura\n");
 
        Atributo vetor_atributos[MAX_ATRIBUTOS];
        Atributo atributo_temporario; 
        int quantidade_atributos = 0; 

        for(quantidade_atributos=0; quantidade_atributos < MAX_ATRIBUTOS + 1; quantidade_atributos++){
            
            if (quantidade_atributos == 10){
                printf("Programa não suporta mais de 10 atributos. =(");
                exit(1);
            }

            fread(&atributo_temporario, sizeof(Atributo), 1, arquivo_dados); 

            if (atributo_temporario.nome_atributo[0] == '#'){
                break;
            }else{
                printf(" %s ", atributo_temporario.nome_atributo); 
                vetor_atributos[quantidade_atributos] = atributo_temporario;
            }
        }

        int tamanho_linha_dados = 0; 
        printf("\n");
        for (int k=0; k < quantidade_atributos; k++){
            tamanho_linha_dados = tamanho_linha_dados + vetor_atributos[k].tamanho;
        }
      

        rewind(arquivo_dados); 
        fseek(arquivo_dados, quantidade_atributos * sizeof(Atributo) + 1, SEEK_CUR); 
        
        int parar_loop = 0; 

        while(!feof(arquivo_dados)){
      
            for(int k = 0; k < tamanho_linha_dados; k++){
             
                fgetc(arquivo_dados);

                if (feof(arquivo_dados)){ 
                    parar_loop = 1;
                    break;
                }
            }

            if(parar_loop){
                break;
            }

            fseek(arquivo_dados, -1 * tamanho_linha_dados, SEEK_CUR); 

            for(int i=0; i < quantidade_atributos; i++){ 
                
                char tipo_dado = vetor_atributos[i].tipo;
                int tamanho_dado = vetor_atributos[i].tamanho;

                if(tipo_dado == 'S'){ 
                    char texto_lido[tamanho_dado]; 
                    fread(texto_lido, tamanho_dado * sizeof(char), 1, arquivo_dados);
                    printf(" %s ", texto_lido);
                }else{
                    if(tipo_dado == 'I'){ 
                        int inteiro_lido = 0; 
                        fread(&inteiro_lido, tamanho_dado, 1, arquivo_dados);
                        printf(" %d ", inteiro_lido);
                    }else{
                        if(tipo_dado == 'F'){ 
                            float float_lido = 0; 
                            fread(&float_lido, tamanho_dado, 1, arquivo_dados);
                            printf(" %.2f ", float_lido);
                        }else{
                            if(tipo_dado == 'C'){ 
                                char char_lido;
                                fread(&char_lido, sizeof(char), 1, arquivo_dados);
                                printf(" %c ", char_lido);
                            }
                        }
                    }
                }
            }
            printf("\n");
        }
    }   
    
    fclose(arquivo_dados);
    return 0;
}