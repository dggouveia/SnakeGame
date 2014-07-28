#include <ncurses.h>
#include <string.h>
#include "tela.h"
#include <time.h>
#include "cobra.h"

#define WIDTH 65								//tamanho da janela
#define HEIGTH 20								//tamanho da janela
#define POS_FIELD_X 5 							//onde começa o campo
#define POS_FIELD_Y 10 							//onde começa o campo

WINDOW *field;									//cria um ponteiro para o tipo WINDOW definido na biblioteca NCURSES

void init_curses()
{
    initscr();
    start_color();
    init_pair(1,COLOR_BLACK,COLOR_CYAN);
    init_pair(2,COLOR_YELLOW,COLOR_BLACK);
    init_pair(3,COLOR_WHITE,COLOR_BLACK);
    /* Cor da cobra */
    init_pair(4,COLOR_RED, COLOR_RED);
    /*Desabilita o cursor na tela*/
    curs_set(0);
    /*Não aparece caractere digitado*/
    noecho();
    /*Não espera ENTER para ler caractere*/
    cbreak();
    /*Lê caracteres especiais do teclado*/
    keypad(stdscr,TRUE);
}

void gameOver(){
	char msg[] = "Game Over!";
	mvwprintw(field,HEIGTH/2,WIDTH/2-strlen(msg)/2,"%s",msg);
	//mostra ranking/salva/sai/reinicia
}

void close(){
	delwin(field);
	endwin();
}

void drawScenario(Snake *snake, Food *food){
	SnakePart *part = snake->head;

	while (!part){
		attron(COLOR_PAIR(4));
		mvwprintw(field, part->line, part->col, " ");
		attroff(COLOR_PAIR(4));
		part = part->next;
	}

	mvwprintw(field, food->line, food->col, "X");
}

void draw_field(){
	init_curses();
	nodelay(field, true);
	bkgd(COLOR_PAIR(3));
	mvprintw(1,10,"ADS Snake\t\t\t\t by: Joao M. and Douglas G.");
	attron(COLOR_PAIR(2));
	mvprintw(3,15,"Uses as setas do seu teclado para movimentar a cobra!");
	attroff(COLOR_PAIR(2));
	field = subwin(stdscr,HEIGTH,WIDTH,POS_FIELD_X,POS_FIELD_Y);
	wbkgd(field,COLOR_PAIR(1));
	box(field,0,0);
	
}

void clearField(){
	wclear(field);
	box(field,0,0);
	wrefresh(field);

}

void refreshField(){
	wrefresh(field);
}

void moveSnake (Snake *snake, char key){
	SnakePart *part = snake->tail;

	while (!part->prev){
		part->line = part->prev->line;
		part->col  = part->prev->col;
		part 	   = part->prev;
	}

	if (key == 'w' || key == KEY_UP)
		part->line--;
	if (key == 's'  || key == KEY_DOWN)
		part->line++;
	if (key == 'a'  || key == KEY_LEFT)
		part->col--;
	if (key == 'd'  || key == KEY_RIGHT)
		part->col++;	

}

void moveFood (Food *food){
	
	/* O tempo alimenta a função randomica */
	srand (time(NULL));

	/* Os números gerads estarão entre as demarcações do cenário */
	food->line = (rand() % HEIGTH);
	food->col  = (rand() % WIDTH);
}

void playGame (){
	
	char key, keyNext;
	Snake *snake = createSnake();

	/*Criar método para retornar uma comida*/
	Food *food = createFood();

	while (key != 'p'){
		
		/*Devolve a quantidade de clocks da máquina desde o início do programa*/
		clock_t start = clock();
		
		mvwprintw(field, 4, 4, "%d", clock() - start);
		keyNext = getch();
		
		do{
			// if (keyNext == ERR)
			// 	keyNext = getch();
		}while (clock() - start < CLOCKS_PER_SEC / 2);

		key = keyNext != ERR ? keyNext : key;	

		mvprintw(1,1,"%d", CLOCKS_PER_SEC);
		/* Atualiza as posições das partes da cobra */
		moveSnake (snake, key);		
		

		drawScenario (snake, food);
		refreshField ();		

		if (snake->head->col == food->col && snake->head->line == food->line){
			/*incrementa a pontuação*/
			snakeIncrease(snake);
			moveFood (food);
		}

		if (snake->head->line <= 0 || snake->head->line >= HEIGTH || snake->head->col <= 0 || snake->head->col >= WIDTH)
			gameOver();

		{
			SnakePart *part = snake->head->next;
			while (!part){
				if (snake->head->line == part->line || snake->head->col == part->col)
					gameOver();
				part = part->next;
			}
		}
	}
}
