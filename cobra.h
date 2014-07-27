#ifndef COBRA_H
#define COBRA_H

typedef struct snakepart SnakePart;
typedef struct snake Snake;

struct snakepart{
	int x,y;
	SnakePart *nextPart;
};

struct snake{
	SnakePart *head;
	SnakePart *tail;
	int length;
};

/*Inicializa uma snake*/
Snake createSnake(Snake c);

/*cria nova parte da snake*/
SnakePart newSnakePart();

/*adiciona uma nova parte à snake*/
void snakeIncrease(Snake c);

/*Destroi uma snake*/
void snakeDestroy(Snake c);


#endif