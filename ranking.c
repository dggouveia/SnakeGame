#include "ranking.h"
#include "stdlib.h"
#include "string.h"
#include "ncurses.h"
#include "tela.h"

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
	FILE *file = fopen(PATCH,"w");
	tmp = rank->top;
	while(tmp){
		fprintf(file, "%s", tmp->name);
		fprintf(file, "%d\n", tmp->points);
		tmp = tmp->next;
	}
	fclose(file);
}


void insertOnRanking(int pScore){
	char line[20], name[5], newName[5];
	int cont = 0, score, entry = 0, *lenght;

	FILE *file = fopen(PATCH,"r");
	if(file == NULL){
		
		file = fopen(PATCH,"w");
		fclose(file);
		file = fopen(PATCH,"r");
		
	}

	Ranking *rankAux = getRanking();
	Ranking *rank = newRanking();
	if (*(lenght = &rankAux->lenght) == 0 && pScore!=0)
	{
		getName(newName);
		strcat(newName,"\n");
		addToRanking(rank, newPlayer(newName, pScore));
		rankingDestroy(rankAux);
	}else{
		lenght = &rank->lenght;
		while(fgets(line, 20, file)){
			if(!(cont%2)){
				strcpy(name, line);
			}else{
	    		score = atoi(line);
				Player *player =  newPlayer(name, score);
				if(entry == 0 && player->points <= pScore && pScore!=0){
					getName(newName);
					strcat(newName,"\n");
					addToRanking(rank, newPlayer(newName, pScore));
					entry = 1;
				}
					addToRanking(rank, player);
			}

			cont++;
		}

		if(entry == 0 && *lenght<10 && pScore>0){
			getName(newName);
			strcat(newName,"\n");
			addToRanking(rank, newPlayer(newName, pScore));
			entry = 1;
		}
	}

	writeRanking(rank);
	rankingDestroy(rank);
	fclose(file);
	openMenu();
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