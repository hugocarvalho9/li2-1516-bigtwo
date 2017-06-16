#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "estrutura.h"
#include "jogo.h"

/**
Função que converte uma string num estado.
@param char* Recebe uma string.
@return ESTADO  Retorna o ESTADO obtido pela string.
*/
ESTADO str2estado (char* str) {
	ESTADO e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.cartas[0], &e.cartas[1], &e.cartas[2],&e.cartas[3], &e.selecao, &e.passar, &e.selecionar, &e.jogar, &e.ultima_jogada, &e.ultimo_jogador, &e.ordenacao, &e.ajuda);
	return e;
}

/**
Função que converte um ESTADO numa string.
@param ESTADO Recebe um ESTADO
@return char*  Retorna a string obtida a partir do ESTADO.

*/
char* estado2str (ESTADO e){
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.cartas[0],e.cartas[1],e.cartas[2],e.cartas[3], e.selecao, e.passar, e.selecionar, e.jogar, e.ultima_jogada, e.ultimo_jogador, e.ordenacao, e.ajuda);
	return res;
}

/**
Função que distribui as cartas baralhadas pelos 4 jogadores.
@param ESTADO Recebe o estado atual.
@return ESTADO Devolve o estado depois de baralhar.
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
/**
Função que seleciona a primeira carta que existe na mão. Para isso, percorre a partir de 2 ciclos as cartas até que existe alguma
que exista na mão. Depois, adiciona essa carta à seleção.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
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
/**
Função que seleciona a maior combinação de naipe.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
ESTADO seleciona_comb_maior_naipe (ESTADO e) {
	int n = da_maior_naipe(e.ultima_jogada);
	int valor_ultima = da_valor(e.ultima_jogada);
    int selecionou = 0;
	int c = 0;
	MAO aux=0;

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
/**
Função que seleciona a maior combinação de valor.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/		
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
/**
Função que seleciona um straight de igual valor.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
ESTADO seleciona_straight_igual (ESTADO e) {
	int max_valor = maior_valorStraight(e.ultima_jogada);
	int n, v, c, selecionou, flag;
	MAO aux = 0;
	c = selecionou = flag = 0;
	
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
/**
Função que seleciona um straight.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
ESTADO seleciona_straight (ESTADO e) {
	int max_valor = maior_valorStraight(e.ultima_jogada);
	int n, v, c, selecionou, flag;
	MAO aux = 0;
	c = selecionou = flag = 0;

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

		if (flag == 0) { c = 0; aux = 0; }

		else if (c==conta_cartas(e.ultima_jogada)) {
		    	e.selecao = aux;
				selecionou = 1;
		        } 
		}
	}	
	return e;
}
/**
Função que seleciona um flush de igual naipe.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
ESTADO seleciona_flush_igual_naipe(ESTADO e) {
	int n, v, c, selecionou;
	MAO aux = 0;
	c = selecionou = 0;

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
/**
Função que seleciona um flush.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
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

/**
Função que seleciona um Full House.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
ESTADO seleciona_fullhouse (ESTADO e) {
		int n, v, i, c, flag, selecionou , valorP, valorT;
        MAO aux = 0;
        int naipesT[3];
        int naipesP[2];
        c = flag = selecionou = valorP = valorT = 0;

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

/**
Função que seleciona um straight flush.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
ESTADO seleciona_straight_flush (ESTADO e) {
	MAO aux = 0;
	int n, v, i, selecionou, flag;
	selecionou = flag = 0;

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
/**
Função que seleciona um 4ofAkind.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
ESTADO seleciona_4OfAKind (ESTADO e) {
		MAO aux = 0;
		int n, v, i, c, flag, selecionou, valor4, naipeS, valorS;
        int naipes4[4];
        valor4 = naipeS = valorS = -1;
        c = flag = selecionou = 0;
 
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
                else c = 0;
             }

            else {
                for(i=0; i<4 ; i++) naipes4[i] = 0;	
                valor4=0;
                c = 0;
            }
   		}

    return e;
}
/**
Retorna o estado depois de se usar o botão ajuda.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado.
*/
ESTADO ajuda (ESTADO e) {
	e.selecao = 0;

	if (e.ultimo_jogador == 0) e = seleciona_primeira(e);
	if ((e.ultimo_jogador != 0) && (conta_cartas(e.ultima_jogada) < 4)) {
		e = seleciona_comb_maior_naipe(e);
		if (e.selecao == 0) e = seleciona_comb_maior_valor(e);
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