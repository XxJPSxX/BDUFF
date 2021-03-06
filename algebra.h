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

char **strsplit(const char* str, const char* delim, size_t* numtokens) {
    // copy the original string so that we don't overwrite parts of it
    // (don't do this if you don't need to keep the old line,
    // as this is less efficient)
    char *s = strdup(str);
    // these three variables are part of a very common idiom to
    // implement a dynamically-growing array
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char*));
    char *token, *strtok_ctx;
    for (token = strtok_r(s, delim, &strtok_ctx);
            token != NULL;
            token = strtok_r(NULL, delim, &strtok_ctx)) {
        // check if we need to allocate more space for tokens
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char*));
        }
        tokens[tokens_used++] = strdup(token);
    }
    // cleanup
    if (tokens_used == 0) {
        free(tokens);
        tokens = NULL;
    } else {
        tokens = realloc(tokens, tokens_used * sizeof(char*));
    }
    *numtokens = tokens_used;
    free(s);
    return tokens;
}

void tiraQuebra(char *string)
{
	char *quebra = strchr(string,'\n');
	if(quebra)*quebra = 0;
}

int conta(TAtr *a);

TAtr *insere_fim(TAtr *l,char *atr, int i)
{
	TAtr *aux = l;
	TAtr *novo = (TAtr*) malloc(sizeof(TAtr));
	novo->prox = NULL;
	novo->indice = i;
	tiraQuebra(atr);
	novo->atr = malloc(strlen(atr));
	strcpy(novo->atr,atr);	
	novo->at = malloc(sizeof(char)*20);	
	
	size_t numtokens;
	char **tokens = strsplit(atr,",",&numtokens);

	strcpy(novo->at,tokens[0]);
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
		printf("%s/%d->",a->atr,a->indice);
		imprime(a->prox);
	}
}

TAtr *busca(TAtr *a, char *valor)
{
	while(1) 
	{
		if(a == NULL) return NULL;
		printf("\nValor:%s\n",a->at);
		if(strcmp(a->at,valor) == 0)
		{
			return a;
		}
		a = a->prox;
	}
}

char *geraNomeArq(char *rel, char *extensao)
{
	char *result = malloc(sizeof(char) * 20);
	strcpy(result,rel);
	strcat(result,extensao);
	return result;
}

int compara(char *op, char *val1, char *val2)
{	
	//tira o \n dos operadores
	tiraQuebra(val1);
	tiraQuebra(val2);
	//
	
	if(strcmp(op,"<") == 0)
	{
		return (strcmp(val1,val2) > 0);
	}
	
	if(strcmp(op,">") == 0)
	{
		int i = strcmp(val2,val1);
		return (strcmp(val2,val1) > 0);
		
	}
	if(strcmp(op,"=") == 0)
	{		
		int i = strcmp(val1,val2);
		if(i == 0) return 1;
		return 0;
	}
	if(strcmp(op,"<=") == 0)
	{
		if(strcmp(val1,val2) >= 0)
			return 1;
		return 0;
	}
	if(strcmp(op,">=") == 0)
	{
		if(strcmp(val1,val2) <= 0)
			return 1;
		return 0;
	}
	if(strcmp(op,"<>") == 0)
	{
		return strcmp(val1,val2);
	}
}

int comparaInt(char *op, char *val1, char *val2)
{	
	int a = atoi(val1),b = atoi(val2);
	if(strcmp(op,"<") == 0)
	{
		printf("%d < %d = %d",a,b,a<b);
		return a < b;
	}
	
	if(strcmp(op,">") == 0)
	{
		return a > b;
		
	}
	if(strcmp(op,"=") == 0)
	{		
		return a == b;
	}
	if(strcmp(op,"<=") == 0)
	{
		return a <= b;
	}
	if(strcmp(op,">=") == 0)
	{
		return  a >= b;
	}
	if(strcmp(op,"<>") == 0)
	{
		return a != b;
	}
	return 0;
}


