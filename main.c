#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void createTable(char *nomeArq, char *nomeTab){
    char linha[100];
    char *token;
    char *verificador;

    int ch=0;
    int grau=0;
    char *aux = malloc(strlen(nomeTab) + 1);
    strcpy(aux, nomeTab);
    FILE *arqComandos = fopen(nomeArq, "r");
    strcat(aux,".ctl.txt");
    FILE *arqInfos = fopen(aux, "w");
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
    //L� PRIMEIRA LINHA SEM REALIZAR NENHUMA OPERA��O, POIS J� FOI LIDA ANTERIORMENTE
    fgets(linha, sizeof(linha), arqComandos);
    //ENQUAcNTO EXISTIR UMA LINHA NOVA
    while(fgets(linha, sizeof(linha), arqComandos)){
        //L� E ESCREVE NOME DO ATRIBUTO
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
                //� ORDENADO!!
                token = strtok(NULL, " ");
                token[strlen(token)-2] = 0;
                fprintf(arqInfos,"%s\n",token);
            }
        }
    }
    fclose(arqComandos);
    fclose(arqInfos);

}
char *strsep(char **stringp, const char *delim) {
  if (*stringp == NULL) { return NULL; }
  char *token_start = *stringp;
  *stringp = strpbrk(token_start, delim);
  if (*stringp) {
    **stringp = '\0';
    (*stringp)++;
  }
  return token_start;
}
void insertTable(char *nomeArq, char *nomeTab,char *valores){
    char linha[100];
    char *token,*token2;
    char *verificador;
    int ch=0;
    int grau=0;
    char *aux = malloc(strlen(nomeTab) + 1);
    strcpy(aux, nomeTab);
    FILE *arqComandos = fopen(nomeArq, "r");
    strcat(aux,".ctl.txt");
    FILE *arqInfos = fopen(aux, "r");
    //verifica se a tabela existe
    if(arqInfos){
        //separa os valores numa matriz pra facilitar compara�ao

        //pega o grau e cardinalidade e separa os valores numa matriz
        fgets(linha, sizeof(linha), arqInfos);
        token = strtok(linha, ",");
        grau=atoi(token);
        ch=atoi(strtok(NULL,","));
        char valoresSep[grau][100];
        //posi�oes dos valores que sao chaves e que prescisao ser ordenados
        char chaves[grau];
        char ordenados[grau];
        int prescisaOrdenar=0;
        int i=0;
        int posOrd;
        for(i=0;i<grau;i++){
            chaves[i]='0';
            ordenados[i]='0';
        }
        token=strtok(valores,",");
        i=0;
        //poe os valores no vetor
        while(token){
            if(token[0]=='(') token++;
            if(token[strlen(token)-1]==')') token[strlen(token)-1] = 0;
            /*if(token[0]=='"') token++;
            if(token[strlen(token)-1]=='"') token[strlen(token)-1] = 0;
            */
            strcpy(valoresSep[i],token);
            printf("%s\n",valoresSep[i]);
            i++;
            token=strtok(NULL,",");
        }
        //se a quantidade de vlaores � correta ele continua a verificacao
        if(i==grau){
            i=0;
            int pos=0;
            //verifica se os valores sao validos
            while(fgets(linha, sizeof(linha), arqInfos)){
                token = strtok(linha, ",");
                token = strtok(NULL, ",");
                while(token){
                    //se � caracter
                    if(!strcmp(token,"C") || !strcmp(token,"C\n")){
                        if((valoresSep[i][0]!='"' || valoresSep[i][strlen(valoresSep[i])-1]!='"')){
                                if(strcmp(valoresSep[i],"NULO")!=0){
                                    return;
                                }
                        }
                    }
                    //se � int
                    else if(!strcmp(token,"I") || !strcmp(token,"I\n")){
                        if(atoi(valoresSep[i])) {
                                return;
                        }
                    }
                    //se � not null
                    else if(!strcmp(token,"nn") || !strcmp(token,"nn\n")){
                        if(strcmp(valoresSep[i],"NULO")==0) return;
                    }
                    //verifica se � chave,se for ele verifica se tem valor repetido nos dados
                    else if(!strcmp(token,"chv") || !strcmp(token,"chv\n")){
                        chaves[i]='1';
                    }
                    //verifica se prescisa ordenar
                    else if(!strcmp(token,"ord") || !strcmp(token,"ord\n")){
                        ordenados[i]='1';
                        prescisaOrdenar=1;
                        posOrd=i;
                    }
                    //printf("%s\n",token);
                    token = strtok(NULL, ",");
                }
                i++;
            }

            strcat(nomeTab,".dad.txt");
            //se o arquivo estiver vazio s� coloca no arquivo
            if(ch==0){
                printf("dasdsa");
                FILE *arqDados= fopen(nomeTab, "w");
                fprintf(arqDados,"%s,%s,%s",valoresSep[0],valoresSep[1],valoresSep[2]);
                fclose(arqDados);
            }
            else{
                //verifica se as chaves se repetem
                i=0;
                int j;
                FILE *arqDados;
                char linhaDados[100];
                for(i=0;i<grau;i++){
                    if(chaves[i]=='1'){
                        arqDados= fopen(nomeTab, "r");
                        while(fgets(linhaDados, sizeof(linhaDados), arqDados)){
                                token=strtok(linhaDados,",");
                                j=0;
                                while(j!=i){
                                    token=strtok(NULL,",");
                                    j++;
                                }
                                //depois de achado o valor da linha da coluna que quero,eu verifico se sao iguais,se sim return
                                if(strcmp(token,valoresSep[i])==0) return;

                        }
                        fclose(arqDados);
                    }
                }
                //se prescisar ordernar � prescisa fazer verifica��o nessa insercao
                if(prescisaOrdenar){
                    int salvouAtual=0;
                    int posSalvou=0;
                    char linhaAux[100];
                    arqDados= fopen(nomeTab, "r+");
                    FILE *arqMaior=fopen("auxDadosMaior.txt","w+");
                    FILE *arqMenor=fopen("auxDadosMenor.txt","w+");
                    //separo em dois arquivos,um antes e outro depois do inserido
                    while(fgets(linhaDados, sizeof(linhaDados), arqDados)){
                            strcpy(linhaAux,linhaDados);
                            token=strtok(linhaDados,",");
                            j=0;
                            while(j!=posOrd){
                                token=strtok(NULL,",");
                                j++;
                            }

                            //depois de achado o valor da linha da coluna que quero,eu verifico se sao iguais,se sim return
                            printf("%s AAA %s\n",token,valoresSep[posOrd]);
                            if(strcmp(token,valoresSep[posOrd])<0){

                                fprintf(arqMenor,"%s",linhaAux);
                                posSalvou++;
                            }
                            else if(!salvouAtual){
                                fprintf(arqMaior,"%s,%s,%s\n",valoresSep[0],valoresSep[1],valoresSep[2]);
                                fprintf(arqMaior,"%s",linhaAux);
                                salvouAtual=1;
                            }
                            else{
                                fprintf(arqMaior,"%s",linhaAux);
                            }

                    }
                    fclose(arqMaior);
                    fclose(arqMenor);
                    fclose(arqDados);
                    //junto os dois arquivos
                    arqDados= fopen(nomeTab, "w+");
                    arqMaior=fopen("auxDadosMaior.txt","r");
                    arqMenor=fopen("auxDadosMenor.txt","r");
                    printf("\n%d aqui\n",posSalvou);

                    while(fgets(linhaAux, sizeof(linhaAux), arqMenor)){
                        printf("%s",linhaAux);
                        fprintf(arqDados,"%s",linhaAux);
                    }
                    fclose(arqMenor);
                    while(fgets(linhaAux, sizeof(linhaAux), arqMaior)){
                        fprintf(arqDados,"%s",linhaAux);
                    }
                    fclose(arqMaior);
                    if(!salvouAtual){
                        FILE *arqDados= fopen(nomeTab, "a");
                        fprintf(arqDados,"%s,%s,%s\n",valoresSep[0],valoresSep[1],valoresSep[2]);
                        fclose(arqDados);
                    }
                    fclose(arqDados);
                }
                //se ele for o maior insere no final
                else{
                    printf("oi");
                    FILE *arqDados= fopen(nomeTab, "a");
                    fprintf(arqDados,"%s,%s,%s\n",valoresSep[0],valoresSep[1],valoresSep[2]);
                    fclose(arqDados);
                }
            }

        }
        fclose(arqInfos);
        fclose(arqComandos);
        //aumenta a cardinalidade
        arqInfos = fopen(aux, "r+");
        ch++;
        fprintf(arqInfos,"%d,%d",grau,ch);
        printf("feito");
    }
    fclose(arqInfos);
    fclose(arqComandos);
}
int main(int argc, char *argv[]){
    char comandoSQL[30];
    //scanf("%s", &comandoSQL);
    FILE *arqComandos = fopen("comando.txt", "r");
    if(!arqComandos){
        printf("arquivo nao localizado");
        return 0;
    }else{
        printf("Arquivo Valido, lendo comando...\n");
        char linha[100];
        char *resultado;
        char *token;
        //l� linha se for v�lida.
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
                if(strcmp(token,"INSERT")==0){
                    token = strtok(NULL, " ");
                    if(strcmp(token, " INTO")){
                        //INICIAR CRIACAO DE TABELA
                        token = strtok(NULL, " ");

                        if(token){
                            char *tabela=token;
                            token = strtok(NULL, " ");

                            if(strcmp(token, " VALUES")){
                                token = strtok(NULL, " ");
                                if(token){
                                    fclose(arqComandos);
                                    insertTable(comandoSQL, tabela,token);
                                    return 0;
                                }
                            }
                        }
                    }
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
