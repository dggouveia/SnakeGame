#include "cobra.h"
#include "tela.h"
#include <stdio.h>


void main (void){
	
	init_curses();
	printf("aqui\n");
	draw_field();
	printf("aqui\n");
	playGame();
	close();
}