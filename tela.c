#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include "tela.h"
#include <time.h>
#include "cobra.h"
#include <stdlib.h>
#include "menu.h"
#include "ranking.h"

#define WIDTH 65								//tamanho da janela
#define HEIGTH 20								//tamanho da janela
#define WSCORE_Y 71								//posição do score
#define POS_FIELD_X 12 							//onde começa o campo
#define POS_FIELD_Y 5 							//onde começa o campo

/*dificuldade*/
#define HARD (CLOCKS_PER_SEC / 15)
#define SCOREHARD 10				
#define NORMAL (CLOCKS_PER_SEC / 10)
#define SCORENORMAL 5
#define EASY (CLOCKS_PER_SEC / 7)
#define SCOREEASY 2

WINDOW *field; //cria um ponteiro para o tipo WINDOW definido na biblioteca NCURSES
ITEM **items; //vetor de items
MENU *mainMenu;
MENU *levelMenu; //menus
ITEM *cur_item; //item selecionado
WINDOW *menu_window, *score_window; //janela que irá conter o menu e janela do score
WINDOW *ranking_window; //janela para ranking

char namePlayer[4];
unsigned int dificulty = NORMAL;
unsigned int addScore  = SCORENORMAL;

void init_curses()
{
    initscr();
    start_color();
    init_pair(1,COLOR_BLACK,COLOR_CYAN);
    /*cor da comida*/
    init_pair(2,COLOR_RED,COLOR_RED);
    init_pair(3,COLOR_WHITE,COLOR_BLACK);
    /* Cor da cobra */
    init_pair(4,COLOR_BLACK, COLOR_BLACK);
    init_pair(5,COLOR_CYAN, COLOR_BLACK);
    init_pair(6,COLOR_BLACK, COLOR_WHITE);
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

int initMainMenu(){
	int i;
	items = (ITEM **)calloc(4, sizeof(ITEM *));
	if(!items)
		return 1;

	for(i=0;i<4;++i)
		items[i] = (ITEM*)new_item(" ", menuChoices[i]);

	mainMenu  = new_menu((ITEM **)items);
	set_menu_spacing(mainMenu,-1,1,1);

	return 0;
}

int initlevelMenu(){
	int i;
	items = (ITEM **)calloc(4, sizeof(ITEM *));
	if(!items)
		return 1;

	for(i=0;i<4;++i)
		items[i] = (ITEM*)new_item(" ", levelChoices[i]);

	levelMenu  = new_menu((ITEM **)items);
	set_menu_spacing(levelMenu,-1,1,1);

	return 0;
}

void initWinRanking(){
	exitMenu(mainMenu, menuChoices);
	ranking_window = subwin(field,WMENU_HEIGHT*2+1,WMENU_WIDTH,WMENU_X-3,WMENU_Y);
	wbkgd(ranking_window,COLOR_PAIR(3));
	box(ranking_window,0,0);

	Ranking *rank = getRanking();
	
	if (rank->lenght > 0)
	{
		printRanking(rank, ranking_window);
		writeRanking(rank);
		rankingDestroy(rank);
	}else{
		mvwprintw(ranking_window,2,6,"Play now");
	}

	box(ranking_window,0,0);

	wgetch(ranking_window);
	delwin(ranking_window);	
	clearField();
	openMenu();	

}

void initWinMenu(MENU *menu){
	menu_window = subwin(field,WMENU_HEIGHT,WMENU_WIDTH,WMENU_X,WMENU_Y);
	keypad(menu_window, TRUE);
	wbkgd(menu_window,COLOR_PAIR(3));
	set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, WMENU_HEIGHT-1, WMENU_WIDTH-1, 1, 1));
    set_menu_mark(menu,">");
	box(menu_window,0,0);
	post_menu(menu);
} 

void showCurrentDificulty(){
	if(dificulty == EASY){
		mvwprintw(menu_window, 1,14,"<<");
		mvwprintw(menu_window, 2,14,"  ");
		mvwprintw(menu_window, 3,14,"  ");
		wrefresh(menu_window);
	}else if(dificulty == NORMAL){
		mvwprintw(menu_window, 1,14,"  ");
		mvwprintw(menu_window, 2,14,"<<");
		mvwprintw(menu_window, 3,14,"  ");
		wrefresh(menu_window);
	}else{
		mvwprintw(menu_window, 1,14,"  ");
		mvwprintw(menu_window, 2,14,"  ");
		mvwprintw(menu_window, 3,14,"<<");
		wrefresh(menu_window);
	}
}

