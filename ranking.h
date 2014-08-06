#include "stdio.h"
#define  PATCH "ranking.txt"

Ranking* newRanking();

Player* newPlayer(char name[], int score);

void addToRanking(Ranking *rank,Player *p);

Ranking* getRanking();

//destroi a fila e escreve no arquivo
void rankingDestroy(Ranking *rank);

void printRanking(Ranking *rank);



