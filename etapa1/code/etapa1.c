#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
URL da CGI
*/
#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
/**
URL da pasta com as cartas
*/
#define BARALHO		"http://127.0.0.1/cards"
/**
Ordem dos naipes
*/
#define NAIPES		"DCHS"
/**
Ordem das cartas
*/
#define VALORES		"3456789TJQKA2"
/**
Formato
*/
#define FORMATO "%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%d_%d_%d_%lld_%d"

/**
	Estado inicial com todas as 52 cartas do baralho
	Cada carta é representada por um bit que está
	a 1 caso ela pertença à mão ou 0 caso contrário
*/
const long long int ESTADO_INICIAL = 0xfffffffffffff;

/**
Estrutura que representa um long long int com o nome "MAO"
*/
typedef long long int MAO;

/**
Estrututa que guarda as informações sobre o jogo
*/
typedef struct {
	MAO mao[4]; /* Array com as maos dos 4 jogadores */
	int cartas[4]; /* Array com o numero de cartas que cada jogador tem, um inteiro entre 0 e 13 */ 
	MAO selecao; /* Array para representar as cartas que estao selecionadas */
	int passar; /* 1 se passa */
	int jogar; /* 1 se joga */
	int selecionar; /* 1 se a carta é selecionada */
	MAO ultima_jogada; /* Guarda a mão que foi jogada na ultima jogada */
	int ultimo_jogador; /* Guarda o numero do ultimo jogador que jogou */
} ESTADO;


/**
Função que converte uma string num estado 
*/
ESTADO str2estado (char* str) {
	ESTADO e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.cartas[0], &e.cartas[1], &e.cartas[2],&e.cartas[3], &e.selecao, &e.passar, &e.selecionar, &e.jogar, &e.ultima_jogada, &e.ultimo_jogador);
	return e;
}

/**
Função que converte um estado numa string
*/
char* estado2str (ESTADO e){
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.cartas[0],e.cartas[1],e.cartas[2],e.cartas[3], e.selecao, e.passar, e.selecionar, e.jogar, e.ultima_jogada, e.ultimo_jogador);
	return res;
}

/** 
Devolve o índice da carta
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/
int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}

/**
Adiciona uma carta ao estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
long long int add_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO | ((long long int) 1 << idx);
}

/**
Remove uma carta do estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
long long int rem_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO & ~((long long int) 1 << idx);
}

/**
Verifica se uma carta pertence ao estado
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		1 se a carta existe e 0 caso contrário
*/
int carta_existe(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (ESTADO >> idx) & 1;
}

/**
Conta quantas cartas tem uma mão
*/
int conta_cartas(MAO h) {
	int n, v;
	int c = 0;

	for (n=0 ; n<4 ; n++) {
		for (v=0 ; v<13 ; v++)
			if (carta_existe(h, n, v)) c++;
	}

	return c;
}

/** 
Verifica se uma combinação de cartas é válida 
*/

int combinacao_valida (MAO selecao) {
	if (conta_cartas (selecao) > 3) return 0;
	return 1;
}

/**
Verifica se o tamanho entre duas mãos é igual 
*/

int compara_tamanho(MAO m1, MAO m2){
	if (conta_cartas(m1) == conta_cartas(m2)) {
		return 1;
	} 

	return 0;
}

/**
Dada uma mão, retorna o valor dessa mão
*/
int da_valor (MAO m) {
	int n, v;
	int primeira = -1;
	int e = 0;

	for (n=0; n<4 && e!=1 ; n++) 
		for (v=0; v<13 && e!=1 ; v++)
			if (carta_existe(m, n, v)) {
											primeira = v;
											e = 1;

			 							}
	

	for (n=0 ; n<4 ; n++) 
		for (v=0 ; v<13 ; v++)
			if (carta_existe(m, n, v))
				if (v!=primeira) return -1;
	
	return primeira;

}

/**
Dada uma mão, dá o maior naipe existente nessa mão 
*/
int da_maior_naipe (MAO m) {
	int n, v;
	int max=0;

	for (n=0 ; n<4 ; n++) 
		for (v=0 ; v<13 ; v++)
			if (carta_existe(m, n, v))
				if (n > max) max=n;

	return max;
}