void scoreWindow(){
	score_window = newwin(WMENU_HEIGHT-2,WMENU_WIDTH,POS_FIELD_X,WSCORE_Y);
	wbkgd(score_window,COLOR_PAIR(6));
	box(score_window,0,0);
	mvwprintw(score_window,1,(WMENU_WIDTH-strlen("Score:"))/2, "%s","Score:");
	refresh();
}

void updateScore(int score){
	mvwprintw(score_window,2,(WMENU_WIDTH/2-strlen("Score:")), "%d",score);
	box(score_window,0,0);
	wrefresh(score_window);
}

void exitMenu(MENU *menu, char **choices){
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
		        menu_driver(mainMenu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(mainMenu, REQ_UP_ITEM);
				break;
		}

		if(key == 10){
			cur_item = current_item(mainMenu);

			if (cur_item == items[ITEM_1])
			{
				nodelay(stdscr, true);
				playGame(dificulty);
			}
			else if(cur_item == items[ITEM_2]){
				initWinRanking();
			}
			else if (cur_item == items[ITEM_3])
			{
				exitMenu(mainMenu, menuChoices);
				openLevelMenu();
				readkeyLevelMenu();
			}
			else
			{
				exitMenu(mainMenu, menuChoices);
				close();
				exit(0);
			}			
		}
	}while(true);
}


