#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "algebra.h"

void createTable(char *nomeArq, char *nomeTab){
    char linha[100];
    char *token;
    char *verificador;
    int ch=0;
    int grau=0;
    char *aux = malloc(strlen(nomeTab) + 1);
    strcpy(aux, nomeTab);
    FILE *arqComandos = fopen(nomeArq, "r");
    strcat(aux,".ctl");
    FILE *arqInfos = fopen(aux, "w");
    strcat(nomeTab,".dad");
    FILE *arqDados= fopen(nomeTab, "w");
    fclose(arqDados);
     //CALCULA GRAU
    while(!feof(arqComandos))
    {
        printf("aa");
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
    printf("aaa");
    while(fgets(linha, sizeof(linha), arqComandos)){
        //LÊ E ESCREVE NOME DO ATRIBUTO
        printf("LINHA: %s\n", linha);
        token = strtok(linha, " ");
        fprintf(arqInfos,"%s",token);//Escreve nome do atributo
        printf("Escrevi: %s\n", token);
        token = strtok(NULL, " ");
        if(token[strlen(token)-1] == ')'){//tirei o caso do -2, ver se da erro
                //Acabou comando
                token[strlen(token)-1]=0;
                if(strcmp(token, "STRING")==0){
                    char tipo = 'C';
                    fprintf(arqInfos,",%c", tipo);//Escreve tipo do atributo
                    printf("Escrevi: %c\n", tipo);
                }else{
                    char tipo = 'I';
                    fprintf(arqInfos,",%c", tipo);//Escreve tipo do atributo
                    printf("Escrevi: %c\n", tipo);
                }

                break;
        }
        if(token[strlen(token)-2] == ','){
            token[strlen(token)-2]=0;
            strcat(token, "\n");
            if(strcmp(token, "STRING\n")==0){
                char tipo = 'C';
                fprintf(arqInfos,",%c\n", tipo);//Escreve tipo do atributo
                printf("Escrevi: %c\n", tipo);
            }else{
                char tipo = 'I';
                fprintf(arqInfos,",%c\n", tipo);//Escreve tipo do atributo
                printf("Escrevi: %c\n", tipo);
            }
            //Acabou essa linha
        }else{
            if(strcmp(token, "STRING")==0){
                char tipo = 'C';
                fprintf(arqInfos,",%c", tipo);//Escreve tipo do atributo
                printf("Escrevi: %c\n", tipo);
            }else{
                char tipo = 'I';
                fprintf(arqInfos,",%c", tipo);//Escreve tipo do atributo
                printf("Escrevi: %c\n", tipo);
            }
            token = strtok(NULL, " ");
            if(token[strlen(token)-1] == ')'){
                //Acabou comando
                token[strlen(token)-1]=0;
                fprintf(arqInfos,",%s",token);
                printf("Escrevi: %s\n", token);
                break;
            }
            if(token[strlen(token)-2] == ','){
                token[strlen(token)-2]=0;
                strcat(token, "\n");
                fprintf(arqInfos,",%s",token);//Escreve nn
                printf("Escrevi: %s\n", token);
                //Acabou essa linha
            }else{
                fprintf(arqInfos,",%s",token);//Escreve nn
                printf("Escrevi: %s\n", token);
                token = strtok(NULL, " ");
                if(token[strlen(token)-1] == ')'){
                    //Acabou comando
                    token[strlen(token)-1]=0;
                    fprintf(arqInfos,",%s",token);
                    printf("Escrevi: %s\n", token);
                    break;
                }
                if(token[strlen(token)-2] == ','){
                token[strlen(token)-2]=0;
                strcat(token, "\n");
                fprintf(arqInfos,",%s",token);//Escreve chv
                printf("Escrevi: %s\n", token);
                //Acabou essa linha
                }else{
                    fprintf(arqInfos,",%s",token);//Escreve chv
                    printf("Escrevi: %s\n", token);
                    token = strtok(NULL, " ");
                    if(token[strlen(token)-1] == ')'){
                        //Acabou comando
                        token[strlen(token)-1]=0;
                        fprintf(arqInfos,",%s",token);
                        printf("Escrevi: %s\n", token);
                        break;
                    }
                    if(token[strlen(token)-2] == ','){
                        token[strlen(token)-2]=0;
                        strcat(token, "\n");
                        fprintf(arqInfos,",%s",token);//Escreve ord
                        printf("Escrevi: %s", token);
                        //Acabou essa linha
                    }
                }

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
    aux[strlen(aux)-1] = 0;
    strcat(aux,".ctl");
    FILE *arqInfos = fopen(aux, "r");
    //verifica se a tabela existe
    if(arqInfos){
        //separa os valores numa matriz pra facilitar comparaçao
        //pega o grau e cardinalidade e separa os valores numa matriz
        fgets(linha, sizeof(linha), arqInfos);
        token = strtok(linha, ",");
        grau=atoi(token);
        ch=atoi(strtok(NULL,","));
        char valoresSep[grau][100];
        //posiçoes dos valores que sao chaves e que prescisao ser ordenados
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
        //separa os valores que vem depois do VALUES
        //poe os valores no vetor
        while(token){
            if(token[0]=='(') token++;
            if(token[strlen(token)-1]==')') token[strlen(token)-1] = 0;
            /*if(token[0]=='"') token++;
            if(token[strlen(token)-1]=='"') token[strlen(token)-1] = 0;
            */
            strcpy(valoresSep[i],token);
            i++;
            token=strtok(NULL,",");
        }
        //se a quantidade de vlaores é correta ele continua a verificacao
        if(i==grau){
            i=0;
            int pos=0;
            //verifica se os valores sao validos
            while(fgets(linha, sizeof(linha), arqInfos)){
                token = strtok(linha, ",");
                token = strtok(NULL, ",");
                while(token){
                    //se é caracter
                    if(!strcmp(token,"C") || !strcmp(token,"C\n")){
                        if((valoresSep[i][0]!='"' || valoresSep[i][strlen(valoresSep[i])-1]!='"')){
                                if(strcmp(valoresSep[i],"NULO")!=0){
                                    printf("Erro nos tipos das variaveis");
                                    return;
                                }
                        }
                    }
                    //se é int
                    else if(!strcmp(token,"I") || !strcmp(token,"I\n")){
                        int k,ehZero=1;
                        for(k=0;k<strlen(valoresSep[i]);k++){
                            if(valoresSep[i][k]!='0'){
                                ehZero=0;
                                break;
                            }
                        }
                        if((!atoi(valoresSep[i])) && !ehZero) {
                                printf("Erro nos tipos das variaveis NUM");
                                return;
                        }
                    }
                    //se é not null
                    else if(!strcmp(token,"nn") || !strcmp(token,"nn\n")){
                        if(strcmp(valoresSep[i],"NULO")==0) {
                            printf("Erro nos tipos das variaveis");
                            return;
                        }
                    }
                    //verifica se é chave,se for ele verifica se tem valor repetido nos dados
                    else if(!strcmp(token,"chv") || !strcmp(token,"chv\n")){
                        chaves[i]='1';
                    }
                    //verifica se prescisa ordenar
                    else if(!strcmp(token,"ord") || !strcmp(token,"ord\n")){
                        ordenados[i]='1';
                        prescisaOrdenar=1;
                        posOrd=i;
                    }
                    token = strtok(NULL, ",");
                }
                i++;
            }
            nomeTab[strlen(nomeTab)-1] = 0;
            strcat(nomeTab,".dad");
            //se o arquivo estiver vazio só coloca no arquivo
            if(ch==0){
                FILE *arqDados= fopen(nomeTab, "w");
                for(i=0;i<grau;i++){
                    if(i==grau-1) fprintf(arqDados,"%s",valoresSep[i]);
                    else fprintf(arqDados,"%s,",valoresSep[i]);
                }
                fprintf(arqDados,"\n");
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
                                if(token[strlen(token)-1]=='\n') token[strlen(token)-1] = 0;
                                if(strcmp(token,valoresSep[i])==0) return;
                        }
                        fclose(arqDados);
                    }
                }
                //se prescisar ordernar é prescisa fazer verificação nessa insercao
                if(prescisaOrdenar)
                {
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
                            if((strlen(token)==strlen(valoresSep[posOrd])&& strcmp(token,valoresSep[posOrd])<0) || (strlen(token)<strlen(valoresSep[posOrd]))){
                                fprintf(arqMenor,"%s",linhaAux);
                                posSalvou++;
                            }
                            else if(!salvouAtual){
                                for(i=0;i<grau;i++){
                                    if(i==grau-1) fprintf(arqMaior,"%s",valoresSep[i]);
                                    else fprintf(arqMaior,"%s,",valoresSep[i]);
                                }
                                fprintf(arqMaior,"\n");
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
                    while(fgets(linhaAux, sizeof(linhaAux), arqMenor)){
                        fprintf(arqDados,"%s",linhaAux);
                    }
                    fclose(arqMenor);
                    while(fgets(linhaAux, sizeof(linhaAux), arqMaior)){
                        fprintf(arqDados,"%s",linhaAux);
                    }
                    fclose(arqMaior);
                    if(!salvouAtual){
                        for(i=0;i<grau;i++){
                            if(i==grau-1) fprintf(arqDados,"%s",valoresSep[i]);
                            else fprintf(arqDados,"%s,",valoresSep[i]);
                        }
                        fprintf(arqDados,"\n");
                        fclose(arqDados);
                    }
                    fclose(arqMaior);
                    fclose(arqMenor);
                    remove("auxDadosMaior.txt");
                    remove("auxDadosMenor.txt");
                    fclose(arqDados);
                }
                //se ele for o maior insere no final
                else{
                    FILE *arqDados= fopen(nomeTab, "a");
                    for(i=0;i<grau;i++){
                        if(i==grau-1) fprintf(arqDados,"%s",valoresSep[i]);
                        else fprintf(arqDados,"%s,",valoresSep[i]);
                    }
                    fprintf(arqDados,"\n");
                    fclose(arqDados);
                }
            }

			//aumenta a cardinalidade
			arqInfos = fopen(aux, "r+");
			ch++;
			fprintf(arqInfos,"%d,%d",grau,ch);
        }
		else{
			printf("Erro na quantidade de valores inseridos");
            fclose(arqInfos);
            fclose(arqComandos);
            return;
		}
        fclose(arqInfos);
        fclose(arqComandos);
    }
	else{
		printf("Erro de arquivo");
		return;
    }
    printf("Insercao concluida");
    fclose(arqInfos);
    fclose(arqComandos);
}

int main(int argc, char *argv[]){
    char comandoSQL[30];

    if(argv[1])strcpy(comandoSQL,argv[1]);
    else scanf("%s", comandoSQL);

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
                        printf("%s",token);
                        createTable(comandoSQL, token);
                        return 0;
                    }
                }
            }else{
                if(strcmp(token,"INSERT\n")==0){
                    if(fgets(linha, sizeof(linha), arqComandos)){
                        token = strtok(linha, " ");
                        if(strcmp(token, "INTO")==0){
                            //INICIAR CRIACAO DE TABELA
                            token = strtok(NULL, " ");
                            if(token)
                            {
                                char tabela[20];
                                strcpy(tabela,token);
                                if(fgets(linha, sizeof(linha), arqComandos))
                                {
                                    token = strtok(linha, " ");
                                    if(strcmp(token, "VALUES")==0)
                                    {
                                        token = strtok(NULL, " ");
                                        if(token){
                                            fclose(arqComandos);
                                            insertTable(comandoSQL, tabela,token);
                                            return 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }else{
                    if(strcmp(token,"SELECT")==0){
                        //INICIAR SELECAO

                        char saida[50];
                        strcpy(saida,"comandoAlgebra.alg");
                        FILE *arqAlgebra = fopen(saida, "wt");

			        	printf("abriu o arq");

                        token = strtok(NULL, " \n");

                        char result[50];
                        strcpy(result,token);

                        //strcmp(result,"*") == 0)
                        char atributos[100];
						strcpy(atributos,result);
                        char aux1[100];

                        char relA[20];
                        char arqAnterior[20];
                        int nAtributos=0;

                        if(strcmp(atributos,"*")){
                        	nAtributos = 1;
                        	token = strtok(NULL,",\n");
                        	while(token){
                        		strcat(atributos,token);
                        		nAtributos++;
                        		token = strtok(NULL,",\n");
                        	}
                        }


                        if(fgets(aux1, sizeof(aux1), arqComandos)){
                        	token = strtok(aux1, " ");
                        	printf("%s",token);
                        	if(strcmp(token,"FROM")==0){
                        		token = strtok(NULL, " (");
                        		//char relA[20];
                        		strcpy(relA,token);

                        		strcpy(arqAnterior,relA);

                        		//verifica se existe join
                        		token = strtok(NULL, " ");
                        		if(token){
                        			//existe join
                        			if(strcmp(token,"JOIN")==0){
                        				char *relB = strtok(NULL, " ");
                        				char cond[50];
                        				token = strtok(NULL, " ");
                        				if(strcmp(token,"ON")==0){
                        					token = strtok(NULL, " )");
                        					strcpy(cond,token);


                        					//faltam os tratamentos pra tirar \n e coisas assim
                        					char comando[100]; //vai concatenar td
                   							strcpy(comando,"J(");
                   							strcat(comando,relA);
                   							strcat(comando,",");
                   							strcat(comando,relB);
                   							strcat(comando,",");
                   							strcat(comando,cond);
                   							strcat(comando,",");
                   							strcat(comando,"RESULTADO_JUNCAO");
                   							strcpy(arqAnterior,"RESULTADO_JUNCAO");
                   							strcat(comando,")");

                   							//printf("%s",comando);
                   							fprintf(arqAlgebra,"%s\n", comando);

                        					interpreta(comando);

										}
										else{
											printf("comando invalido");
										}
									}
									else{
										printf("comando invalido");
									}
                        		}

                        		//verifica se existe WHERE
                        		char aux2[100];
                        		if(fgets(aux2, sizeof(aux2), arqComandos)){
                        			token = strtok(aux2, " ");
                        			if(strcmp(token,"WHERE")==0){

                        				//char *val = strtok(NULL, "=<>");

                   						token = strtok(NULL, " ");

                   						char cond[100], cond_aux[100];
                   						strcpy(cond,token); //cond guarda a condicao de selecao
                   						strcpy(cond_aux,cond);

                   						char op[3];
                   						printf("condicao: %s",cond);
                   						if((strstr(cond,"<>") != NULL)){
                   						 	strcpy(op,"<>");
                   						}
                   						else if(strstr(cond,"<=") != NULL){
               								strcpy(op,"<=");
               							}
               							else if(strstr(cond,">=") != NULL){
           									strcpy(op,">=");
           								}
           								else if(strstr(cond,"=") != NULL){
       											strcpy(op,"=");
       									}
       									else if(strstr(cond,">") != NULL){
       										strcpy(op,">");
   										}
                   						else if(strstr(cond,"<") != NULL){
											strcpy(op,"<");
										}
										else{
											exit(1);
										}

                   						//printf("%s",op);

                   						char *atr = strtok(cond,op);
                   						char *val = strtok(NULL,op);

                   						printf("%s",atr);
                   						printf("%s",val);

                   						relA[strlen(relA)-1]=0;
                   						val[strlen(val)-1]=0;
                   						char comando[100]; //vai concatenar td
                   						strcpy(comando,"S(");
                                        tiraQuebra(arqAnterior);//tira quebra de linha se ela existe
                   						strcat(comando,arqAnterior); //caso não tenha join arqAnterior tem o mesmo valor de relA
                   						strcat(comando,",");
                   						strcat(comando,atr);
                   						strcat(comando,",");
                   						strcat(comando,op);
                   						strcat(comando,",");
                   						strcat(comando,val);
                   						strcat(comando,",");
                   						strcat(comando,"RESULTADO_SELECAO");

                   						strcpy(arqAnterior,"RESULTADO_SELECAO");

                   						strcat(comando,")");

                   						//printf("%s",comando);
                   						fprintf(arqAlgebra,"%s\n", comando);

                        				interpreta(comando);
                                        imprimeTabela(arqAnterior);
                        			}
                        			else{
                        				// TA ERRADO TEM Q MODIFICAR ESSE CASO DO IMPRIME TABELA
                        				// PRECISA CHECAR ANTES SE ELE N TEM PROJECAO

                        				imprimeTabela(relA); //o resultado e a propria tabela
                        			}


                        			if(strcmp(atributos,"*"))
                        			{
                        				//faz projeção
                        				char comando[100];
                        				strcpy(comando,"P(");
                        				strcat(comando,arqAnterior);
                        				strcat(comando,",");
                        				//transforma int para string
                        				char nAtrib[10];
                        				sprintf(nAtrib, "%d", nAtributos);
                        				strcat(comando,nAtrib);
                        				strcat(comando,",");
                        				strcat(comando,atributos);
                        				strcat(comando,",");
                        				strcat(comando,"RESULTADO_PROJECAO");
                        				strcat(comando,")");

                        				fprintf(arqAlgebra,"%s\n",comando);
                        				interpreta(comando);
                        			}
                        		}
                        	}
                    		else{
                       			//nunca entra nesse caso
                       			//pois sempre tem FROM
                       			printf("comando invalido");
                    		}
                        }
                    fclose(arqAlgebra);
                }
            }

        }
        fclose(arqComandos);
    	}
    return 0;
	}
}
