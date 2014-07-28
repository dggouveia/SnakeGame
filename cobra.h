#ifndef COBRA_H
#define COBRA_H

typedef struct snakepart SnakePart;
typedef struct snake Snake;
typedef struct food Food;

struct snakepart{
	int col, line;
	SnakePart *next;
	SnakePart *prev;
};

struct food {
	int col, line;
};

struct snake{
	SnakePart *head;
	SnakePart *tail;
	int length;
};

Food* createFood ();

/*Inicializa uma snake*/
Snake* createSnake();

/*cria nova parte da snake*/
SnakePart* newSnakePart();

/*adiciona uma nova parte à snake*/
void snakeIncrease(Snake *snake);

/*Destroi uma snake*/
void snakeDestroy(Snake *snake);

//void snakeReverse (Snake c);


#endif