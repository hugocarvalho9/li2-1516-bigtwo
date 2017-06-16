#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estrutura.h"
#include "bots.h"
#include "estado.h"
#include "interface.h"
#include "jogo.h"

/**
Trata os argumentos da CGI.
Esta função recebe a query que é passada à cgi-bin e trata-a.
Neste momento, a query contém o estado que é um inteiro que representa um conjunto de cartas.
Cada carta corresponde a um bit que está a 1 se essa carta está no conjunto e a 0 caso contrário.
Caso não seja passado nada à cgi-bin, ela assume que todas as cartas estão presentes.
@param query A query que é passada à cgi-bin
 */
void parse(char *query) {
	ESTADO e = {{0},{0},0,0,0,0,0,0,0,0};

	if(query != NULL && strlen(query)!=0) {
		e = str2estado(query);
		if (e.selecionar) 
			e.selecionar = 0;
		if(e.jogar)
			e = jogar(e);
		if(e.passar)
			e = passar(e); 
		if(e.ajuda)
			e = ajuda(e);
	} else {
		e =  baralha(e);
		}
	
	if (e.ordenacao == 0) imprimeVAL(BARALHO, e);
	else imprimeNAI(BARALHO, e);

	imprime_jogar(e);
	imprime_passar(e);
	imprime_ordena(e);
	imprime_ajuda(e);
	imprime_info(e);
}

/**
Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai imprimir o código html para desenhar as cartas
 */
int main() {
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title>BIG 2</title></header>\n");
	printf("<body>\n");

	printf("<h1>BIG 2</h1>\n");
	printf("<svg height = \"800\" width = \"800\">\n");
	printf("<rect x = \"0\" y = \"0\" height = \"800\" width = \"800\" style = \"fill:#007700\"/>\n");

	parse(getenv("QUERY_STRING"));

	printf("</svg>\n");

	printf("</body>\n");
	return 0;
}