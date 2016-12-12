#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//lista de atributos para auxiliar comparação e escrita
typedef struct atributo
{
	char *atr;
	char *at;
	int indice;
	struct atributo *prox;
}TAtr;

TAtr *insere_fim(TAtr *l,char *atr, int i)
{
	TAtr *aux = l;
	TAtr *novo = (TAtr*) malloc(sizeof(TAtr));
	novo->indice = i;
	novo->prox = NULL;
	novo->atr = malloc(sizeof(atr));
	novo->at = malloc(sizeof(atr));
	//tira o \n da linha do arquivo
	char *aux_atr = strtok(atr,"\n");
	//copia a linha do catálogo
	strcpy(novo->atr,aux_atr);	
	aux_atr = strtok(aux_atr,",");
	strcpy(novo->at,aux_atr);	
	//copia exatamento o nome do atributo
	if(aux) 
	{
		while(aux->prox) aux = aux->prox;
		aux->prox = novo;
		return l;
	}
	return novo;
}

void libera(TAtr *a)
{
	if(a)
	{
		libera(a->prox);
		free(a->at);
		free(a->atr);
		free(a);
	}
}

void imprime(TAtr *a)
{
	if(a)
	{
		printf("%s %d\n",a->atr,a->indice);
		imprime(a->prox);
	}
}

TAtr *busca(TAtr *a, char *valor)
{
	while(a) 
	{
		if(strcmp(a->at,valor) == 0)
		{
			return a;
		}
		a = a->prox;
	}
	return NULL;
}

TAtr *junta_mudando_nomes(TAtr *a, TAtr *b,char *relA, char *relB)
{
	TAtr *resp = NULL;
	if(!strcmp(relA,relB))
	{
		TAtr *aux = a;
		int i = 1;
		while(aux)
		{
			char atributo[sizeof(aux->atr) + 2],con[2];
			sprintf(con, "%d", i);
			
			//separa so o nome do atributo
			char *nome = strtok(aux->atr,",");
			strcpy(atributo, nome);
			strcat(atributo, con);
			strcat(atributo,",");
			//pega o resto da linha com o not null e ordenação
			nome = strtok(NULL,"");
			strcat(atributo,nome);
			//----------------------------
			strcpy(aux->atr,atributo);
			aux = aux->prox;
			i++;
		}
		
		aux = b;
		i = 1;
		while(aux)
		{
			char atributo[sizeof(aux->atr) + 2],con[2];
			sprintf(con, "%d", i);

			//separa so o nome do atributo
			char *nome = strtok(aux->atr,",");
			strcpy(atributo, nome);
			strcat(atributo, con);
			strcat(atributo,",");
			//pega o resto da linha com o not null e ordenação
			nome = strtok(NULL,"");
			strcat(atributo,nome);
			//----------------------------
			strcpy(aux->atr,atributo);
			aux = aux->prox;
			i++;
		}
	}
	else
	{
		TAtr *aux = a;
		while(aux)
		{
			char atributo[sizeof(aux->atr) + sizeof(relA)];
			strcpy(atributo,relA);
			strcat(atributo,aux->atr);
			strcpy(aux->atr,atributo);
			aux = aux->prox;
		}
		
		aux = b;
		while(aux)
		{
			char atributo[sizeof(aux->atr) + sizeof(relB)];
			strcpy(atributo,relB);
			strcat(atributo,aux->atr);
			strcpy(aux->atr,atributo);
			aux = aux->prox;
		}
	}
	
	while(a)
	{
		resp = insere_fim(resp,a->atr, a->indice);
		a = a->prox;
	}
	
	while(b)
	{
		resp = insere_fim(resp,b->atr, b->indice);
		b = b->prox;
	}
	return resp;
}

void interpreta(char *a);

char *geraNomeArq(char *rel, char *extensao)
{
	char *result = malloc(sizeof(char) * 20);
	strcpy(result,rel);
	strcat(result,extensao);
	return result;
}

int compara(char *op, char *val1, char *val2)
{
	if(strcmp(op,"<") == 0)
	{
		return strcmp(val1,val2);
	}
	
	if(strcmp(op,">") == 0)
	{
		return strcmp(val2,val1);
	}
	if(strcmp(op,"=") == 0)
	{		
		int i = strcmp(val1,val2);
		if(i == 0) return 1;
		return 0;
	}
	if(strcmp(op,"<=") == 0)
	{
		if(strcmp(val1,val2) <= 0)
			return 1;
		return 0;
	}
	if(strcmp(op,">=") == 0)
	{
		if(strcmp(val1,val2) >= 0)
			return 1;
		return 0;
	}
	if(strcmp(op,"<>") == 0)
	{
		return strcmp(val1,val2);
	}
}

