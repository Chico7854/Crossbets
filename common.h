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
#define MEMORY_ERROR -2

typedef enum {      //cartas que para possuem valores diferente das faces
    A = 1,
    J = 11,
    Q,
    K
} SpecialCard;

int multiple_choice_numbers(int min, int max);      //input para pergunta de numeros multiplos como resposta
int scanf_num ();        //scanf para numeros
char scanf_sn ();       //scanf para perguntas de sim ou nao
int scanf_bet();        //scanf para as apostas
int initialize_hands(int** player_hand, int** dealer_hand, int** player_card_count, int** dealer_card_count, int max_hand);     //inicializa alguns jogos com baralho
void draw (int* hand, int* count);      //compra carta
void print_card(int card);      //printa carta
void print_hand (int* hand, int* count, int total);     //printa mao

#endif