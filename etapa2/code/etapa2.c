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
#define FORMATO "%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%d_%d_%d_%lld_%d_%d_%d"

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
	int ordenacao; /*0 se por valor, 1 se por naipe */
	int ajuda;
} ESTADO;


/**
Função que converte uma string num estado 
*/
ESTADO str2estado (char* str) {
	ESTADO e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.cartas[0], &e.cartas[1], &e.cartas[2],&e.cartas[3], &e.selecao, &e.passar, &e.selecionar, &e.jogar, &e.ultima_jogada, &e.ultimo_jogador, &e.ordenacao, &e.ajuda);
	return e;
}

/**
Função que converte um estado numa string
*/
char* estado2str (ESTADO e){
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.cartas[0],e.cartas[1],e.cartas[2],e.cartas[3], e.selecao, e.passar, e.selecionar, e.jogar, e.ultima_jogada, e.ultimo_jogador, e.ordenacao, e.ajuda);
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

/****************************************************************SRAIGHT************************************************************/
int isFlush (MAO m);
int isFullHouse(MAO m);
int is4OfAKind(MAO m);


int maior_valorStraight (MAO e) {
   	int n, v;
   	int i;
    int aux[14]={0};

	if (isFullHouse(e) || is4OfAKind(e)) return -1;

	for (v=0, i=2; v < 13; v++) {
        for(n = 0; n < 4; n++) {
            switch (v) {
                case 11: if (carta_existe(e,n,v)) { aux[0]++; aux[13]++; } break;
                case 12: if (carta_existe(e,n,v)) { aux[1]++; } break;
                default: if (carta_existe(e,n,v)) { aux[i]++; } break;
            }
        }
        i++;
    }
    
    i=13;
    while ((i-4) >= 0) {
        if ((aux[i] != 0) && (aux[i-1] != 0) && (aux[i-2] != 0) && (aux[i-3] != 0) && (aux[i-4] != 0)) return (i-2);
        i--;
    }
	
	return -1;
} 

int da_maior_naipe_straight (MAO m) {
	int n, v;
	int max=-1;

	int max_valor = maior_valorStraight(m);
	for (v=0; v<=max_valor ; v++) 
		for (n=0; n<4; n++)
			if (carta_existe(m,n,v))
				max=n;
	
	return max;
}

int isStraight (MAO m) {
    int v,i,n;
    int aux[14]={0};

    for (v=0, i=2; v < 13; v++) {
        for(n = 0; n < 4; n++) {
            switch (v) {
                case 11: if (carta_existe(m,n,v)) { aux[0]++; aux[13]++; } break;
                case 12: if (carta_existe(m,n,v)) { aux[1]++; } break;
                default: if (carta_existe(m,n,v)) { aux[i]++; } break;
            }
        }
        i++;
    }
    
    i=0;
    while ((i+4) < 14) {
        if ((aux[i] != 0) && (aux[i+1] != 0) && (aux[i+2] != 0) && (aux[i+3] != 0) && (aux[i+4] != 0)) return 1;
        i++;
    }
    
    return 0;
}


/****************************************************************FLUSH*************************************************************/
int da_valor_flush(MAO e) {
	int r = -1;
	int n, v;

	for (n=0; n<4 ; n++) {
		for (v=0 ; v<13 ; v++) 
			if (carta_existe(e,n,v)) r = v;

		if (r!=-1) return r;
	}

	return r;
}

int isFlush (MAO selecao) {
	int n, v;
	int ouros = 0;
	int paus = 0;
	int copas = 0;
	int espadas = 0;

    for (v=0, n=0; v<13; v++) { if(carta_existe(selecao, n, v)) ouros++; }
	for (v=0, n=1; v<13; v++) { if(carta_existe(selecao, n, v)) paus++; }
	for (v=0, n=2; v<13; v++) { if(carta_existe(selecao, n, v)) copas++; }
	for (v=0, n=3; v<13; v++) { if(carta_existe(selecao, n, v)) espadas++; }

	if (ouros==5 && paus==0 && copas==0 && espadas==0 ) return 1;
	if (ouros==0 && paus==5 && copas==0 && espadas==0 ) return 1;
	if (ouros==0 && paus==0 && copas==5 && espadas==0 ) return 1;
	if (ouros==0 && paus==0 && copas==0 && espadas==5 ) return 1;

	return 0;
}

/****************************************************************FULL HOUSE********************************************************/
int maior_trio_fullhouse (MAO m) {
    int v,n;
    int aux[14] = {0};
    int r = -1;

    for (v = 0; v < 13; v++) 
        for(n = 0; n < 4; n++) 
            if (carta_existe(m,n,v)) aux[v]++;   
    
    
    for (v = 0; v < 13; v++) {
        if (aux[v] == 3)
            r = v;
     
    }
    
    return r;
}


