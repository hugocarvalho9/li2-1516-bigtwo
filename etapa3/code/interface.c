#include <stdio.h>

#include "estrutura.h"
#include "jogo.h"
#include "estado.h"
#include "bots.h"
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
Função que imprime o botão "JOGAR". Dependendo da possibilidade de jogar, existem 2 tipos de botões que podem aparecer.
Um deles é válido, isto é permite que seja usado para jogar, enquanto o outro não, pois não é possivel jogar. 
@param MAO	Recebe um estado.
*/
void imprime_jogar (ESTADO e) {
	char script[10240];

	if (posso_jogar (e)) {

		ESTADO new = e;
		new.jogar=1;

		sprintf(script, "%s?%s", SCRIPT, estado2str(new));
		printf("<a xlink:href = \"%s\"><image x = \"640\" y = \"615\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/submit2.png\" /></a>\n", script);
	}
	else 
		printf("<image x = \"640\" y = \"615\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/submit1.png\" /></a>\n");
}
/**
Função que imprime uma caixa com o score de cada jogador no final do jogo e diz o número do jogador que ganhou.
@param ESTADO	Recebe o estado.
@param int	Recebe o número do vencedor.
*/
void imprime_vencedor (ESTADO e, int n_vencedor) {
	printf("<rect x = \"200\" y = \"200\" height = \"400\" width = \"400\" style = \"fill:#FFFFCC\"/>");
	printf("<text x =\"290\" y=\"265\" fill=\"black\" font-weight =\"bold\" font-size =\"30\">FIM DO JOGO</text>");
	printf("<text x =\"360\" y=\"310\" fill=\"black\" font-weight =\"bold\" font-size =\"20\">SCORE</text>"); 
	printf("<text x =\"345\" y=\"355\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">JOGADOR Nº1 : %d</text>", -1 * calcula_score(e.mao[0]));
	printf("<text x =\"345\" y=\"385\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">JOGADOR Nº2 : %d</text>", -1 * calcula_score(e.mao[1]));
	printf("<text x =\"345\" y=\"415\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">JOGADOR Nº3 : %d</text>", -1 * calcula_score(e.mao[2]));
	printf("<text x =\"345\" y=\"445\" fill=\"black\" font-weight =\"bold\" font-size =\"12\">JOGADOR Nº4 : %d</text>", -1 * calcula_score(e.mao[3]));
	printf("<text x =\"290\" y=\"490\" text-align =\"center\" fill=\"black\" font-weight =\"bold\" font-size =\"15\">VENCEU O JOGADOR Nº %d !</text>", n_vencedor); 
	printf("<a xlink:href = \"%s\"><image x = \"345\" y = \"470\" height = \"150\" width = \"125\" xlink:href = \"http://127.0.0.1/botoes/novo.png\" /></a>\n", SCRIPT);
}
/**
Função que imprime o botão "PASS", tal como a imprime_jogar, esta função também imprime 2 tipos de botões. Um que deixa passar a jogada caso
o ultimo jogador seja diferente de zero e o outro nada faz. Existe um diferença na cor para os distinguir.
@param ESTADO Recebe o ESTADO atual.

*/
void imprime_passar (ESTADO e) {
	char script[10240];
	ESTADO new = e;
	
	if (e.ultimo_jogador !=0 ) {
		new.passar=1;

		sprintf(script, "%s?%s", SCRIPT, estado2str(new));
		printf("<a xlink:href = \"%s\"><image x = \"640\" y = \"655\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/pass2.png\" /></a>\n", script);
		} 
	else 
		printf("<image x = \"640\" y = \"655\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/pass1.png\" /></a>\n");	
}
/**
Função que imprime o botão de ajuda.
@param ESTADO Recebe o ESTADO atual
*/
void imprime_ajuda (ESTADO e) {
	char script[10240];
	ESTADO new = e;

	new.ajuda=1;

	sprintf(script, "%s?%s", SCRIPT, estado2str(new));
	printf("<a xlink:href = \"%s\"><image x = \"640\" y = \"695\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/ajuda.png\" /></a>\n", script);
}
/**
Função que imprime o botão "ORDENA". Dependendo do tipo de ordenação 0 ou 1 vai imprimir um tipo de botão.
@param ESTADO Recebe o ESTADO atual.
*/
void imprime_ordena	(ESTADO e) {
	char script[10240];
	ESTADO new = e;
	
	if (e.ordenacao == 0) {
		new.ordenacao=1;

		sprintf(script, "%s?%s", SCRIPT, estado2str(new));
		printf("<a xlink:href = \"%s\"><image x = \"65\" y = \"695\" height = \"30\" width = \"30\" xlink:href = \"http://127.0.0.1/botoes/ord_naipe.png\" /></a>\n", script);
		} 
	else {
		new.ordenacao=0;

		sprintf(script, "%s?%s", SCRIPT, estado2str(new));
		printf("<a xlink:href = \"%s\"><image x = \"65\" y = \"695\" height = \"30\" width = \"30\" xlink:href = \"http://127.0.0.1/botoes/ord_valor.png\" /></a>\n", script);
		} 	
}
/**
Imprime o estado em que as cartas estão baralhadas por valor.
Esta função está a imprimir o estado em quatro colunas: uma para cada naipe.
@param path	o URL correspondente à pasta que contém todas as cartas.
@param ESTADO	O estado atual.
*/
void imprimeVAL (char *path, ESTADO e) {
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
Imprime o estado em que as cartas estão baralhadas por naipe.
Esta função está a imprimir o estado em quatro colunas: uma para cada naipe.
@param path	o URL correspondente à pasta que contém todas as cartas.
@param ESTADO	O estado atual.
*/
void imprimeNAI (char *path, ESTADO e) {
	int v, n;
	int m;

	int X[4] = {220,35,220,685};
	int Y[4] = {660,200,20,200};

	for (m=0 ; m<4; m++) {
		int x = X[m];
		int y = Y[m];
		for(n=0 ; n<4 ; n++) 
			for (v=0 ; v<13 ; v++) 
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
/*
Função que imprime o nº de cada jogador e o nº de cartas que tem.
@param ESTADO Recebe o ESTADO atual.
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
Função que devolve o ESTADO depois de jogar.
@param ESTADO Recebe o ESTADO atual.
@return ESTADO Devolve o ESTADO.
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

	if (conta_cartas(e.mao[0])) e = joga_bot(e);
	else imprime_vencedor(e,1);

	return e;
}

/**
Função que devolve o ESTADO depois de clicar no botão "PASS".
@param ESTADO Recebe o ESTADO atual.
@return ESTADO Devolve o ESTADO depois de passar.
*/
ESTADO passar (ESTADO e) {
	e.passar = 0;
	e.selecao = 0;
	e = joga_bot (e);
	return e;
}
