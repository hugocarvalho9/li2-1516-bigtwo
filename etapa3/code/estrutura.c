#define FORMATO 	"%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%d_%d_%d_%lld_%d_%d_%d"
#define BARALHO		"http://127.0.0.1/cards"
#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
#define NAIPES		"DCHS"
#define VALORES		"3456789TJQKA2"

/**
	Estado inicial com todas as 52 cartas do baralho
	Cada carta é representada por um bit que está
	a 1 caso ela pertença à mão ou 0 caso contrário.
*/
typedef long long int MAO;

typedef struct {
	MAO mao[4];  /** Array com as maos dos 4 jogadores */
	int cartas[4];  /** Array com o numero de cartas que cada jogador tem, um inteiro entre 0 e 13 */ 
	MAO selecao; /** Array para representar as cartas que estao selecionadas */
	int passar;/** 1 se passa */
	int jogar;/** 1 se joga */
	int selecionar;/** 1 se a carta é selecionada */
	MAO ultima_jogada; /** Guarda a mão que foi jogada na ultima jogada */
	int ultimo_jogador; /** Guarda o numero do ultimo jogador que jogou */
	int ordenacao;/** 0 se por valor, 1 se por naipe */
	int ajuda;/** 1 caso o botão de ajuda esteja a ser usado, 0 caso contrário */
} ESTADO;