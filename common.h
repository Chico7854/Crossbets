#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

#define INVALID -1

int scanf_num ();        //scanf para numeros
char scanf_sn ();       //scanf para perguntas de sim ou nao
int scanf_bet();        //scanf para as apostas
int initialize_hands(int** player_hand, int** dealer_hand, int** player_card_count, int** dealer_card_count, int max_hand);     //inicializa alguns jogos com baralho

#endif