int isFullHouse (MAO m) {
	int v, n;
	int tem3cartas = 0;
	int tem2cartas = 0;
	int c=0;


	for (v=0 ; v<13 ; v++) {
		for (n=0 ; n<4 ; n++) 
			if(carta_existe(m,n,v)) c++;


		if (c == 3) tem3cartas = 1;
		if (c == 2) tem2cartas = 1;

		c=0;

	}

	return (conta_cartas(m)==5 && tem2cartas && tem3cartas);
}
/****************************************************************POKER*************************************************************/
int valor_4OfKind (MAO m) {
	int v, n;
	int c = 0;

	for (v=0; v<13 ; v++) {
		for (n=0 ; n<4 ; n++) 
			if (carta_existe(m,n,v)) c++;

		if (c==4) return v;
		c=0;
	}

	return -1;
}

int is4OfAKind (MAO m) {
	int v, n;
	int c = 0;
	int tem4cartas = 0;

	for (v=0; v<13; v++) {
		for (n=0; n<4 ; n++)
			if(carta_existe(m,n,v)) c++;

		if (c==4) tem4cartas = 1;
		c=0;
	}

	return (conta_cartas(m)==5 && tem4cartas);
}
/***************************************************************Straight_Flush*******************************************************/
int isStraightFlush (MAO m) {
	return (isStraight(m) && isFlush(m));
}


int primeiro_jogador (ESTADO e) {
    int i;
    for (i = 0; i < 4; i++) 
        if (carta_existe(e.mao[i], 0, 0)) return i;
 
    return -1;
}

int ranking (MAO m) {
	if (isStraightFlush(m)) return 5;
	if (isStraight(m)) return 1;
	if (isFlush(m)) return 2;
	if (isFullHouse(m)) return 3;
	if (is4OfAKind(m)) return 4;


	return 0;
}

/** 
Verifica se uma combinação de cartas é válida 
*/

int combinacao_valida (MAO selecao) {
	if (conta_cartas (selecao) == 4) return 0;
	if (conta_cartas (selecao) > 5) return 0;
	if (conta_cartas (selecao) == 5) {
		if (isStraight(selecao)) return 1;
		if (isFlush(selecao)) return 1;
		if (isFullHouse(selecao)) return 1; 
		if (is4OfAKind(selecao)) return 1;
		return 0;
	}

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

	int max=-1;

	if (isFullHouse(m) || is4OfAKind(m)) return max;

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

	if (conta_cartas(ultima) < 4) {
		if (da_valor(selecao) != -1) {
										if (da_valor(selecao) > da_valor (ultima)) return 1;
										if (da_valor(selecao) < da_valor (ultima)) return 0;
										if (da_valor(selecao) == da_valor(ultima)) 
											if (da_maior_naipe(selecao) > da_maior_naipe(ultima)) return 1;
		} 
	}


	if (conta_cartas(ultima) == 5) {    if (ranking(ultima) == 1) {
											if (ranking(selecao) > 1) return 1;
											if (ranking(selecao) < 1) return 0;
		                                    if (ranking(selecao == 1)) {
		                                        if (maior_valorStraight(ultima) < maior_valorStraight(selecao)) return 1;
		                                        if (maior_valorStraight(ultima) > maior_valorStraight(selecao)) return 0;
		                                        if (maior_valorStraight(ultima) == maior_valorStraight(selecao)) {
		                                            if (da_maior_naipe_straight(selecao) > da_maior_naipe_straight(ultima)) return 1;
		                                            else return 0; 
		                                            }
		                                 		}
		                                 }



		                                 if (ranking(ultima) == 2) {
		                                 	if (ranking(selecao) > 2) return 1;
		                                 	if (ranking(selecao) < 2) return 0;
		                                 	if (ranking(selecao) == 2) {
		                                 		if (da_maior_naipe(selecao) > da_maior_naipe(ultima)) return 1;
									    		if (da_maior_naipe(selecao) < da_maior_naipe(ultima)) return 0;
									    		if (da_maior_naipe(selecao) == da_maior_naipe(ultima)) {
									    			if (da_valor_flush(selecao) > da_valor_flush(ultima)) return 1;
									    			else return 0;
									    		     }
		                                 	    }
		                                 }

		                                 if (ranking(ultima) == 3) {
		                                 	if (ranking(selecao) > 3) return 1;
		                                 	if (ranking(selecao) < 3) return 0;
		                                 	if (ranking(selecao) == 3) {
		                                 		if (maior_trio_fullhouse(selecao) > maior_trio_fullhouse(ultima)) return 1;
									        	else return 0;
		                                 	    }
		                                 }

										if (ranking(ultima) == 4) {
		                                 	if (ranking(selecao) > 4) return 1;
		                                 	if (ranking(selecao) < 4) return 0;
		                                 	if (ranking(selecao) == 4) {
		                                 		if (valor_4OfKind(selecao) > valor_4OfKind(ultima)) return 1;
									        	else return 0;
		                                 	    }
		                                 }


										if (ranking(ultima) == 5) {
											if (ranking(selecao) > 5) return 1;
											if (ranking(selecao) < 5) return 0;
		                                    if (ranking(selecao == 5)) {
									    		if (da_maior_naipe(selecao) > da_maior_naipe (ultima)) return 1;
									    		if (da_maior_naipe(selecao) < da_maior_naipe (ultima)) return 0;
									    		if (da_maior_naipe(selecao) == da_maior_naipe (ultima)) {
									    			if (maior_valorStraight(selecao) > maior_valorStraight(ultima)) return 1;
									    			else return 0;
									    	}
		                                 }
		                             }
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
		if (conta_cartas(e.selecao)<4) if (da_valor(e.selecao) == -1) return 0;
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
		printf("<a xlink:href = \"%s\"><image x = \"640\" y = \"615\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/submit2.png\" /></a>\n", script);
	}
	else 
		printf("<image x = \"640\" y = \"615\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/submit1.png\" /></a>\n");
}