void readkeyLevelMenu(){
	int key;
	do
	{
		key = wgetch(menu_window);
	   	switch(key)
	    {	
	    	case KEY_DOWN:
		        menu_driver(levelMenu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(levelMenu, REQ_UP_ITEM);
				break;
		}

		if(key == 10){
			cur_item = current_item(levelMenu);

			if (cur_item == items[ITEM_1])
			{
				dificulty = EASY;
				addScore  = SCOREEASY;
				showCurrentDificulty();
			}
			else if(cur_item == items[ITEM_2])
			{
				dificulty = NORMAL;
				addScore  = SCORENORMAL;
				showCurrentDificulty();
			}
			else if (cur_item == items[ITEM_3])
			{
				dificulty = HARD;
				addScore  = SCOREHARD;
				showCurrentDificulty();
			}
			else
			{
				exitMenu(levelMenu, levelChoices);
				openMenu();
			}			
		}
	}while(true);

}



void openMenu(){
	initMainMenu();
	initWinMenu(mainMenu);
	refresh();
	readkeyMenu();
}

void openLevelMenu(){
	initlevelMenu();
	initWinMenu(levelMenu);
	showCurrentDificulty();
	refresh();
}

void getName(char *newName){	
	curs_set(1);
	echo();
	nodelay(stdscr, TRUE);
	menu_window = subwin(field,WMENU_HEIGHT,WMENU_WIDTH,WMENU_X,WMENU_Y);
	clearField();
	wbkgd(menu_window,COLOR_PAIR(3));
	mvwprintw(menu_window,2,2,"Your name: ");
	box(menu_window,0,0);
	refresh();
	nocbreak();
	keypad(menu_window, FALSE);
	wscanw(menu_window,"%c%c%c",&newName[0],&newName[1],&newName[2]);
	int i;
	for (i = 0; i<3; ++i)
		newName[i] = (char) toupper(newName[i]);
	cbreak();
	curs_set(0);
	noecho();
	delwin(menu_window);
	clearField();
}


void gameOver(Snake *snake, int score){
	delwin(score_window);
	char msg[] = "Game Over!";
	snakeDestroy(snake);
	nodelay(stdscr, false);
	clearField();
	mvwprintw(field,HEIGTH/2,WIDTH/2-strlen(msg)/2,"%s",msg);
	refreshField();
	insertOnRanking(score);
	getch();
	nodelay(stdscr, true);
	openMenu(mainMenu);

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
	wattron(field, COLOR_PAIR(2));
	mvwprintw(field, food->line, food->col, "X");
	wattroff(field, COLOR_PAIR(2));
	scoreWindow();
}

void draw_field(){
	init_curses();
	bkgd(COLOR_PAIR(3));
	wattron(stdscr, COLOR_PAIR(5));
	mvprintw(0, 0,"%s%s%s%s%s%s%s%s%s%s%s", "         ___           ___           ___           ___           ___              \n",
"        /\\  \\         /\\__\\         /\\  \\         /\\__\\         /\\  \\             \n",
"       /::\\  \\       /::|  |       /::\\  \\       /:/  /        /::\\  \\            \n",
"      /:/\\ \\  \\     /:|:|  |      /:/\\:\\  \\     /:/__/        /:/\\:\\  \\           \n",
"     _\\:\\~\\ \\  \\   /:/|:|  |__   /::\\~\\:\\  \\   /::\\__\\____   /::\\~\\:\\  \\          \n",
"    /\\ \\:\\ \\ \\__\\ /:/ |:| /\\__\\ /:/\\:\\ \\:\\__\\ /:/\\:::::\\__\\ /:/\\:\\ \\:\\__\\         \n",
"    \\:\\ \\:\\ \\/__/ \\/__|:|/:/  / \\/__\\:\\/:/  / \\/_|:|~~|~    \\:\\~\\:\\ \\/__/         \n",
"     \\:\\ \\:\\__\\       |:/:/  /       \\::/  /     |:|  |      \\:\\ \\:\\__\\           \n",
"      \\:\\/:/  /       |::/  /        /:/  /      |:|  |       \\:\\ \\/__/           \n",
"       \\::/  /        /:/  /        /:/  /       |:|  |        \\:\\__\\             \n",
"        \\/__/         \\/__/         \\/__/         \\|__|         \\/__/             ");
	wattroff(stdscr, COLOR_PAIR(5));
	mvprintw (32, 38, "by Douglas Gabriel & João Marcos");
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
	SnakePart *part = snake->tail; //isso aqui nao ta precisando

	if (snake->tail->prev){
		SnakePart *part = snake->tail;
		snake->tail = snake->tail->prev;
		snake->tail->next = NULL;
		part->prev = NULL;
		part->next = snake->head;
		snake->head->prev = part;
		part->line = snake->head->line;
		part->col = snake->head->col;
		snake->head = part;		
	}
	if (key == KEY_UP)
		snake->head->line--;
	else if (key == KEY_DOWN)
		snake->head->line++;
	else if (key == KEY_LEFT)
		snake->head->col--;
	else if (key == KEY_RIGHT)
		snake->head->col++;
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
	snake->tail->line = snake->head->line;
	snake->tail->col  = snake->head->col + 1;
	Food *food = createFood();
	moveFood(food);
	drawScenario(snake, food);
	updateScore(score);
	key = KEY_RIGHT;
	cbreak();
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
		}while (clock() - start < dificulty);

		if (keyNext == (int)' '){
			snakeReverse(snake);
			if (snake->head->line > snake->head->next->line)
				key = KEY_DOWN;
			else if (snake->head->line < snake->head->next->line)
				key = KEY_UP;
			else if (snake->head->col > snake->head->next->col)
				key = KEY_RIGHT;
			else if (snake->head->col < snake->head->next->col)
				key = KEY_LEFT;
		}else if(!(key == KEY_UP && keyNext == KEY_DOWN) && !(key == KEY_DOWN && keyNext == KEY_UP) && !(key == KEY_LEFT && keyNext == KEY_RIGHT) && !(key == KEY_RIGHT && keyNext == KEY_LEFT))
			key = keyNext != ERR ? keyNext : key;	

		moveSnake (snake, key);
		
		if (snake->head->col == food->col && snake->head->line == food->line){
			score += addScore;
			snakeIncrease(snake); 
			moveFood (food);
			updateScore(score);
		}else if (snake->head->line <= 0 || snake->head->line >= HEIGTH-1 || snake->head->col <= 0 || snake->head->col >= WIDTH-1 ){
			gameOver(snake, score);
			break;
		}
		else{	
			part = snake->head->next;
			while (part){
				if (snake->head->line == part->line && snake->head->col == part->col)
					gameOver(snake,score);
				part = part->next;
			}
		}

	}
}
