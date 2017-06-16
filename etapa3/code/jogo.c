#include "estrutura.h"

/** 
Devolve o índice da carta.
@param naipe	O naipe da carta (inteiro entre 0 e 3).
@param valor	O valor da carta (inteiro entre 0 e 12).
@return	Int     O índice correspondente à carta (um inteiro).
*/
int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}
/**
Adiciona uma carta ao estado.
@param ESTADO	O estado atual.
@param naipe	O naipe da carta (inteiro entre 0 e 3).
@param valor	O valor da carta (inteiro entre 0 e 12).
@return	ESTADO	O novo estado com a carta adicionada.\n
<b>Exemplo:</b> add_carta(ESTADO,0,0) adiciona o 3♦ ao estado.
*/
long long int add_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO | ((long long int) 1 << idx);
}

/**
Remove uma carta do estado.
@param ESTADO	O estado atual.
@param naipe	O naipe da carta (inteiro entre 0 e 3).
@param valor	O valor da carta (inteiro entre 0 e 12).
@return	ESTADO	O novo estado com a carta removida.\n
<b>Exemplo:</b> rem_carta(ESTADO,0,0) remove o 3♦ do estado.
*/
long long int rem_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO & ~((long long int) 1 << idx);
}


/**
Verifica se uma carta pertence ao estado.
@param ESTADO	O estado atual.
@param naipe	O naipe da carta (inteiro entre 0 e 3).
@param valor	O valor da carta (inteiro entre 0 e 12).
@return	ESTADO	1 se a carta existe e 0 caso contrário.\n
<b>Exemplo:</b> carta_existe(ESTADO,0,0) que representa o 3♦, se existir no estado obtém-se o valor 1.

*/
int carta_existe(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (ESTADO >> idx) & 1;
}
/**
Conta quantas cartas tem uma mão.
@param MAO	Recebe uma mão.
@return	int	Retorna o número de cartas dessa mão.\n
<b>Exemplo:</b> conta_cartas(1099780128770) que é a mão com 4♦ 4♠ 5♥ 6♣ vai retornar o número 5.
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
Verifica na mão qual é o maior valor do straight.

@param MAO	Recebe uma mão.
@return	int	Retorna o maior valor do straight ou retorna -1 caso exista algum Full  House ou um Four of Kind ou se 
não existir nenhum straight\n
<b>Exemplo:</b> maior_valorStraight(1342177322) que é a mão com 4♦ 5♥ 6♦ 7♥ 8♦ vai retornar o valor 5 (que representa o 8).
*/
int maior_valorStraight (MAO e) {
   	int n, v, i;
    int aux[14]= {0};

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
/**
Verifica na mão qual é o maior naipe do straight. Esta função com a variável max vai buscar o valor obtido no maior_valorStraight
para depois descobrir qual é o naipe do maior valor do straight, para assim se puder comparar straights do mesmo valor.

@param MAO	Recebe uma mão.
@return	int	Retorna o naipe do maior valor do straigh (entre 0 e 4)\n
<b>Exemplo:</b> da_maior_naipe_straight(1342177322) que é a mão com 4♦ 5♥ 6♦ 7♥ 8♦ vai retornar o valor 0 (que representa o ouros).
*/
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
/**
Verifica se uma determinada mão tem algum Full House. 

@param MAO	Recebe uma mão.
@return	int	Retorna 1 caso a mão tenha algum straight.\n
<b>Exemplo:</b> isStraight(1342177322) que é a mão com 4♦ 5♥ 6♦ 7♥ 8♦ vai retornar 1.
*/
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
/**
Descobre qual é o valor do flush.

@param MAO	Recebe uma mão.
@return	int	Retorna o valor do flush ou retorna -1 caso não exista nenhum flush na mão.\n
<b>Exemplo:</b> da_valor_flush(138028392512) que é a mão com 3♣ 6♥ 7♣ 9♦ J♣ A♣ 2♣  vai retornar 12.
*/
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
/**
Verifica se uma determinada mão tem algum flush. Isto é se existe 5 cartas com o mesmo naipe. Para isso a função isFlush
usa 4 ciclos for para ver se algum dos naipes (ouros,paus,copas,espadas) se repete 5 vezes.

@param MAO	Recebe uma mão da seleção.
@return	int	Retorna 1 caso a mão tenha um flush, retorna 0 caso contrário.\n
<b>Exemplo:</b> isFlush(138028392512) que é a mão com 3♣ 6♥ 7♣ 9♦ J♣ A♣ 2♣  vai retornar 1.
*/
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
/**
Descobre qual é o maior valor do trio do Full House. Para isso descobre, a partir de dois ciclos for, se existe algum valor que tenha 
tenha 3 naipes (por exemplo 5♦ 5♣ 5♠). Caso exista, passa esse valor para um variável r e retorna essa mesmo variável.
@param MAO	Recebe uma mão.
@return	int	Retorna o valor do trio do Full House ou -1 caso não exista.\n
<b>Exemplo:</b> isFullHouse(6597069864964) que é a mão com 5♦ 5♣ 5♠ 6♣ 6♠ vai retornar o valor 2 (representa o 5).
*/
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

/**
Verifica se uma determinada mão tem algum Full House. Ela vai verificar se existe um trio (com ajuda da variável tem3cartas)
e um par (com a tem2cartas). Caso os tenha, ambas estas variáveis vão passar a ter valor 1,para depois serem usadas no return.

@param MAO	Recebe uma mão.
@return	int	Retorna 1 caso a mão tenha 5 cartas e se as variáveis tem2cartas e tem3cartas estiverem a 1.\n
<b>Exemplo:</b> isFullHouse(1099780128770) que é a mão com 4♦ 4♠ 5♥ 6♣ vai retornar o número 0.
*/
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
/**
Descobre o valor do Four of a Kind. Para isso usa o mesmo métodos que o maior_trio_fullhouse, no entanto vai procura um valor que tenha
os 4 naipes (por exemplo 5♦ 5♣ 5♥ 5♠).
@param MAO	Recebe uma mão.
@return	int	Retorna o valor do Four of a Kind ou -1 caso não exista.\n
<b>Exemplo:</b> valor_4OfKind(1099780128770) que é a mão com 4♦ 4♠ 5♥ 6♣ vai retornar -1,pois não possui nenhum Four of a Kind.
*/
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
/**
Verifica se uma determinada mão tem algum 4 of a kind. A função vai verificar a partir de dois ciclos for se existe na mão
algum valor que tenha os 4 naipes. Caso isto aconteça a variável tem4cartas fica a 1.

@param MAO	Recebe uma mão.
@return	int	Retorna 1 caso a mão tenha 5 cartas e se a variável tem4cartas estiver a 1.\n
<b>Exemplo:</b> is4OfKind(1099780128770) que é a mão com 4♦ 4♠ 5♥ 6♣ vai retornar 0.
*/
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

/**
Verifica se uma determinada mão tem um straight flush. Visto que essa combinação é a junção de duas outras combinações o straight e o flush,
esta função verifica se ambas as combinações estão presentes de forma a que exista um straight flush.
@param MAO	Recebe uma mão.
@return	int	Retorna o 1 caso exista um straight flush. 
*/
int isStraightFlush (MAO m) {
	return (isStraight(m) && isFlush(m));
}
/**
Estabelece a ordem de grandeza das combinações, sendo o straight flush a maior e o straight a menor, para assim poder-se
comparar as jogadas e ver qual é a combinação mais alta (entre combinações de 5 cartas).
@param MAO	Recebe uma mão.
@return	int	Retorna o valor da jogada entre 1 e 5. Sendo 1 o straight e 5 o straight flush.
*/
int ranking (MAO m) {
	if (isStraightFlush(m)) return 5;
	if (isStraight(m)) return 1;
	if (isFlush(m)) return 2;
	if (isFullHouse(m)) return 3;
	if (is4OfAKind(m)) return 4;
	
	return 0;
}
/**
Verifica se uma combinação de cartas é valida a partir de vários fatores com o número de cartas e a existência de straight, flush, fullhouse
e 4ofakind.
@param MAO	Recebe a mão selecionada.
@return	int	Retorna 1 caso a combinão seja válida e 0 caso contrário.
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
Verifica se o tamanho entre duas mãos é igual.
@param MAO	Recebe uma mão.
@param MAO	Recebe uma mão.
@return	int	Retorna 1 caso as duas mãos tenham o mesmo tamanho ou 0 caso contrário.
*/
int compara_tamanho(MAO m1, MAO m2){
	if (conta_cartas(m1) == conta_cartas(m2)) {
		return 1;
	} 

	return 0;
}
/**
Dada uma mão, retorna o valor dessa mão.
@param MAO	Recebe uma mão.
@return	int	Retorna o valor da mão.
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
Dada uma mão, dá o maior naipe existente nessa mão. Para isso vai se verificar que cartas é que existem na mão e conforme o naipe que
aparecer se este for maior que o anterior ele vai o substituir na variável "max".
@param MAO	Recebe uma mão.
@return	int	Retorna o valor do maior naipe da mão (entre 0 e 4).
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
Verifica se a combinação de cartas selecionadas é maior do que a combinação de cartas da ultima jogada. Para isso usa várias funções
que comparam aspetos como o valor da mão, o ranking, o número de cartas da mão, entre outros.
@param MAO	Recebe a mão selecionad.
param MAO	Recebe a mão da última jogada.
@return	int Retorna 1 caso seja maior e 0 caso contrário.
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
Verifica se estou em condições para poder jogar.
@param MAO	Recebe um estado.
@return	int	1 caso possa jogar, 0 caso contrário. 
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
Função que calcula o score de uma mão no final do jogo.
@param MAO	Recebe uma mão.
@return	int	Retorna o score.
*/
int calcula_score (MAO m) {
	int r = 0;

	if (conta_cartas(m) == 13) r = 3*13;
	if (conta_cartas(m) <= 9 && conta_cartas(m) > 0) r = 1*(conta_cartas(m));
	if (conta_cartas(m) >= 10 && conta_cartas(m)<=12) r = 2*(conta_cartas(m));

	return r; 
}