void selecao(char *relacao, char *atr, char *op, char *val, char *saida)
{
	//copiar catalogo para o saida e depois copiar as tuplas que interessam ao op e ao val
	char *aux;
	//ajusta o nome do arquvio que será aberto
	aux = geraNomeArq(relacao,".ctl");
	FILE *frelacao = fopen(aux,"rt");
	free(aux);
	if(!frelacao)exit(1);
	//ajusta o nome do arquvio que será aberto
	aux = geraNomeArq(saida,".ctl");
	FILE *fsaida = fopen(aux,"wt");
	free(aux);
	if(!fsaida)exit(1);	
	int ehI = 0;
	
	//linha do arquivo que será lido
	char linha[200];
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
		if(!strcmp(atr,atrname))
		{
			atrib = i;
			atrname = strtok(NULL,",");
			if(!strcmp(atrname,"I"))ehI = 1;
		}
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

	char linha_aux[200];
	//guarda a cardinalidade da relação
	int cont = 0;
	while(fgets(linha, sizeof(linha), frelacao))
	{
		strcpy(linha_aux,linha);
		char *tkn = strtok(linha,",");
		//serve pra chegar no atributo que quero
		for(i = 0; i < atrib ;i++) tkn = strtok(NULL,","); 
		//se a comparar funcionar, escreve o arquivo a atualiza o contador
		if(ehI)
		{
			if(comparaInt(op,tkn,val)) 
			{	
				fprintf(fsaida,"%s", linha_aux);
				cont++;
			}
		}
		else
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
	char aux[strlen(linha)];
	strcpy(aux,linha);
	tiraQuebra(aux);
	
	char *tkn = strtok(aux,",");
	char *resp = malloc(sizeof(char) * 20);
	while(a && strcmp(atr,a->at))
	{
		tkn = strtok(NULL,",");
		a = a->prox;
	}
	strcpy(resp,tkn);
	return resp;
}

char *junta(char *linhaA, char *linhaB)
{
	strcat(linhaA,",");
	strcat(linhaA,linhaB);
	tiraQuebra(linhaA);
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

TAtr *junta_mudando_nomes(TAtr *a, TAtr *b,char *relA, char *relB)
{
	TAtr *resp = NULL;
	//quando a relação é igual
	if(!strcmp(relA,relB))
	{
		TAtr *aux = a;
		int i = 1;
		//variavel que rece a string do contador
		char cont[2];
		sprintf(cont, "%d", i);
		
		while(aux)
		{
			char atributo[sizeof(aux->atr) + 2];
			
			//separa so o nome do atributo
			char *nome = strtok(aux->atr,",");
			strcpy(atributo, nome);
			strcat(atributo, cont);
			strcat(atributo,",");
			//pega o resto da linha com o not null e ordenação
			nome = strtok(NULL,"");
			strcat(atributo,nome);
			//----------------------------
			strcpy(aux->atr,atributo);
			aux = aux->prox;
		}
		
		aux = b;
		i++;
		sprintf(cont, "%d", i);
		while(aux)
		{
			char atributo[sizeof(aux->atr) + 2];
			//separa so o nome do atributo
			char *nome = strtok(aux->atr,",");
			strcpy(atributo, nome);
			strcat(atributo, cont);
			strcat(atributo,",");
			//pega o resto da linha com o not null e ordenação
			nome = strtok(NULL,"");
			strcat(atributo,nome);
			//----------------------------
			strcpy(aux->atr,atributo);
			aux = aux->prox;
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

///J(A,B,condição-juncao,Z)
void juncao(char *relA, char *relB, char *con, char *saida)
{
	//separa os atributos da comparação
	char *tkn = strtok(con,"=");
	char atrA[100], atrB[100];
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
		
	char linha[200];
	int i = 0;
	//lista auxiliares para construção do arquivo
	TAtr *a = NULL, *b = NULL;
	
	//guarda na primeira linha do arquivo ctl os espaços para o grau e a cardinalidade
	fgets(linha, sizeof(linha), arqA);	
	fprintf(fsaida,"%s",linha);
	
	while(fgets(linha, sizeof(linha), arqA))
	{
		if(strcmp(linha,"\n"))
			a = insere_fim(a,linha,i);
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
		if(strcmp(linha,"\n"))
			b = insere_fim(b,linha,i);
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
	
	char linhaA[150], linhaB[150];
	int card = 0;
	
	while(fgets(linhaA, sizeof(linhaA), arqA))
	{
		tiraQuebra(linhaA);
		char *valA = pegaVal(a,atrA,linhaA);
		while(fgets(linhaB, sizeof(linhaB), arqB))
		{
			tiraQuebra(linhaB);
			char *valB = pegaVal(b,atrB,linhaB);
			if(compara("=",valA,valB))
			{
				//variavel criada para guarda o valor da linhaA, a qual so é lida uma vez e estava sendo alterada indevidamente sem o uso deste
				char aux_junta[300];
				strcpy(aux_junta,linhaA);

				char *linha = junta(aux_junta,linhaB);
				fprintf(fsaida,"%s\n", linha);
				printf("Linha:%s\n",linha);
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
	
	libera(a);
	libera(b);
	fclose(fsaida);
	libera(atributos);
}

void projecao(char *relacao, char *n, char *lista, char *saida)
{
	//copiar catalogo para o saida e depois copiar as tuplas que interessam ao op e ao val
	char *aux;
	
	//ajusta o nome do arquvio que serÃ¡ aberto
	aux = geraNomeArq(relacao,".ctl");
	FILE *frelacao = fopen(aux,"rt");
	free(aux);
	if(!frelacao)exit(1);
	
	//ajusta o nome do arquvio que serÃ¡ aberto
	aux = geraNomeArq(saida,".ctl");
	FILE *fsaida = fopen(aux,"wt");
	free(aux);
	if(!fsaida)exit(1);	
	
	//escreve o novo grau e a cardinalidade(que nao se altera) na saida
	char linha[200],lin[200] ;
	fgets(linha, sizeof(linha), frelacao);
	char *grau_antigo = strtok(linha,",");
	char *cardin = strtok(NULL,",");
	strcpy(lin,n); // lin recebe o grau "novo" (apos a projecao)
	strcat(lin,",");
	strcat(lin,cardin);
	fprintf(fsaida,"%s", lin);
	
	//converte a lista de atributos em char para uma lista de atributos struct
	char copia[strlen(lista)];
	strcpy(copia,lista);

	char *atributo = strtok(copia,",");
	TAtr *atr = NULL;
	
	while(atributo)
	{	
		atr = insere_fim(atr,atributo,0);
		atributo = strtok(NULL,",");
	}
	
	int i = 0;
	//le o arquivo ctl para procurar pelas infos relevantes
	while(fgets(linha, sizeof(linha), frelacao))
	{
		printf("%d",i);
		char aux[20];
		strcpy(aux,linha);
		if(strcmp(aux,"\n"))
		{
			char *nomeAtr = strtok(aux,",");
			TAtr *auxAtr = busca(atr,nomeAtr);
			if(auxAtr) 
			{
				auxAtr->indice = i;
				fprintf(fsaida,"%s", linha);
			}
			i++;
		}
	}
	fclose(fsaida);
	fclose(frelacao);
	
	//ajusta o nome do arquvio que serÃ¡ aberto
	aux = geraNomeArq(relacao,".dad");
	frelacao = fopen(aux,"rt");
	free(aux);
	if(!frelacao)exit(1);
	//ajusta o nome do arquvio que serÃ¡ aberto
	aux = geraNomeArq(saida,".dad");
	fsaida = fopen(aux,"wt");
	free(aux);
	if(!fsaida)exit(1);	
	
	//lista de atributos usada pra percorrer a string de dados que serão copiados
	TAtr *auxAtr = NULL;

	//controlando a copia de virgulas
	int quantVal = 0;
	int grau = atoi(n);
	auxAtr = atr;
	imprime(atr);
	while(fgets(linha, sizeof(linha), frelacao))
	{
		i = 0;
		char *valor = strtok(linha,",");
		char linhaSaida[200];	
		strcpy(linhaSaida,"");
		while(auxAtr)
		{
			//dentro da linha lida, procura pelo valor esperado
			while(valor && i < auxAtr->indice) 
			{
				i++;
				valor = strtok(NULL,","); 
			}
			if(valor)
			{
				quantVal++;
				strcat(linhaSaida,valor);
				if(quantVal != grau)
					strcat(linhaSaida,",");
			}
			i = 0;
			valor = strtok(linha,",");
			auxAtr = auxAtr->prox;
		}
		tiraQuebra(linhaSaida);
		fprintf(fsaida,"%s\n",linhaSaida);
		auxAtr = atr;
		quantVal = 0;
	}
	fclose(fsaida);
	fclose(frelacao);
	libera(atr);
}

void interpreta(char *inst)
{	
	//na primeira leitura, a variavel string vai conter a operação que será realizada
	char *strings = strtok(inst,"(,)");
	if(!strcmp(strings,"S"))
	{
		char rel[15];
		strcpy(rel,strtok(NULL,"(,)"));
		char atributo[15];
		strcpy(atributo,strtok(NULL,"(,)"));
		char operador[2];
		strcpy(operador,strtok(NULL,"(,)"));
		char val[50];
		strcpy(val,strtok(NULL,"(,)"));
		char saida[30];
		strcpy(saida,strtok(NULL,"(,)"));
		selecao(rel,atributo,operador,val,saida);
	}
	else if(!strcmp(strings,"J"))
	{
		char relA[50];
		strcpy(relA,strtok(NULL,"(,)"));
		char relB[50];
		strcpy(relB,strtok(NULL,"(,)"));
		char comp[50];
		strcpy(comp,strtok(NULL,"(,)"));
		char saida[50];
		strcpy(saida,strtok(NULL,"(,)"));
		juncao(relA,relB,comp,saida);
	}
	else if(!strcmp(strings,"P"))
	{
		char relacao[50];
		strcpy(relacao,strtok(NULL,"(,)"));
		
		char n[5];
		strcpy(n,strtok(NULL,"(,)"));
		//converte o numero de atributos para saber quantos atributos serão lidos
		int num = atoi(n);
		int i;
		
		char *lista_aux = strtok(NULL,",");
		char *lista = malloc(sizeof(char) * 50); //(André)é necessário uma lista auxiliar?(Alysson)acho q nao
		strcpy(lista,lista_aux);	
		for(i=1;i<num;i++){
			strcat(lista,",");
			strcat(lista,strtok(NULL,"(,)"));
		}
		char saida[50];
		strcpy(saida,strtok(NULL,"(,)"));
		projecao(relacao,n,lista,saida);
		free(lista);
	}
	
	else 
	{
		printf("%s operação inválida",strings);	
		exit(1);
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

void imprimeTabela(char *tabela)
{
	char *nomeArq = geraNomeArq(tabela,".ctl");
	FILE *ctl = fopen(nomeArq,"rt");
	if(!ctl)exit(1);
	
	char linha[100];
	//tira o cabeçalho
	fgets(linha, sizeof(linha), ctl);
	while(fgets(linha, sizeof(linha), ctl))
	{
		char *print = strtok(linha,",");
		printf("%s\t\t",print);
	}
	printf("\n");
	fclose(ctl);
	
	nomeArq = geraNomeArq(tabela,".dad");
	FILE *dad = fopen(nomeArq,"rt");
	if(!dad)exit(1);
	
	while(fgets(linha, sizeof(linha), dad))
	{
		tiraQuebra(linha);
		char *print = strtok(linha,",");
		while(print)
		{
			printf("%s\t\t",print);
			print = strtok(NULL,",");
		}
		printf("\n");
	}
}
