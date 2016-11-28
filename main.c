#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void createTable(char *nomeArq, char *nomeTab){
    char linha[100];
    char *token;
    char *verificador;

    int ch=0;
    int grau=0;
    char *aux;
    FILE *arqComandos = fopen(nomeArq, "r");
    strcat(aux,".ctl.txt");
    FILE *arqInfos = fopen(nomeTab, "w");
    strcat(nomeTab,".dad.txt");
    FILE *arqDados= fopen(nomeTab, "w");
    fclose(arqDados);
     //CALCULA GRAU
    while(!feof(arqComandos))
    {
        ch = fgetc(arqComandos);
        if(ch == '\n')
        {
            grau++;
        }
    }
    fclose(arqComandos);
    //ABRE ARQUIVO NOVAMENTE PARA LER NOVAMENTE
    arqComandos = fopen(nomeArq, "r");
    //ESCREVE GRAU E CARDINALIDADE 0 POIS A TABELA ESTA VAZIA
    fprintf(arqInfos, "%d,0\n",grau);
    //LÊ PRIMEIRA LINHA SEM REALIZAR NENHUMA OPERAÇÃO, POIS JÁ FOI LIDA ANTERIORMENTE
    fgets(linha, sizeof(linha), arqComandos);
    //ENQUAcNTO EXISTIR UMA LINHA NOVA
    while(fgets(linha, sizeof(linha), arqComandos)){
        //LÊ E ESCREVE NOME DO ATRIBUTO
        token = strtok(linha, " ");
        fprintf(arqInfos,"%s,",token);
        token = strtok(NULL, " ");
        if(token[strlen(token)-1] == ')'){
            token[strlen(token)-1] = 0;
            fprintf(arqInfos,"%s",token);
            //acabou o comando SQL
            return;
        }
        if(token[strlen(token)-2] == ','){
            //TIRA VIRGULA E \n
            token[strlen(token)-2] = 0;
            fprintf(arqInfos,"%s\n",token);
        }else{
            fprintf(arqInfos,"%s,",token);
            token = strtok(NULL, " ");
            fprintf(arqInfos,"%s,",token);
            token = strtok(NULL, " ");
            if(token[strlen(token)-2] == ')'){
                //acabou o comando
                return;
            }
            if(token[strlen(token)-2] == ','){
                //TIRA VIRGULA E \n
                token[strlen(token)-2] = 0;
                fprintf(arqInfos,"%s\n",token);
            }else{
                fprintf(arqInfos,"%s,",token);
                //É ORDENADO!!
                token = strtok(NULL, " ");
                token[strlen(token)-2] = 0;
                fprintf(arqInfos,"%s\n",token);
            }
        }
    }
    fclose(arqComandos);
    fclose(arqInfos);

}
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
        if(fgets(linha, sizeof(linha), arqComandos)){
            token = strtok(linha, " ");
            if(strcmp(token, "CREATE")==0){
                token = strtok(NULL, " ");
                if(strcmp(token, " TABLE")){
                    //INICIAR CRIACAO DE TABELA
                    token = strtok(NULL, " ");
                    if(token){
                        fclose(arqComandos);
                        createTable(comandoSQL, token);
                        return 0;
                    }
                }
            }else{
                if(strcmp(token,"INSERT\n")==0){
                    //INICIAR INSERCAO
                }else{
                    if(strcmp(token,"SELECT")==0){
                        //INICIAR SELECAO
                    }
                }
            }

        }
        fclose(arqComandos);

    }
    return 0;
}