/**
Função que imprime uma caixa com o score de cada jogador no final do jogo
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


ESTADO joga_primeira (ESTADO e, int j, int posX, int posY) {
	int n, v;
	int jogou = 0;
	MAO aux=0;

	for (v = 0; v<13 && jogou == 0 ; v++) 
				for (n = 0 ; n<4 && jogou == 0; n++) 
					if (carta_existe(e.mao[j],n,v)) {
						e.mao[j] = rem_carta (e.mao[j], n, v);
						imprime_carta (BARALHO, posX, posY, e, j, n, v);
						e.ultima_jogada = add_carta(aux, n, v);
						e.ultimo_jogador = j;
						jogou = 1;
					}

	return e;
} 

ESTADO joga_comb_maior_naipe (ESTADO e, int j, int posX, int posY) {
	  int n = da_maior_naipe(e.ultima_jogada);
	  int valor_ultima = da_valor(e.ultima_jogada);
	  int jogou = 0;
	  MAO aux=0;
	  int c = 0;
	  int i;
	  int naipes[3] = {0};
	  int valores[3] = {0};

	  for ( ; n<4 && jogou == 0 ; n++) {
			for(i=0; i<3; i++) { naipes[i] = 0; valores[i] = 0; }
			c = 0;
			
			if (carta_existe(e.mao[j], n, valor_ultima)) { 
				naipes[c] = n;
				valores[c] = valor_ultima;
				c++;
			    aux = add_carta(aux, n, valor_ultima);
		     }

			if (c == conta_cartas(e.ultima_jogada)) {
				for (i=0 ; i<c ; i++) {
					e.mao[j] = rem_carta (e.mao[j], naipes[i], valores[i]);
					imprime_carta (BARALHO, posX, posY, e, j, naipes[i], valores[i]);
					posX += 20;	
				}
						
				e.ultima_jogada = aux;
				e.ultimo_jogador = j;
				jogou = 1;
			}
				
      }
	
	return e;
} 
	

ESTADO joga_comb_maior_valor (ESTADO e, int j, int posX, int posY) {
	int v = da_valor(e.ultima_jogada);
	int n, i;
	int c = 0;
	int jogou = 0;
	int naipes[3] = {0};
	int valores[3] = {0};
	MAO aux;

	for (v = v+1 ; v<13 && jogou == 0 ; v++) {
		for(i=0; i<3; i++) {naipes[i]=0; valores[i]=0; }
		c = 0;
		aux = 0;
		
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
					imprime_carta (BARALHO, posX, posY, e, j, naipes[i], valores[i]);
					posX += 20;	
				}

			e.ultima_jogada = aux;
			e.ultimo_jogador = j;
			jogou = 1;
			}
		}
	 }

	return e;	
}

ESTADO joga_straight_igual (ESTADO e, int j, int posX, int posY) {
	int naipes[5] = {0};
	int valores[5] = {0};
	int max_valor = maior_valorStraight(e.ultima_jogada);
	int c = 0;
	MAO aux = 0;
	int n, v, i;
	int jogou = 0;
	int flag = 0;

	if ((max_valor-4)<0) {
		if ((max_valor-4)==-1) v=12;
		else v=11;
		for(; v<13 ; v++)
			for(n=0,flag=0; n<4 && flag == 0; n++)
				if (carta_existe(e.mao[j], n, v)) {
					naipes[c]=n;
					valores[c]=v;
					c++;
					aux=add_carta(aux,n,v);
					flag++;
				}
	}

	if ((max_valor-4)<0) v=0;
	else v = max_valor-4; 

	for (; v<=max_valor && jogou == 0; v++) {
		flag = 0; 
     	for(n=0 ; n<4 && flag == 0 ; n++) 
			if (carta_existe(e.mao[j],n,v)) {
					naipes[c] = n;
					valores[c] = v;
					c++;
					aux = add_carta(aux, n, v);
					flag++;
				}

		if (flag == 0) return e;

		else if (c==conta_cartas(e.ultima_jogada)) {
			    if (da_maior_naipe_straight(aux)<da_maior_naipe_straight(e.ultima_jogada)) return e;
				for (i=0; i<c; i++) {
					e.mao[j] = rem_carta(e.mao[j], naipes[i], valores[i]);
					imprime_carta (BARALHO, posX, posY, e, j, naipes[i], valores[i]);
					posX += 20;
				    }

		    	e.ultima_jogada = aux;
				e.ultimo_jogador = j;
				jogou = 1;
		        } 
	}

	return e;
}


ESTADO joga_straight (ESTADO e, int j, int posX, int posY) {
	int naipes[5] = {0};
	int valores[5] = {0};
	int max_valor = maior_valorStraight(e.ultima_jogada);
	int c = 0;
	MAO aux = 0;
	int n, v, i;
	int jogou = 0;
	int flag = 0;

	e = joga_straight_igual(e,j,posX,posY); 

	if (e.ultimo_jogador!=j) {

	if ((max_valor-3)<0) {
		v=12;
		for(n=0,flag=0; n<4 && flag == 0; n++)
			if (carta_existe(e.mao[j], n, v)) {
				naipes[c]=n;
				valores[c]=v;
				c++;
				aux=add_carta(aux,n,v);
				flag++;
				}
	}

	if((max_valor-3)<0) v=0;
	else v = max_valor-3;

	for ( ; v<12 && jogou == 0; v++) {
		flag = 0; 
     	for(n=0 ; n<4 && flag == 0 ; n++) 
			if (carta_existe(e.mao[j],n,v)) {
					naipes[c] = n;
					valores[c] = v;
					c++;
					aux = add_carta(aux, n, v);
					flag++;
				}

		if (flag == 0) {
	        for(i=0; i<5 ; i++) { naipes[i] = 0; valores[i] = 0; }
		    c = 0;
		    aux = 0;
		}

		else if (c==conta_cartas(e.ultima_jogada)) {
				for (i=0; i<c; i++) {
					e.mao[j] = rem_carta(e.mao[j], naipes[i], valores[i]);
					imprime_carta (BARALHO, posX, posY, e, j, naipes[i], valores[i]);
					posX += 20;
				    }

		    	e.ultima_jogada = aux;
				e.ultimo_jogador = j;
				jogou = 1;
		        } 
	}
}
	return e;
}

ESTADO joga_flush_igual_naipe(ESTADO e, int j, int posX, int posY) {
	int valores[5] = {0};
	int naipe = -1;
	int jogou = 0;
	int n, v, i, c;
	MAO aux = 0;


	n = da_maior_naipe(e.ultima_jogada);
	

	
	for(i=0; i<5; i++) { valores[i] = 0 ; naipe = -1; }
		c = 0;
		aux = 0;

	for (v=0; v<13 && jogou == 0 ; v++) {	
		if (carta_existe(e.mao[j], n, v)) {
            naipe = n;
            valores[c] = v;
			c++;
			aux = add_carta (aux, n, v);
		    }

			if (c==conta_cartas(e.ultima_jogada)) {
				if (da_valor_flush(aux)<da_valor_flush(e.ultima_jogada)) return e;
				for (i=0; i<c; i++) {
					e.mao[j] = rem_carta(e.mao[j], naipe, valores[i]);
					imprime_carta (BARALHO, posX, posY, e, j, naipe, valores[i]);
					posX += 20;
				}

				e.ultima_jogada = aux;
				e.ultimo_jogador = j;
				jogou = 1;
			}
	      
	} 

	return e;
	     
}


ESTADO joga_flush(ESTADO e, int j, int posX, int posY) {
	int valores[5] = {0};
	int naipe = -1;
	int jogou = 0;
	int n, v, i, c;
	MAO aux;

	if (isFlush(e.ultima_jogada)) e = joga_flush_igual_naipe(e,j,posX,posY);


	if (e.ultimo_jogador != j) {
	if (isFlush(e.ultima_jogada)) n = da_maior_naipe(e.ultima_jogada)+1;
	else n =0;
	

	for ( ; n<4 && jogou == 0 ; n++) {
		for(i=0; i<5; i++) { valores[i] = 0 ; naipe = -1; }
		c = 0;
		aux = 0;

		for (v=0; v<13 && jogou == 0 ; v++) {	
		    if (carta_existe(e.mao[j], n, v)) {
                naipe = n;
                valores[c] = v;
				c++;
			    aux = add_carta (aux, n, v);
		    }

			if (c==conta_cartas(e.ultima_jogada)) {
				for (i=0; i<c; i++) {
					e.mao[j] = rem_carta(e.mao[j], naipe, valores[i]);
					imprime_carta (BARALHO, posX, posY, e, j, naipe, valores[i]);
					posX += 20;
				}

				e.ultima_jogada = aux;
				e.ultimo_jogador = j;
				jogou = 1;
					}
	      }
	} 
}
	return e;
}



ESTADO joga_straight_flush (ESTADO e, int j, int posX, int posY) {
	MAO aux = 0;
	int n, v, i;
	int jogou = 0;
	int flag = 0;

	if (e.ultimo_jogador!=j) {
	if (isStraightFlush(e.ultima_jogada)) n = da_maior_naipe(e.ultima_jogada);
	else n = 0;


	for ( ; n<4 && jogou == 0; n++) {
		flag = 0; 
		for(v=0; v<13 && flag == 0 ; v++) {
			if (carta_existe(e.mao[j],n,v) && carta_existe(e.mao[j],n,v+1) && carta_existe(e.mao[j],n,v+2) && carta_existe(e.mao[j],n,v+3) && carta_existe(e.mao[j],n,v+4)) {
				for (i=0; i<5 && flag == 1; i++) {
					aux = add_carta(aux,n,v+i);
					e.mao[j] = rem_carta(e.mao[j], n, v+i);
					imprime_carta (BARALHO, posX, posY, e, j, n, v+i);
					posX += 20;
				}

				flag = 1;
				e.ultima_jogada = aux;
				e.ultimo_jogador = j;
				jogou = 1;
			}
		}
					
	}
}

	return e;
}

ESTADO joga_4OfAKind (ESTADO e, int j, int posX, int posY) {
		int c = 0;
		MAO aux = 0;
		int n, v, i;
        int naipes4[4];
        int valor4 = -1;
        int naipeS = -1;
        int valorS = -1;
        int flag = 0;
        int jogou = 0;

        if (is4OfAKind(e.ultima_jogada)) v = valor_4OfKind(e.ultima_jogada) + 1;
        else v = 0;

         for ( ; v<13 && jogou == 0 ; v++) {
         	for (n=0; n<4 && flag == 0; n++) {
         		if (carta_existe(e.mao[j], n, v)) { naipes4[c] = n ; valor4 = v ; c++; }
         		if (c==4) flag++;
         	}

         	if (flag!=0) { 
         		for (i=0 ; i<c ; i++) aux = add_carta(aux, naipes4[i], valor4);
                c = 0;
                flag = 0;
             
                for (v=0 ; v<13 && flag == 0 ; v++) {
                    c = 0;
              		for (n=0 ; n<4 && flag == 0 && v != valor4 ; n++) {
                       	if(carta_existe(e.mao[j], n, v)) { naipeS = n; valorS = v; c++; }
                        if(c==1) flag++;
                    }
                }
             
             	if (flag!=0)  {
             		aux = add_carta(aux, naipeS, valorS);

                    for (i=0; i<4; i++) {
						e.mao[j] = rem_carta(e.mao[j], naipes4[i], valor4);
						imprime_carta (BARALHO, posX, posY, e, j, naipes4[i], valor4);
						posX += 20;
				      }

						         
					e.mao[j] = rem_carta(e.mao[j], naipeS, valorS);
					imprime_carta (BARALHO, posX, posY, e, j, naipeS, valorS);
					posX += 20;
					
					e.ultima_jogada = aux;
				    e.ultimo_jogador = j;
					jogou = 1;
                 }

                else 
                    c = 0;
             }

            else {
                    for(i=0; i<4 ; i++) naipes4[i] = 0;	
                    valor4=0;
                    c = 0;
                  }
            }

            return e;
}



ESTADO joga_full_house (ESTADO e, int j, int posX, int posY) {
		int c = 0;
		int n, v, i;
        MAO aux = 0;
        int naipesT[3];
        int valorT=0;
        int naipesP[2];
        int valorP = 0;
        int flag = 0;
        int jogou = 0;

        if (isFullHouse(e.ultima_jogada)) v = maior_trio_fullhouse(e.ultima_jogada)+1;
        else v = 0;


         for (  ; v<13 && jogou == 0 ; v++) {
         	c=0;
         	for (n=0; n<4 && flag == 0; n++) {
         		if (carta_existe(e.mao[j], n, v)) { naipesT[c] = n ; valorT = v ; c++; }
         		if (c==3) flag++;
         	}

         	if (flag!=0) { for (i=0 ; i<c ; i++) aux = add_carta(aux, naipesT[i], valorT);
                           c = 0;
                           flag = 0;
                           for (v=0 ; v<13 && flag == 0 ; v++) {
                           	    c = 0;
                           		for (n=0 ; n<4 && flag == 0 && v != valorT ; n++) {
                       				if(carta_existe(e.mao[j], n, v)) { naipesP[c] = n; valorP = v; c++; }
                       		    	if(c==2) flag++;
                           		}
                           	}

                           	if (flag!=0)  {

                           		for (i=0 ; i<2 ; i++) aux = add_carta(aux, naipesP[i], valorP);

                           		for (i=0; i<3; i++) {
									e.mao[j] = rem_carta(e.mao[j], naipesT[i], valorT);
									imprime_carta (BARALHO, posX, posY, e, j, naipesT[i], valorT);
									posX += 20;
						         }

						         for (i=0; i<2; i++) {
									e.mao[j] = rem_carta(e.mao[j], naipesP[i], valorP);
									imprime_carta (BARALHO, posX, posY, e, j, naipesP[i], valorP);
									posX += 20;
						         }

						         e.ultima_jogada = aux;
						 		 e.ultimo_jogador = j;
								 jogou = 1;

                            }

                            else {
                            	valorP = -1;
                            	for(i=0; i<2 ; i++) naipesP[i] = 0;	
                            	
                               }
                        }

            else {
                 	valorT = -1;
                    for(i=0; i<3 ; i++) naipesT[i] = 0;	
                    
                  }
            }

            return e;
}


/**
FUnção que devolve o ESTADO depois de todos os bots jogarem ou passarem
*/
ESTADO joga_bot (ESTADO e) {
	int X[3] = {205,345,500};
	int Y[3] = {340,200,340};
	int j = 1;
	int jogou;

	do {
	    jogou = 0;

		if (e.ultimo_jogador == j) { e = joga_primeira(e,j,X[j-1],Y[j-1]); if (e.ultimo_jogador == j) jogou = 1; }

		if (conta_cartas(e.ultima_jogada)<4 && jogou == 0) {
			e = joga_comb_maior_naipe(e,j,X[j-1],Y[j-1]);
			if (e.ultimo_jogador == j) jogou = 1;
			if (jogou == 0) e = joga_comb_maior_valor(e,j,X[j-1],Y[j-1]);
		}

		if (conta_cartas(e.ultima_jogada)==5) 

			switch(ranking(e.ultima_jogada)) {
				case 1: e = joga_straight(e, j, X[j-1], Y[j-1]);
						if (e.ultimo_jogador == j) break; 
				case 2: e = joga_flush(e, j, X[j-1], Y[j-1]);
						if (e.ultimo_jogador == j) break; 
				case 3: e = joga_full_house(e, j, X[j-1], Y[j-1]);
						if (e.ultimo_jogador == j) break;
				case 4: e = joga_4OfAKind(e,j,X[j-1],Y[j-1]);
						if (e.ultimo_jogador == j) break;
				case 5: e = joga_straight_flush(e,j,X[j-1],Y[j-1]);
						break; 

			}

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
		printf("<a xlink:href = \"%s\"><image x = \"640\" y = \"655\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/pass2.png\" /></a>\n", script);
		} 
	else 
		printf("<image x = \"640\" y = \"655\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/pass1.png\" /></a>\n");
		
}

