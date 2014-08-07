#include "stdio.h"
#define  PATCH "ranking.txt"

typedef struct player Player;
typedef struct ranking Ranking;

struct player{
		char name[5];
		int points;
		Player *next;
};

struct ranking{
	Player *top;
	Player *tail;
	int lenght;
};


Ranking* newRanking();

Player* newPlayer(char name[], int score);

void addToRanking(Ranking *rank,Player *p);

Ranking* getRanking();

void writeRanking(Ranking *rank);

//destroi a fila e escreve no arquivo
void rankingDestroy(Ranking *rank);

void printRanking(Ranking *rank, WINDOW *tela);



