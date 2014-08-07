#ifndef _MENU_H
#define _MENU_H
#include <menu.h>

#define WMENU_WIDTH 18 //Largura da janela de menu
#define WMENU_HEIGHT 6 //Alrura da janela de menu
#define WMENU_X 11 //Posição x da janela
#define WMENU_Y 33 //posição y da janela


enum options{
	ITEM_1, ITEM_2, ITEM_3, ITEM_4
};

/*Descrĩção dos item do menu*/
char *choices[] =  {
	"Play",
	"Ranking",
	"Select level",
	"exit",
	NULL
};

/*Abre o menu*/
void openMenu();

/*inicializa o menu*/
int initMenu();

/*inicializa a janela*/
void initWinMenu(); 

/*encerra o menu*/
void exitMenu(); 

/*ler teclado*/
void readkeyMenu();

#endif