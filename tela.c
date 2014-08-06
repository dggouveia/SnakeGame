#include <ncurses.h>
#include <string.h>
#include "tela.h"
#include <time.h>
#include "cobra.h"
#include <stdlib.h>
#include "menu.h"

#define WIDTH 65								//tamanho da janela
#define HEIGTH 20								//tamanho da janela
#define POS_FIELD_X 5 							//onde começa o campo
#define POS_FIELD_Y 10 							//onde começa o campo
#define TIME (CLOCKS_PER_SEC / 5)

WINDOW *field; //cria um ponteiro para o tipo WINDOW definido na biblioteca NCURSES
ITEM **items; //vetor de items
MENU *menu; //menu
ITEM *cur_item; //item selecionado
WINDOW *menu_window; //janela que irá conter o menu


void init_curses()
{
    initscr();
    start_color();
    init_pair(1,COLOR_BLACK,COLOR_CYAN);
    init_pair(2,COLOR_YELLOW,COLOR_BLACK);
    init_pair(3,COLOR_WHITE,COLOR_BLACK);
    /* Cor da cobra */
    init_pair(4,COLOR_BLACK, COLOR_BLACK);
    /*Desabilita o cursor na tela*/
    curs_set(0);
    nodelay(stdscr, true);
    /*Não aparece caractere digitado*/
    noecho();
    /*Não espera ENTER para ler caractere*/
    cbreak();
    /*Lê caracteres especiais do teclado*/
    keypad(stdscr,TRUE);
}

int initMenu(){
	int i;
	items = (ITEM **)calloc(4, sizeof(ITEM *));
	if(!items)
		return 1;

	for(i=0;i<4;++i)
		items[i] = (ITEM*)new_item(" ", choices[i]);

	menu = new_menu((ITEM **)items);
	set_menu_spacing(menu,-1,1,1);
}

void initWinMenu(){
	menu_window = subwin(field,WMENU_HEIGHT,WMENU_WIDTH,WMENU_X,WMENU_Y);
	keypad(menu_window, TRUE);
	wbkgd(menu_window,COLOR_PAIR(3));
	set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, WMENU_HEIGHT-1, WMENU_WIDTH-1, 1, 1));
    set_menu_mark(menu,">");
	box(menu_window,0,0);
	post_menu(menu);
} 

void exitMenu(){
	int i;
	/*exclui menu*/	
	unpost_menu(menu);

	/*desaloca os items do menu*/
	for(i=0;choices[i];i++)
		free_item(items[i]);	
	/*desaloca o menu*/
	free_menu(menu);

	/*deleta a janela do menu*/
	delwin(menu_window);

	/*Limpa a tela*/
	clearField();

}

void readkeyMenu(){
	int key;
	do
	{
		key = wgetch(menu_window);
	   	switch(key)
	    {	
	    	case KEY_DOWN:
		        menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
		}

		if(key == 10){
			cur_item = current_item(menu);
			if (cur_item == items[ITEM_1])
			{
				playGame();
				break;
			}
			else if (cur_item == items[ITEM_4])
			{
				exitMenu();
				close();
				exit(0);
			}

			
		}
	}while(true);

}

void openMenu(){
	initMenu();
	initWinMenu();
	refresh();
	readkeyMenu();
}


void gameOver(Snake *snake){
	char msg[] = "Game Over!";
	snakeDestroy(snake);
	nodelay(stdscr, false);
	clearField();
	mvwprintw(field,HEIGTH/2,WIDTH/2-strlen(msg)/2,"%s",msg);
	refreshField();
	getch();
	nodelay(stdscr, true);
	openMenu();

	//mostra ranking/salva/sai/reinicia
}

void close(){
	delwin(field);
	endwin();
}

void drawScenario(Snake *snake, Food *food){
	SnakePart *part = snake->head;

	while (part){
		wattron(field, COLOR_PAIR(4));
		mvwprintw(field, part->line, part->col, "O");
		wattroff(field, COLOR_PAIR(4));
		part = part->next;
	}
	mvwprintw (stdscr, 1, 1, "line = %d / col = %d", food->line, food->col);
	mvwprintw(field, food->line, food->col, "X");
}

void draw_field(){
	init_curses();
	bkgd(COLOR_PAIR(3));
	mvprintw(1,10,"ADS Snake\t\t\t\t by: Joao M. and Douglas G.");
	wattron(field, COLOR_PAIR(2));
	mvprintw(3,15,"Uses as setas do seu teclado para movimentar a cobra!");
	wattroff(field, COLOR_PAIR(2));
	field = subwin(stdscr,HEIGTH,WIDTH,POS_FIELD_X,POS_FIELD_Y);
	nodelay(field, true);
	curs_set(0);
    noecho();
    cbreak();
    keypad(field,TRUE);
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

void moveSnake (Snake *snake, int key){
	SnakePart *part = snake->tail;

	while (part->prev){
		part->line = part->prev->line;
		part->col  = part->prev->col;
		part 	   = part->prev;
	}

	if (key == KEY_UP)
		part->line--;
	else if (key == KEY_DOWN)
		part->line++;
	else if (key == KEY_LEFT)
		part->col--;
	else if (key == KEY_RIGHT)
		part->col++;
}


void moveFood (Food *food){
	
	/* O tempo alimenta a função randomica */
	srand (time(NULL));

	/* Os números gerads estarão entre as demarcações do cenário */
	food->line =  ((rand() % (HEIGTH-2)+1));
	
	food->col  =  ((rand() % (WIDTH-2))+1);
	
}

void playGame (){
	
	int key, keyNext, score = 0;
	SnakePart *part;
	Snake *snake = createSnake();
	snake->head->line = HEIGTH/2;
	snake->head->col  = WIDTH/2;
	Food *food = createFood();
	moveFood(food);
	drawScenario(snake, food);

	for(;;){
		
		clock_t start = clock();
		clearField ();		
		drawScenario (snake, food);
		refreshField();
		nodelay(field, true);

		keyNext = getch();

		do{
			if (keyNext == ERR)
				keyNext = getch();
		}while (clock() - start < TIME - score*1000);

		if (keyNext == (int)' '){
			snakeReverse(snake);
			if (key == KEY_UP)
				key = KEY_DOWN;
			else if (key == KEY_DOWN)
				key = KEY_UP;
			else if (key == KEY_LEFT)
				key = KEY_RIGHT;
			else if (key == KEY_RIGHT)
				key = KEY_LEFT;
		}else key = keyNext != ERR ? keyNext : key;	

		moveSnake (snake, key);
		
		if (snake->head->col == food->col && snake->head->line == food->line){
			score += 5;
			snakeIncrease(snake, key);
			moveFood (food);
		}else if (snake->head->line <= 0 || snake->head->line >= HEIGTH-1 || snake->head->col <= 0 || snake->head->col >= WIDTH-1 ){
			gameOver(snake);
			break;
		}
		else{	
			part = snake->head->next;
			while (part){
				if (snake->head->line == part->line && snake->head->col == part->col)
					gameOver(snake);
				part = part->next;
			}
		}

	}
}
