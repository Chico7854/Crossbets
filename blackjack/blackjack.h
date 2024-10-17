#ifndef _CROSSBETS_
#define _CROSSBETS_

#include "../common.h"

#define DECK_SIZE 52
#define MAX_HAND 11     //maximo de cartas que pode ter na mão

void draw (int* hand, int* hand_index);             //compra carta
int hand_score(int* hand, int* count);              //calcula valor da mao
void print_card (int card);                         //printa carta unica
void print_hand (int* hand, int* count);            //printa mao
void update_currency(int* currency, int bet_value, int result);      //update no saldo
void start_game(int* currency);                                   //simula jogo

#endif