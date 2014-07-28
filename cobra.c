#include "cobra.h"
#include <stdlib.h>

Food* createFood (){
	
	Food *food =  malloc (sizeof(Food));
	return food;

}

SnakePart* newSnakePart (){

	SnakePart *part = malloc (sizeof (SnakePart));
	return part;
}

void snakeIncrease (Snake *snake){

	SnakePart *newPart = newSnakePart();
	newPart->prev = snake->tail;
	snake->tail = newPart;
	newPart->next = NULL;
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