void lerAlgebra(char *arq)
{
	FILE *alg = fopen(arq,"rt");
	if(!alg) exit(1);
	char linha[50];
	while(fgets(linha, sizeof(linha), alg))
	{
		interpreta(linha);
	}
	fclose(alg);
}

void selecao(char *relacao, char *atr, char *op, char *val, char *saida)
{
	//copiar catalogo para o saida e depois copiar as tuplas que interessam ao op e ao val
	char *aux;
	
	//ajusta o nome do arquvio que será aberto
	aux = geraNomeArq(relacao,".ctl");
	FILE *frelacao = fopen(aux,"rd");
	free(aux);
	if(!frelacao)exit(1);
	//ajusta o nome do arquvio que será aberto
	aux = geraNomeArq(saida,".ctl");
	FILE *fsaida = fopen(aux,"wt");
	free(aux);
	if(!fsaida)exit(1);	
	
	//linha do arquivo que será lido
	char linha[50];
	int i = 0,atrib = -1;	
	
	//faz a primeira leitura para guardar o grau da relação(não muda na operação de seleção)
	fgets(linha, sizeof(linha), frelacao);
	//escreve essa linha no arquivo, sendo que ela será modificada no final para atualizar a cardinalidade
	fprintf(fsaida,"%s", linha);	

	//guarda o valor do grau da relação
	//ao percorrer o while, o valor de grau_aux muda, por isso tem que copiar o conteúdo
	char *grau_aux = strtok(linha,",");
	char grau[20];
	strcpy(grau,grau_aux);
	
	//copia o ".ctl"
	while(fgets(linha, sizeof(linha), frelacao))
	{
		fprintf(fsaida,"%s", linha);
		//daqui pra baixo é pra saber qual atributo devo comparar, pra ler o ".clt" uma vez só
		char *atrname = strtok(linha,",");
		if(!strcmp(atr,atrname))atrib = i;
		i++;
		
	}
	//fecha os arquivos de ".ctl"
	fclose(fsaida);
	fclose(frelacao);

	//começa a leitura do arquivo de dados
	aux = geraNomeArq(relacao,".dad");
	frelacao = fopen(aux,"rt");
	free(aux);
	if(!frelacao)exit(1);
	
	aux = geraNomeArq(saida,".dad");
	fsaida = fopen(aux,"wt");
	free(aux);
	if(!fsaida)exit(1);	

	char linha_aux[100];
	//guarda a cardinalidade da relação
	int cont = 0;
	
	while(fgets(linha, sizeof(linha), frelacao))
	{
		strcpy(linha_aux,linha);
		char *tkn = strtok(linha,",");
		//serve pra chegar no atributo que quero
		for(i = 1; i < atrib ;i++) tkn = strtok(NULL,","); 
		//se a comparar funcionar, escreve o arquivo a atualiza o contador
		if(compara(op,val,tkn)) 
		{
			fprintf(fsaida,"%s", linha_aux);
			cont++;
		}
	}
	
	fclose(fsaida);
	fclose(frelacao);
	
	//ajusta a cardinalidade da relação
	aux = geraNomeArq(saida,".ctl");
	fsaida = fopen(aux,"rt+");
	free(aux);
	if(!fsaida)exit(1);	
	//transforma o contador em string
	char con[3];
	sprintf(con, "%d", cont);
	//-------------------------------
	strcat(grau,",");
	strcat(grau,con);
	fprintf(fsaida,"%s", grau);
	fclose(fsaida);
}


char *pegaVal(TAtr *a, char *atr, char *linha)
{
	char *tkn = strtok(linha," ,");
	char *resp = malloc(sizeof(char) * 20);
	while(a && strcmp(atr,a->at))
	{
		tkn = strtok(NULL," ,");
		a = a->prox;
	}
	strcpy(resp,tkn);
	return resp;
}

char *junta(char *linhaA, char *linhaB)
{
	strcat(linhaA,",");
	strcat(linhaA,linhaB);
	strcat(linhaA,"\n");
	return linhaA;
}

int conta(TAtr *a)
{
	int i = 0;
	while(a)
	{
		i++;
		a = a->prox;
	}
	return i;
}