/**
Verifica se a combinação de cartas selecionadas é maior do que a combinação de cartas da ultima jogada
*/
int combinacao_maior (MAO selecao, MAO ultima) {

	if (da_valor(selecao) != -1) {
									if (da_valor(selecao) > da_valor (ultima)) return 1;
									if (da_valor(selecao) < da_valor (ultima)) return 0;
									if (da_valor(selecao) == da_valor(ultima)) 
										if (da_maior_naipe(selecao) > da_maior_naipe(ultima)) return 1;
	}

	return 0;
}

/**
Verifica se estou em condições para poder jogar
*/
int posso_jogar (ESTADO e) {

	if (e.ultimo_jogador == 0) e.ultima_jogada = 0;


	if (e.ultima_jogada == 0) {
		if (!combinacao_valida(e.selecao)) return 0;
		if (da_valor(e.selecao) == -1) return 0;
		}
	else {
		if (!combinacao_valida(e.selecao)) return 0;
		if (!compara_tamanho(e.ultima_jogada, e.selecao)) return 0;
		if (!combinacao_maior (e.selecao, e.ultima_jogada)) return 0;
	}

	return 1;
}

/**
Função que calcula o score de uma mão no final do jogo
*/
int calcula_score (MAO m) {
	int r = 0;

	if (conta_cartas(m) == 13) r = 3*13;
	if (conta_cartas(m) <= 9 && conta_cartas(m) > 0) r = 1*(conta_cartas(m));
	if (conta_cartas(m) >= 10 && conta_cartas(m)<=12) r = 2*(conta_cartas(m));

	return r; 
}


/**
Imprime o html correspondente a uma carta
@param path	o URL correspondente à pasta que contém todas as cartas
@param x A coordenada x da carta
@param y A coordenada y da carta
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
*/
void imprime_carta(char *path, int x, int y, ESTADO e, int mao, int naipe, int valor) {
	char *suit = NAIPES;
	char *rank = VALORES;
	char script[10240];
	ESTADO new = e;
	if(mao==0) {
		if (carta_existe(new.selecao, naipe, valor))
			new.selecao = rem_carta(new.selecao, naipe, valor);
		else 
			new.selecao = add_carta(new.selecao, naipe, valor);

		new.selecionar = 1;

		sprintf(script, "%s?%s", SCRIPT, estado2str(new));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, path, rank[valor], suit[naipe]);
	}
	else {
		printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" />\n", x, y, path, rank[valor], suit[naipe]);
	}
}

/**
Função que imprime o botão "JOGAR"
*/
void imprime_jogar (ESTADO e) {
	char script[10240];

	if (posso_jogar (e)) {

		ESTADO new = e;
		new.jogar=1;

		sprintf(script, "%s?%s", SCRIPT, estado2str(new));
		printf("<a xlink:href = \"%s\"><image x = \"640\" y = \"630\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/submit2.png\" /></a>\n", script);
	}
	else 
		printf("<image x = \"640\" y = \"630\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/submit1.png\" /></a>\n");
}


