#ifndef _MENU_H
#define _MENU_H
#include <menu.h>

#define WMENU_WIDTH 18 //Largura da janela de menu
#define WMENU_HEIGHT 6 //Alrura da janela de menu
#define WMENU_X 18 //Posição x da janela
#define WMENU_Y 29 //posição y da janela

enum options{
	ITEM_1, ITEM_2, ITEM_3, ITEM_4
};

/*Descrĩção dos item do menu principal*/
char *menuChoices[] =  {
	"Play", //ITEM_1
	"Ranking", //ITEM_2
	"Select level", //ITEM_3
	"exit", //ITEM_4
	NULL
};

/*Descrĩção dos item do menu Level*/
char *levelChoices[] =  {
	"Easy", //ITEM_1
	"Normal", //ITEM_2
	"hard", //ITEM_3
	"Back", //ITEM_4
	NULL
};

/*Abre o menu*/
void openMenu();

void openLevelMenu();

/*inicializa o menu*/
int initMainMenu();

/*inicializa o menu de dificuldade*/
int initLevelMenu();

/*inicializa a janela*/
void initWinMenu(MENU *menu); 

/*encerra o menu*/
void exitMenu(MENU *menu, char** choices); 

/*ler teclado*/
void readkeyMenu();

void readkeyLevelMenu();

void showCurrentDificulty();

#endif