///J(A,B,condição-juncao,Z)
void juncao(char *relA, char *relB, char *con, char *saida)
{
	//separa os atributos da comparação
	char *tkn = strtok(con,"=");
	char atrA[20], atrB[20];
	strcpy(atrA,tkn);
	tkn = strtok(NULL,"=");
	strcpy(atrB,tkn);

	//abre os arquivos de catálogo para descobrir qual será o atributo testado
	char *arq = geraNomeArq(saida,".ctl");
	FILE *fsaida = fopen(arq,"wt");
	if(!fsaida)exit(1);
	free(arq);
	
	arq = geraNomeArq(relA,".ctl");
	FILE *arqA = fopen(arq,"rt");
	if(!arqA)exit(1);
	free(arq);
		
	char linha[30];
	int i = 0;
	//lista auxiliares para construção do arquivo
	TAtr *a = NULL, *b = NULL;
	
	//guarda na primeira linha do arquivo ctl os espaços para o grau e a cardinalidade
	fgets(linha, sizeof(linha), arqA);	
	while(fgets(linha, sizeof(linha), arqA))
	{
		if(strcmp(linha,"\n"))a = insere_fim(a,linha,i);
		i++;
	}
		
	arq = geraNomeArq(relB,".ctl");
	FILE *arqB = fopen(arq,"rt");
	if(!arqB)exit(1);
	free(arq);
	
	//descarta primeira linha do arquivo
	fgets(linha, sizeof(linha), arqB);
	//faz a leitura dos atributos	 
	i = 0;
	
	while(fgets(linha, sizeof(linha), arqB))
	{
		if(strcmp(linha,"\n"))b = insere_fim(b,linha,i);
		i++;
	}

	TAtr *atributos = junta_mudando_nomes(a,b,relA,relB), *aux;
	aux = atributos;
	
	//copia os atributos pro arquivo ctl
	while(aux)
	{
		fprintf(fsaida,"%s\n",aux->atr);
		aux = aux->prox;
	}

	fclose(fsaida);
	fclose(arqA);
	fclose(arqB);
	
	//testa os atributos e escreve no outro arquivo
	arq = geraNomeArq(relA,".dad");
	arqA = fopen(arq,"rt");
	if(!arqA)exit(1);
	free(arq);	
	
	arq = geraNomeArq(relB,".dad");
	arqB = fopen(arq,"rt");
	if(!arqB)exit(1);
	free(arq);
	
	arq = geraNomeArq(saida,".dad");
	fsaida = fopen(arq,"wt");
	if(!arqB)exit(1);
	free(arq);	
	
	char linhaA[50], linhaB[50];
	int card = 0;
	
	while(fgets(linhaA, sizeof(linhaA), arqA) && strcmp(linhaA,"\n"))
	{
		char auxA[sizeof(linhaA)];
		//tira o \n
		char *val = strtok(linhaA,"\n");
		strcpy(auxA,val);
		char *valA = pegaVal(a,atrA,auxA);
		
		while(fgets(linhaB, sizeof(linhaB), arqB) && strcmp(linhaB,"\n"))
		{
			char auxB[sizeof(linhaB)];
			//tira o \n
			val = strtok(linhaB,"\n");
			strcpy(auxB,val);
			//---------
			char *valB = pegaVal(b,atrB,auxB);
			if(compara("=",valA,valB))
			{
				char *linha = junta(linhaA,linhaB);
				fprintf(fsaida,"%s", linha);
				card++;
			}

			free(valB);
		}
		free(valA);
		rewind(arqB);
	}
	fclose(fsaida);
	fclose(arqA);
	fclose(arqB);	
	
	arq = geraNomeArq(saida,".ctl");
	fsaida = fopen(arq,"rt+");
	if(!fsaida)exit(1);
	free(arq);		
	fprintf(fsaida,"%d,%d", conta(atributos),card);
	
	fclose(fsaida);
	libera(atributos);
	libera(a);
	libera(b);
}