void imprime_ajuda (ESTADO e) {
	char script[10240];
	ESTADO new = e;

	new.ajuda=1;

	sprintf(script, "%s?%s", SCRIPT, estado2str(new));
	printf("<a xlink:href = \"%s\"><image x = \"640\" y = \"695\" height = \"115\" width = \"90\" xlink:href = \"http://127.0.0.1/botoes/ajuda.png\" /></a>\n", script);
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



/*****************************************************************************AJUDA**************************************************************************************************/


ESTADO seleciona_primeira (ESTADO e) {
	int n, v;
	int selecionou = 0;

	for (v = 0; v<13 && selecionou == 0 ; v++) 
				for (n = 0 ; n<4 && selecionou == 0; n++) 
					if (carta_existe(e.mao[0],n,v)) {
						e.selecao = add_carta(e.selecao, n, v);
						selecionou = 1;
					}

	return e;
} 

ESTADO seleciona_comb_maior_naipe (ESTADO e) {
	  int n = da_maior_naipe(e.ultima_jogada);
	  int valor_ultima = da_valor(e.ultima_jogada);
	  int selecionou = 0;
	  MAO aux=0;
	  int c = 0;

	  for ( ; n<4 && selecionou == 0 ; n++) {
			c = 0;
			
			if (carta_existe(e.mao[0], n, valor_ultima)) { 
				c++;
			    aux = add_carta(aux, n, valor_ultima);
		     }

			if (c == conta_cartas(e.ultima_jogada)) {
				e.selecao = aux;
				selecionou = 1;
			}
				
      }
	
	return e;
} 
	
ESTADO seleciona_comb_maior_valor (ESTADO e) {
	int v = da_valor(e.ultima_jogada);
	int n;
	int c = 0;
	int selecionou = 0;
	MAO aux;

	for (v = v+1 ; v<13 && selecionou == 0 ; v++) {
		c = 0;
		aux = 0;
		
		for (n = 0 ; n<4 && selecionou == 0 ; n++) {
			if (carta_existe(e.mao[0], n, v))   { 
				c++;
				aux = add_carta (aux, n, v);
			 }

			if (c == conta_cartas(e.ultima_jogada)) {
			e.selecao = aux;
			selecionou = 1;
			}
		}
	 }

	return e;	
}

ESTADO seleciona_straight_igual (ESTADO e) {
	int max_valor = maior_valorStraight(e.ultima_jogada);
	int c = 0;
	MAO aux = 0;
	int n, v;
	int selecionou = 0;
	int flag = 0;

	if ((max_valor-4)<0) {
		if ((max_valor-4)==-1) v=12;
		else v=11;

		for(; v<13 ; v++)
			for(n=0,flag=0; n<4 && flag == 0; n++)
				if (carta_existe(e.mao[0], n, v)) {
					c++;
					aux=add_carta(aux,n,v);
					flag++;
				}
	}

	if ((max_valor-4)<0) v=0;
	else v = max_valor-4; 

	for (; v<=max_valor && selecionou == 0; v++) {
		flag = 0; 
     	for(n=0 ; n<4 && flag == 0 ; n++) 
			if (carta_existe(e.mao[0],n,v)) {
					c++;
					aux = add_carta(aux, n, v);
					flag++;
				}

		if (flag == 0) return e;

		else if (c==conta_cartas(e.ultima_jogada)) {
			    if (da_maior_naipe_straight(aux)<da_maior_naipe_straight(e.ultima_jogada)) return e;
		    	e.selecao = aux;
				selecionou = 1;
		        } 
	}

	return e;
}

ESTADO seleciona_straight (ESTADO e) {
	int max_valor = maior_valorStraight(e.ultima_jogada);
	int c = 0;
	MAO aux = 0;
	int n, v;
	int selecionou = 0;
	int flag = 0;

	e = seleciona_straight_igual(e); 

	if (e.selecao == 0) {

	if ((max_valor-3)<0) {
		v=12;
		for(n=0,flag=0; n<4 && flag == 0; n++)
			if (carta_existe(e.mao[0], n, v)) {
				c++;
				aux=add_carta(aux,n,v);
				flag++;
				}
	}

	if((max_valor-3)<0) v=0;
	else v = max_valor-3;

	for ( ; v<12 && selecionou == 0; v++) {
		flag = 0; 
     	for(n=0 ; n<4 && flag == 0 ; n++) 
			if (carta_existe(e.mao[0],n,v)) {
					c++;
					aux = add_carta(aux, n, v);
					flag++;
				}

		if (flag == 0) {
		    c = 0;
		    aux = 0;
		}

		else if (c==conta_cartas(e.ultima_jogada)) {
		    	e.selecao = aux;
				selecionou = 1;
		        } 
	}
}
	return e;
}

ESTADO seleciona_flush_igual_naipe(ESTADO e) {
	int selecionou = 0;
	int n, v;
	int c = 0;
	MAO aux = 0;

	n = da_maior_naipe(e.ultima_jogada);
	

	for (v=0; v<13 && selecionou == 0 ; v++) {	
		if (carta_existe(e.mao[0], n, v)) {
			c++;
			aux = add_carta (aux, n, v);
		 }

			if (c==conta_cartas(e.ultima_jogada)) {
				if (da_valor_flush(aux)<da_valor_flush(e.ultima_jogada)) return e;
				e.selecao = aux;
				selecionou = 1;
			}
	      
	} 

	return e;
	     
}

ESTADO seleciona_flush(ESTADO e) {
	int selecionou = 0;
	int n, v, c;
	MAO aux;

	if (isFlush(e.ultima_jogada)) e = seleciona_flush_igual_naipe(e);


	if (e.ultimo_jogador != 0) {
	
	if (isFlush(e.ultima_jogada)) n = da_maior_naipe(e.ultima_jogada)+1;
	else n =0;
	

	for ( ; n<4 && selecionou == 0 ; n++) {
		c = 0;
		aux = 0;

		for (v=0; v<13 && selecionou == 0 ; v++) {	
		    if (carta_existe(e.mao[0], n, v)) {
				c++;
			    aux = add_carta (aux, n, v);
		    }

			if (c==conta_cartas(e.ultima_jogada)) {
				e.selecao = aux;
				selecionou = 1;
				}
	      }
	} 
}
	return e;
}


ESTADO seleciona_fullhouse (ESTADO e) {
		int c = 0;
		int n, v, i;
        MAO aux = 0;
        int naipesT[3];
        int valorT=0;
        int naipesP[2];
        int valorP = 0;
        int flag = 0;
        int selecionou = 0;

        if (isFullHouse(e.ultima_jogada)) v = maior_trio_fullhouse(e.ultima_jogada)+1;
        else v = 0;


        for (  ; v<13 && selecionou == 0 ; v++) {
         	c=0;
         	for (n=0; n<4 && flag == 0; n++) {
         		if (carta_existe(e.mao[0], n, v)) { naipesT[c] = n ; valorT = v ; c++; }
         		if (c==3) flag++;
         	}

         	if (flag!=0) { for (i=0 ; i<c ; i++) aux = add_carta(aux, naipesT[i], valorT);
                           c = 0;
                           flag = 0;
                           for (v=0 ; v<13 && flag == 0 ; v++) {
                           	    c = 0;
                           		for (n=0 ; n<4 && flag == 0 && v != valorT ; n++) {
                       				if(carta_existe(e.mao[0], n, v)) { naipesP[c] = n; valorP = v; c++; }
                       		    	if(c==2) flag++;
                           		}
                           	}

                           	if (flag!=0)  {

                           		for (i=0 ; i<2 ; i++) aux = add_carta(aux, naipesP[i], valorP);

						         e.selecao = aux;
								 selecionou = 1;

                            }

                            else {
                            	valorP = -1;
                            	for(i=0; i<2 ; i++) naipesP[i] = 0;	
                            	
                               }
                        }

            else {
                 	valorT = -1;
                    for(i=0; i<3 ; i++) naipesT[i] = 0;	
                    
                  }
            }

            return e;
}


ESTADO seleciona_straight_flush (ESTADO e) {
	MAO aux = 0;
	int n, v, i;
	int selecionou = 0;
	int flag = 0;

	if (e.ultimo_jogador!=0) {
	if (isStraightFlush(e.ultima_jogada)) n = da_maior_naipe(e.ultima_jogada);
	else n = 0;


	for ( ; n<4 && selecionou == 0; n++) {
		flag = 0; 
		for(v=0; v<13 && flag == 0 ; v++) {
			if (carta_existe(e.mao[0],n,v) && carta_existe(e.mao[0],n,v+1) && carta_existe(e.mao[0],n,v+2) && carta_existe(e.mao[0],n,v+3) && carta_existe(e.mao[0],n,v+4)) {
				for (i=0; i<5 && flag == 1; i++)
					aux = add_carta(aux,n,v+i);


				flag = 1;
				e.selecao = aux;
				selecionou = 1;
			}
		}
					
	}
}

	return e;
}

ESTADO seleciona_4OfAKind (ESTADO e) {
		int c = 0;
		MAO aux = 0;
		int n, v, i;
        int naipes4[4];
        int valor4 = -1;
        int naipeS = -1;
        int valorS = -1;
        int flag = 0;
        int selecionou = 0;

        if (is4OfAKind(e.ultima_jogada)) v = valor_4OfKind(e.ultima_jogada) + 1;
        else v = 0;

         for ( ; v<13 && selecionou == 0 ; v++) {
         	for (n=0; n<4 && flag == 0; n++) {
         		if (carta_existe(e.mao[0], n, v)) { naipes4[c] = n ; valor4 = v ; c++; }
         		if (c==4) flag++;
         	}

         	if (flag!=0) { 
         		for (i=0 ; i<c ; i++) aux = add_carta(aux, naipes4[i], valor4);
                c = 0;
                flag = 0;
             
                for (v=0 ; v<13 && flag == 0 ; v++) {
                    c = 0;
              		for (n=0 ; n<4 && flag == 0 && v != valor4 ; n++) {
                       	if(carta_existe(e.mao[0], n, v)) { naipeS = n; valorS = v; c++; }
                        if(c==1) flag++;
                    }
                }
             
             	if (flag!=0)  {
             		aux = add_carta(aux, naipeS, valorS);
					e.selecao = aux;
					selecionou = 1;
                 }

                else 
                    c = 0;
             }

            else {
                    for(i=0; i<4 ; i++) naipes4[i] = 0;	
                    valor4=0;
                    c = 0;
                  }
            }

            return e;
}


ESTADO ajuda (ESTADO e) {
	e.selecao = 0;

	if (e.ultimo_jogador == 0) { e = seleciona_primeira(e);}
	if ((e.ultimo_jogador != 0) && (conta_cartas(e.ultima_jogada) < 4)) {
		e = seleciona_comb_maior_naipe(e);
		if (e.selecao == 0)
			e = seleciona_comb_maior_valor(e);
	}
	if ((e.ultimo_jogador != 0) && (conta_cartas(e.ultima_jogada) == 5)) {
		switch (ranking(e.ultima_jogada)) {
			case 1: e = seleciona_straight(e);
					if (e.selecao != 0) break;
			case 2: e = seleciona_flush(e);
					if (e.selecao != 0) break;
			case 3: e = seleciona_fullhouse(e);
					if (e.selecao != 0) break;
			case 4: e = seleciona_4OfAKind(e);
					if (e.selecao != 0) break;
			case 5: e = seleciona_straight_flush(e);
			        break;
		}
	}

	e.ajuda = 0;
	return e;

}


/**
Função que imprime o botão "ORDENA"
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
Imprime o estado
Esta função está a imprimir o estado em quatro colunas: uma para cada naipe
@param path	o URL correspondente à pasta que contém todas as cartas
@param ESTADO	O estado atual
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
		
/**
Função que distribui as cartas baralhadas pelos 4 jogadores
*/
ESTADO baralha (ESTADO e) {
	
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