/**
Função que imprime uma caixa com o score de cada jogador no final do jogo
*/
void imprime_vencedor (ESTADO e, int n_vencedor) {
	printf("<rect x = \"200\" y = \"200\" height = \"400\" width = \"400\" style = \"fill:#FFFFCC\"/>");
	printf("<text x =\"290\" y=\"265\" fill=\"black\" font-weight =\"bold\" font-size =\"30\">FIM DO JOGO</text>");
	printf("<text x =\"360\" y=\"310\" fill=\"black\" font-weight =\"bold\" font-size =\"20\">SCORE</text>"); 
	printf("<text x =\"345\" y=\"355\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">JOGADOR Nº1 : %d</text>", calcula_score(e.mao[0]));
	printf("<text x =\"345\" y=\"385\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">JOGADOR Nº2 : %d</text>", calcula_score(e.mao[1]));
	printf("<text x =\"345\" y=\"415\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">JOGADOR Nº3 : %d</text>", calcula_score(e.mao[2]));
	printf("<text x =\"345\" y=\"445\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">JOGADOR Nº4 : %d</text>", calcula_score(e.mao[3]));
	printf("<text x =\"290\" y=\"490\" text-align =\"center\" fill=\"black\" font-weight =\"bold\" font-size =\"15\">VENCEU O JOGADOR Nº %d !</text>", n_vencedor); 
	printf("<a xlink:href = \"%s\"><image x = \"345\" y = \"470\" height = \"150\" width = \"125\" xlink:href = \"http://127.0.0.1/botoes/novo.png\" /></a>\n", SCRIPT);
}

/**
FUnção que devolve o ESTADO depois de todos os bots jogarem ou passarem
*/
ESTADO joga_bot (ESTADO e) {
	int n, v;
	int j = 1;
	int X[3] = {205,345,500};
	int Y[3] = {350,200,350};

	do {
		int VALOR = da_valor(e.ultima_jogada);
		int NAIPE = da_maior_naipe(e.ultima_jogada);
		MAO aux = 0;
		int jogou = 0;
		int i;

		/* Caso o ultimo jogador a jogar tenha sido ele mesmo, pode jogar a combinação que quiser */
		if (e.ultimo_jogador == j) {
			for (v = 0; v<13 && jogou == 0 ; v++) 
				for (n = 0 ; n<4 && jogou == 0; n++) 
					if (carta_existe(e.mao[j],n,v)) {
						e.mao[j] = rem_carta (e.mao[j], n, v);
						imprime_carta (BARALHO, X[j-1], Y[j-1], e, j, n, v);
						e.ultima_jogada = add_carta(aux, n, v);
						e.ultimo_jogador = j;
						jogou = 1;
					}

			}
		
		/* caso não seja ele proprio o ultimo jogador a jogar, verifica se consegue jogar uma combinação maior (mas com o mesmo valor) que a anterior, com o mesmo numero de cartas que a combinação anterior */
		for (n = NAIPE ; n<4 && jogou == 0 ; n++) {
			int naipes[3] = {0};
			int valores[3] = {0};
			int c = 0;
			if (carta_existe(e.mao[j], n, VALOR)) { 
													naipes[c] = n;
													valores[c] = VALOR;
													c++;
													aux = add_carta(aux, n, VALOR);
													 }
			if (c == conta_cartas(e.ultima_jogada)) {
				for (i=0 ; i<c ; i++) {
					e.mao[j] = rem_carta (e.mao[j], naipes[i], valores[i]);
					imprime_carta (BARALHO, X[j-1], Y[j-1], e, j, naipes[i], valores[i]);
					X[j-1] += 20;	
					}
				e.ultima_jogada = aux;
				e.ultimo_jogador = j;
				jogou = 1;
			}
		}

		/* se falhou os dois casos anteriores, verifica se consegue jogar uma combinação maior (com valor maior) que a anterior, com o mesmo numero de cartas da combinação anterior */
		for (v = VALOR+1 ; v<13 && jogou == 0 ; v++) {
			int naipes[3] = {0};
			int valores[3] = {0};
			int c = 0;
			MAO aux = 0;
			for (n = 0 ; n<4 && jogou == 0 ; n++) {
				if (carta_existe(e.mao[j], n, v))   { 
														naipes[c] = n;
														valores[c] = v;
														c++;
														aux = add_carta (aux, n, v);
													 }

				if (c == conta_cartas(e.ultima_jogada)) {
					for (i=0 ; i<c ; i++) {
						e.mao[j] = rem_carta (e.mao[j], naipes[i], valores[i]);
						imprime_carta (BARALHO, X[j-1], Y[j-1], e, j, naipes[i], valores[i]);
						X[j-1] += 20;	
						}
					e.ultima_jogada = aux;
					e.ultimo_jogador = j;
					jogou = 1;
				}
			}
		}

		/* se o numero de cartas na sua mão é 0, é porque venceu, logo o jogo termina */ 
		if (conta_cartas(e.mao[j]) == 0) {  imprime_vencedor(e,j+1);
											j=4; 
										 }
		j++;

	} while (j<4);

	return e;
}


/**
Função que devolve o ESTADO depois de jogar
*/
ESTADO jogar (ESTADO e) {
	int n, v;
	int x = 325, y= 475;
	e.jogar=0;


		for(n = 0; n < 4; n++) 
			for(v = 0; v < 13; v++) 
				if(carta_existe(e.selecao, n, v)) {
					x+=20;
					e.mao[0] = rem_carta (e.mao[0],n,v);
					imprime_carta(BARALHO, x, y, e, 4, n,v);
				    }
			
		e.ultima_jogada = e.selecao;
		e.ultimo_jogador = 0;
		e.selecao = 0;

		if (conta_cartas(e.mao[0])) 
			e = joga_bot(e);
		else 
			imprime_vencedor(e,1);

	return e;
}

/**
Função que imprime o botão "PASS"
*/
void imprime_passar (ESTADO e) {
	char script[10240];
	ESTADO new = e;
	
	if (e.ultimo_jogador !=0 ) {
		new.passar=1;

		sprintf(script, "%s?%s", SCRIPT, estado2str(new));
		printf("<a xlink:href = \"%s\"><image x = \"640\" y = \"675\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/pass2.png\" /></a>\n", script);
		} 
	else 
		printf("<image x = \"640\" y = \"675\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/pass1.png\" /></a>\n");
		
}

/**
Função que devolve o ESTADO depois de clicar no botão "PASS"
*/
ESTADO passar (ESTADO e) {
	e.passar = 0;
	e.selecao = 0;
	e = joga_bot (e);
	return e;
}


/**
Imprime o estado
Esta função está a imprimir o estado em quatro colunas: uma para cada naipe
@param path	o URL correspondente à pasta que contém todas as cartas
@param ESTADO	O estado atual
*/
void imprime (char *path, ESTADO e) {
	int v, n;
	int m;

	int X[4] = {220,35,220,685};
	int Y[4] = {660,200,20,200};

	for (m=0 ; m<4; m++) {
		int x = X[m];
		int y = Y[m];
		for(v=0 ; v<13 ; v++) 
			for (n=0 ; n<4 ; n++) 
				if (carta_existe (e.mao[m], n, v)) {
					if (m%2 == 0) x+=20;
					else y+=20;

					if (m==0 && carta_existe(e.selecao, n, v))
						imprime_carta(path, x, y-20, e, m, n, v);
					else 
						imprime_carta(path, x, y, e, m, n, v);
					}			
		}
}
		
/**
Função que distribui as cartas baralhadas pelos 4 jogadores
*/
ESTADO baralha () {
	ESTADO e = {{0},{0},0,0,0,0,0,0};
	int v, n;
	int number;
	srand(time(NULL));

	for (v=0; v<13; v++) 
		for (n=0; n<4; n++) {
			do {
				number = random()%4;
			} while (e.cartas[number] ==13);
			e.mao[number] = add_carta(e.mao[number], n, v);
			e.cartas[number]++;
		}
	
	return e;

}

/*
Função que imprime o nº de cada jogador e o nº de cartas que tem
*/
void imprime_info (ESTADO e) {
	int X[4] = {110,20,110,660};
	int Y[4] = {705,185,60,185};
	int m;

	for (m=0 ; m<4 ; m++) {
		printf("<text x =\"%d\" y=\"%d\" fill=\"black\" font-weight =\"bold\" font-size =\"14\">JOGADOR Nº %d</text>", X[m],Y[m], m+1);
		printf("<text x =\"%d\" y=\"%d\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">Nº de cartas:  %d</text>", X[m],Y[m]+20, conta_cartas(e.mao[m]));
	}
}

/**
Trata os argumentos da CGI
Esta função recebe a query que é passada à cgi-bin e trata-a.
Neste momento, a query contém o estado que é um inteiro que representa um conjunto de cartas.
Cada carta corresponde a um bit que está a 1 se essa carta está no conjunto e a 0 caso contrário.
Caso não seja passado nada à cgi-bin, ela assume que todas as cartas estão presentes.
@param query A query que é passada à cgi-bin
 */
void parse(char *query) {
	ESTADO e;
	if(query != NULL && strlen(query)!=0) {
		e = str2estado(query);
		if (e.selecionar) 
			e.selecionar = 0;
		if(e.jogar)
			e = jogar(e);
		if(e.passar)
			e = passar(e); 
	} else {
		e =  baralha();
		}
	
	imprime(BARALHO, e);
	imprime_jogar(e);
	imprime_passar(e);
	imprime_info(e);
}


/**
Função principal
Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai imprimir o código html para desenhar as cartas
 */

int main() {

/*
 * Cabeçalhos necessários numa CGI
 */
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title>BIG 2</title></header>\n");
	printf("<body>\n");

	printf("<h1>BIG 2</h1>\n");
	printf("<svg height = \"800\" width = \"800\">\n");
	printf("<rect x = \"0\" y = \"0\" height = \"800\" width = \"800\" style = \"fill:#007700\"/>\n");



/*
 * Ler os valores passados à cgi que estão na variável ambiente e passá-los ao programa
 */
	parse(getenv("QUERY_STRING"));/* ,str2estado(e)) */

	printf("</svg>\n");

	printf("</body>\n");
	return 0;
}