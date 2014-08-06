#include "ranking.h"
#include "stdlib.h"
#include "string.h"

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

Ranking* newRanking(){
	Ranking *p = malloc(sizeof(Ranking));
	p->top = NULL;
	p->tail = NULL;
	return p;
}

Player* newPlayer(char name[], int score){
	Player *p = malloc(sizeof(Player));
	strcpy(p->name, name);
	p->points = score;
	p->next = NULL;
	return p;
}

void addToRanking(Ranking *rank,Player *p){
	if (rank->top == NULL){
		rank->top = p;
		rank->tail = p;
	}else{
		rank->tail->next = p;
		rank->tail = p;
	}
	rank->lenght++;
}

Ranking* getRanking(){
	Ranking *rank = newRanking();
	char line[12], name[5];
	int cont = 0, score;

	FILE *file = fopen(PATCH,"r");
	if(file == NULL){
		
		file = fopen(PATCH,"w");
		fclose(file);
		file = fopen(PATCH,"r");
		
	}

	while(fgets(line, 10, file)){
		if(!(cont%2)){
			strcpy(name, line);
		}else{
    		score = atoi(line);
			addToRanking(rank, newPlayer(name, score));
		}
		cont++;
	}

	fclose(file);
	return rank;

}

void rankingDestroy(Ranking *rank){
	
	Player *tmp;

	while(rank->top->next){
		tmp = rank->top->next;
		free(rank->top);
		rank->top = tmp;
	}

	free(rank->top);
}

void printRanking(Ranking *rank){
	Player *aux = rank->top;
	while(aux){
		printf("%d - %s", aux->points, aux->name);
		aux = aux->next;
	}
	printf("%d\n", rank->lenght);
}

int main(int argc, char const *argv[])
{
	Ranking *rank = getRanking();
	printRanking(rank);
	rankingDestroy(rank);
	getchar();

	return 0;

}