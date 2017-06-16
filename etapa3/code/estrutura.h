#define FORMATO 	"%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%d_%d_%d_%lld_%d_%d_%d"
#define BARALHO		"http://127.0.0.1/cards"
#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
#define NAIPES		"DCHS"
#define VALORES		"3456789TJQKA2"

typedef long long int MAO;

typedef struct {
	MAO mao[4]; 
	int cartas[4];  
	MAO selecao; 
	int passar;
	int jogar;
	int selecionar;
	MAO ultima_jogada; 
	int ultimo_jogador; 
	int ordenacao;
	int ajuda;
} ESTADO;