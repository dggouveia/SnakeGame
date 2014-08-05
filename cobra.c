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
	if (key == KEY_UP){
		newPart->line = snake->tail->line + 1;
		newPart->col  = snake->tail->col;
	}else if (key == KEY_DOWN){
		newPart->line = snake->tail->line - 1;
		newPart->col  = snake->tail->col;
	}else if (key == KEY_LEFT){
		newPart->col = snake->tail->col + 1;
		newPart->line  = snake->tail->line;
	}else if (key == KEY_RIGHT){
		newPart->col = snake->tail->col - 1;
		newPart->line  = snake->tail->line;
	}
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