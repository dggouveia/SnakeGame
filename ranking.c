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
	if (rank->lenght<11){
		if (rank->top == NULL){
			rank->top = p;
			rank->tail = p;
			rank->lenght++;
		}else{
			rank->tail->next = p;
			rank->tail = p;
		}
		rank->lenght++;
	}
	
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
	file = fopen(PATCH,"w");

	while(rank->top->next){
		fprintf(file, "%s", rank->top->name);
		fprintf(file, "%d\n", rank->top->points);
		tmp = rank->top->next;
		free(rank->top);
		rank->top = tmp;
	}
	fprintf(file, "%s", rank->top->name);
	fprintf(file, "%d\n", rank->top->points);
	free(rank->top);
}

void printRanking(Ranking *rank){
	Player *aux = rank->top;
	while(aux){
		printf("%d - %s", aux->points, aux->name);
		aux = aux->next;
	}
}

int main(int argc, char const *argv[])
{
	Ranking *rank = getRanking();
	char name[4];
	scanf("%c%c%c",&name[0],&name[1],&name[2]);
	strcat(name,"\n");
	
	addToRanking(rank, newPlayer(name, 100));
	printRanking(rank);
	rankingDestroy(rank);


	return 0;

}