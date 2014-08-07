#include "ranking.h"
#include "stdlib.h"
#include "string.h"
#include "ncurses.h"

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
	p->lenght = 0;
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
	if (rank->lenght<10){
		if (rank->top == NULL){
			rank->top = p;
			rank->tail = p;
			rank->lenght++;
		}else{
			rank->tail->next = p;
			rank->tail = p;
			rank->lenght++;
		}
		
	}
	
}

Ranking* getRanking(){
	Ranking *rank = newRanking();
	char line[20], name[5];
	int cont = 0, score;

	FILE *file = fopen(PATCH,"r");
	if(file == NULL){
		
		file = fopen(PATCH,"w");
		fclose(file);
		file = fopen(PATCH,"r");
		
	}

	while(fgets(line, 20, file)){
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

	while(rank->top){
		tmp = rank->top->next;
		free(rank->top);
		rank->top = tmp;
	}

	free(rank);
}

void writeRanking(Ranking *rank){
	Player *tmp;
	file = fopen(PATCH,"w");
	tmp = rank->top;
	while(tmp){
		fprintf(file, "%s", tmp->name);
		fprintf(file, "%d\n", tmp->points);
		tmp = tmp->next;
	}
	fclose(file);
}

void printRanking(Ranking *rank, WINDOW *tela){
	Player *aux = rank->top;
	int cont = 2;
	mvwprintw(tela,1,2,"Score    Name");
	while(aux){
		mvwprintw(tela,cont++,2,"%d   -   %s", aux->points, aux->name);
		aux = aux->next;
	}
}
