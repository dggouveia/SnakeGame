#include "stdio.h"
#define  PATCH "ranking.txt"

Ranking* newRanking();

Player* newPlayer(char name[], int score);

void addToRanking(Ranking *rank,Player *p);

Ranking* getRanking();

void rankingDestroy(Ranking *rank);

void printRanking(Ranking *rank);

void writeInFile(Ranking *rank);



