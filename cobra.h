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

Snake* createSnake();

SnakePart* newSnakePart();

void snakeIncrease(Snake *snake, int key);

void snakeDestroy(Snake *snake);

void snakeReverse (Snake *snake);


#endif