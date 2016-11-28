#include <stdlib.h>
#include <stdio.h>
int main(int argc, char *argv[]){
    char comandoSQL[30];
    scanf("%s", &comandoSQL);
    FILE *arqComandos = fopen(comandoSQL, "r");
    if(!arqComandos){
        printf("arquivo nao localizado");
        return 0;
    }else{
        printf("Arquivo Valido, lendo comando...\n");
        char linha[100];
        char *resultado;
        char *token;
        //lê linha se for válida.
        while(fgets(linha, sizeof(linha), arqComandos)){
            token = strtok(linha, " ");
            if(strcmp(token, "CREATE")==0){
                token = strtok(NULL, " ");
                if(strcmp(token, " TABLE")){
                    //INICIAR CRIACAO DE TABELA
                }
            }else{
                if(strcmp(token,"INSERT\n")==0){
                    printf("eh insert");
                }
            }
            return 0;

        }
        fclose(arqComandos);

    }
    return 0;
}