void projecao(char *relacao, char *n, char *lista, char *saida)
{
	//copiar catalogo para o saida e depois copiar as tuplas que interessam ao op e ao val
	char *aux;
	
	//ajusta o nome do arquvio que serÃ¡ aberto
	aux = geraNomeArq(relacao,".ctl");
	FILE *frelacao = fopen(aux,"rd");
	free(aux);
	if(!frelacao)exit(1);
	//ajusta o nome do arquvio que serÃ¡ aberto
	aux = geraNomeArq(saida,".ctl");
	FILE *fsaida = fopen(aux,"wt");
	free(aux);
	if(!fsaida)exit(1);	
	
	//escreve o novo grau e a cardinalidade(que nao se altera) na saida
	char linha[50],lin[50] ;
	fgets(linha, sizeof(linha), frelacao);
	char *grau_antigo = strtok(linha,",");
	char *cardin = strtok(NULL,",");
	strcpy(lin,n); // lin recebe o grau "novo" (apos a projecao)
	strcat(lin,",");
	strcat(lin,cardin);
	fprintf(fsaida,"%s", lin);
	
	//converte a lista de atributos em char para uma lista de atributos struct
	TAtr *atr = NULL;
	char *atributo = strtok(lista,",\n");
	
	while(atributo)
	{	
		atr = insere_fim(atr,atributo,-1);
		atributo = strtok(NULL,",");
	}
	
	int i = 0;
	//le o arquivo ctl para procurar pelas infos relevantes
	while(fgets(linha, sizeof(linha), frelacao))
	{
		char aux[20];
		strcpy(aux,linha);
		char *nomeAtr = strtok(aux,",");
		TAtr *auxAtr = busca(atr,nomeAtr);
		if(auxAtr) 
		{
			auxAtr->indice = i;
			fprintf(fsaida,"%s", linha);
		}
		i++;
	}

	//ajusta o nome do arquvio que serÃ¡ aberto
	aux = geraNomeArq(relacao,".dad");
	frelacao = fopen(aux,"rd");
	free(aux);
	if(!frelacao)exit(1);
	//ajusta o nome do arquvio que serÃ¡ aberto
	aux = geraNomeArq(saida,".dad");
	fsaida = fopen(aux,"wt");
	free(aux);
	if(!fsaida)exit(1);	
	
	//lista de atributos usada pra percorrer a string de dados que serão copiados
	TAtr *auxAtr = atr;

	//controlando a copia de virgulas
	int quantVal = 0;
	int grau = atoi(n);
	while(fgets(linha, sizeof(linha), frelacao))
	{
		i = 0;
		char *valor = strtok(linha,",");
		char linhaSaida[50];	
		while(auxAtr)
		{
			while(i < auxAtr->indice) 
			{
				i++;
				valor = strtok(NULL,","); 
			}
			quantVal++;
			strcat(linhaSaida,valor);
			valor = strtok(linhaSaida,"\n");
			if(quantVal != grau)
				strcat(linhaSaida,",");
			auxAtr = auxAtr->prox;
		}
		printf("Linha de saida: %s\n",valor);
		fprintf(fsaida,"%s\n",valor);
	}
	fclose(fsaida);
	fclose(frelacao);
	libera(atr);
}

void interpreta(char *inst)
{
	printf("%s",inst);
	return;
	//na primeira leitura, a variavel string vai conter a operação que será realizada
	char *strings = strtok(inst,"(,)");
	if(!strcmp(strings,"S"))
	{
		char *rel = strtok(NULL,"(,)");
		char *atributo = strtok(NULL,"(,)");
		char *operador = strtok(NULL,"(,)");
		char *valor = strtok(NULL,"(,)");
		char *saida = strtok(NULL,"(,)");
		selecao(rel,atributo,operador,valor,saida);
	}
	else if(!strcmp(strings,"J"))
	{
		char *relA = strtok(NULL,"(,)");
		char *relB = strtok(NULL,"(,)");
		char *comp = strtok(NULL,"(,)");
		char *saida = strtok(NULL,"(,)");
		juncao(relA,relB,comp,saida);
	}
	else if(!strcmp(strings,"P"))
	{
		char *relacao = strtok(NULL,"(,)");
		char *n = strtok(NULL,"(,)");
		
		//converte o numero de atributos para saber quantos atributos serão lidos
		int num = atoi(n);
		int i;
		
		char *lista_aux = strtok(NULL,",");
		char *lista = malloc(sizeof(char) * 50); //(André)é necessário uma lista auxiliar?(Alysson)acho q nao
		strcpy(lista,lista_aux);	
		for(i=0;i<num-1;i++){
			strcat(lista,strtok(NULL,"(,)"));
		}
		
		char *saida = strtok(NULL,"(,)");
		projecao(relacao,n,lista,saida);
		free(lista);
	}
	
	else 
	{
		printf("%s operação inválida",strings);	
		exit(1);
	}
}

