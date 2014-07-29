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

void snakeIncrease (Snake *snake, int key){

	SnakePart *newPart = newSnakePart();
	newPart->line = snake->tail->line;
	newPart->col = snake->tail->col;
	moveSnake(snake, key);
	snake->tail->next = newPart;
	newPart->prev = snake->tail;
	newPart->next = NULL;
	snake->tail = newPart;
	snake->length++;
}

Snake* createSnake (){

	Snake *snake = malloc (sizeof(Snake));
	snake->head = snake->tail = newSnakePart();

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