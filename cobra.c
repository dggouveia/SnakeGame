#include "cobra.h"
#include <stdlib.h>
#include "tela.h"

Food* createFood (){
	
	Food *food =  malloc (sizeof(Food));
	return food;

}

SnakePart* newSnakePart (){

	SnakePart *part = malloc (sizeof (SnakePart));
	part->prev = part->next = NULL;
	return part;
}

void snakeIncrease (Snake *snake){

	SnakePart *newPart = newSnakePart();

	if (snake->tail->line > snake->tail->prev->line){
		newPart->line = snake->tail->line + 1;
		newPart->col  = snake->tail->col;
	}
	else if (snake->tail->line < snake->tail->prev->line){
		newPart->line = snake->tail->line - 1;
		newPart->col  = snake->tail->col;	
	}
	else if (snake->tail->col > snake->tail->prev->col){
		newPart->line = snake->tail->line;
		newPart->col  = snake->tail->col + 1;		
	}
	else if (snake->tail->col < snake->tail->prev->col){
		newPart->line = snake->tail->line;
		newPart->col  = snake->tail->col - 1;		
	}

	snake->tail->next = newPart;
	newPart->prev = snake->tail;
	newPart->next = NULL;
	snake->tail = newPart;
	snake->length++;
	
}

Snake* createSnake (){

	Snake *snake = malloc (sizeof(Snake));
	snake->head = newSnakePart();
	snake->tail = newSnakePart();
	snake->head->next = snake->tail;
	snake->tail->prev = snake->head;
	
	return snake;
}

void snakeDestroy (Snake *snake){

	while (!snake->head){
		SnakePart *part = snake->head;
		snake->head = snake->head->next;
		free(part);
	}

	free(snake);
}

void snakeReverse (Snake *snake){
	SnakePart *part;
	snake->tail = snake->head;

	while (snake->head){
		part = snake->head->next;
		snake->head->next = snake->head->prev;
		snake->head->prev = part;
		if (part)
			snake->head = part;
		else break;
	}
}