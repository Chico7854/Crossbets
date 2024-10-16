#ifndef _CROSSBETS_
#define _CROSSBETS_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DECK_SIZE 52
#define MAX_HAND 11     //maximo de cartas que pode ter na mão

void draw (int* hand, int* hand_index);             //compra carta
int hand_score(int* hand, int* count);              //calcula valor da mao
void print_card (int card);                         //printa carta unica
void print_hand (int* hand, int* count);            //printa mao
int start_game();                                   //simula jogo

#endif