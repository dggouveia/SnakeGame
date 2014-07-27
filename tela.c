#include <ncurses.h>
#include <string.h>
#include "tela.h"

#define WIDTH 65
#define HEIGTH 20
#define POS_FIELD_X 5
#define POS_FIELD_Y 10

WINDOW *field;

void init_curses()
{
    initscr();
    start_color();
    init_pair(1,COLOR_BLACK,COLOR_CYAN);
    init_pair(2,COLOR_YELLOW,COLOR_BLACK);
    init_pair(3,COLOR_WHITE,COLOR_BLACK);
    curs_set(0);
    noecho();
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

void drawSnakePart(int line, int col){
	if(line<HEIGTH && col<WIDTH){
		wattron(field,A_BOLD);
		mvwprintw(field,line,col,"%c",SNAKE);
		wattroff(field,A_BOLD);
	}else{
		gameOver();
	}
}

void draw_field(){
	init_curses();
	bkgd(COLOR_PAIR(3));
	mvprintw(1,10,"ADS Snake\t\t\t\t by: Joao M. and Douglas G.");
	attron(COLOR_PAIR(2));
	mvprintw(3,15,"Uses as setas do seu teclado para movimentar a cobra!");
	attroff(COLOR_PAIR(2));
	field = subwin(stdscr,HEIGTH,WIDTH,POS_FIELD_X,POS_FIELD_Y);
	wbkgd(field,COLOR_PAIR(1));
	box(field,0,0);
	
}

//main temporario para testes
int main(int argc, char const *argv[])
{
	
	draw_field();
	getch();
	gameOver();
	wrefresh(field);
	getch();
	close();

}