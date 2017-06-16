#include "estrutura.h"

#include "jogo.h"
#include "interface.h"

/**
Função que joga a carta mais baixa que existir. Para isso usa 2 ciclos, um para percorrer os valores outro para os naipes e quando descobrir 
alguma carta que exista na mão ela vai ser removida da mão, imprimida, e adicionada à ultima jogada.
@param ESTADO Recebe o estado.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado depois de se ter jogado uma carta.
*/

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

/**

@param ESTADO Recebe o estado.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado.

*/
ESTADO joga_comb_maior_naipe (ESTADO e, int j, int posX, int posY) {
	  int n = da_maior_naipe(e.ultima_jogada);
	  int valor_ultima = da_valor(e.ultima_jogada);
	  MAO aux=0;
	  int i, jogou, c;
	  int naipes[3] = {0};
	  int valores[3] = {0};
	  jogou = c = 0;

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

/**
@param ESTADO Recebe o estado.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado.

*/
ESTADO joga_comb_maior_valor (ESTADO e, int j, int posX, int posY) {
	int v = da_valor(e.ultima_jogada);
	int n, i, c, jogou;
	int naipes[3] = {0};
	int valores[3] = {0};
	MAO aux;
	c = jogou = 0;

	for (v = v+1 ; v<13 && jogou == 0 ; v++) {
		for(i=0; i<3; i++) { naipes[i]=0; valores[i]=0; }
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

/**
Função que joga um straight de igual valor.
@param int	Recebe o estado atual.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado. 

*/

ESTADO joga_straight_igual (ESTADO e, int j, int posX, int posY) {
	int naipes[5] = {0};
	int valores[5] = {0};
	int max_valor = maior_valorStraight(e.ultima_jogada);
	MAO aux = 0;
	int n, v, i, c, jogou, flag;
	c = jogou = flag = 0;

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

/**
Função que joga um straight.
@param ESTADO Recebe o estado.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado.
*/

ESTADO joga_straight (ESTADO e, int j, int posX, int posY) {
	int naipes[5] = {0};
	int valores[5] = {0};
	int max_valor = maior_valorStraight(e.ultima_jogada);
	MAO aux = 0;
	int n, v, i, c, jogou, flag;
	c = jogou = flag = 0;

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

/**
Função que joga um flush de igual naipe.
@param ESTADO Recebe o estado.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado.
*/
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


/**
Função que joga um flush.
@param ESTADO Recebe o estado.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado.
*/
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

/**
Função responsável por jogar um  Full House.
@param ESTADO Recebe o estado.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado.

*/
ESTADO joga_full_house (ESTADO e, int j, int posX, int posY) {
		int n, v, i, c, valorT, valorP, flag, jogou;
        MAO aux = 0;
        int naipesT[3];
        int naipesP[2];
        c = valorT = valorP = flag = jogou = 0;

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
                            } else {
                            	valorP = 0;
                            	for(i=0; i<2 ; i++) naipesP[i] = 0;	
                               }
            }
            else {
                valorT = 0;
                for(i=0; i<3 ; i++) naipesT[i] = 0;	
            }
        }
    return e;
}

/**
Função responsável por jogar um Four of a Kind.
@param ESTADO Recebe o estado.
@param int	Recebe o número do jogador.
@param int	Recebe a posição do x.
@param int	Recebe a posição do y.
@return ESTADO Retorna o estado.
*/
ESTADO joga_4OfAKind (ESTADO e, int j, int posX, int posY) {
		MAO aux = 0;
		int n, v, i, c, valor4, naipeS, valorS, flag, jogou;
        int naipes4[4];
        c = valor4 = naipeS = valorS = flag = jogou = 0;

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
                 }  else  c = 0;
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
Função  que faz com que os bots joguem ou passem. Para isso a função está dividida em 3 partes. Uma para quando
o bot joga, sendo ele o último jogador, logo joga a carta mais baixa que tiver na mão,outra para quando combinações de menos de 5 cartas
e, por último, uma para as combinações de 5 cartas.
@param ESTADO Recebe o estado.
@return ESTADO Retorna o estado depois dos bots jogarem ou passarem.
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
				case 5: break; 

			}

		if (conta_cartas(e.mao[j]) == 0) {  imprime_vencedor(e,j+1);
											j=4; 
										 }
		j++;

	} while (j<4);

	return e;
}