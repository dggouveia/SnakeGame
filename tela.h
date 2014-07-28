#ifndef TELA_h
#define TELA_h
#include <ncurses.h>
#include <string.h>

#define FOOD '*'
#define SNAKE 'o'

 /*Inicialia ncurses*/
void init_curses();

/*Fim de jogo*/
void gameOver();

/*Encerra ncurses*/
void close();

/*Desenha parte da cobra, recebe uma parte da cobra que contem sua posição salva
line e col sao demonstrativas*/
void drawSnakePart(int line, int col);

/*Desenha o campo*/
void draw_field();

/*Atualiza o campo*/
void refreshField();

/*Limpa completamente o campo*/
void clearField();

/*Jogo (lê comandos e move a cobra)*/
void playGame ();

#endif