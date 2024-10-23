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
#define MEMORY_ERROR -2     //erro de alocacao de memoria

typedef enum {      //cartas que para possuem valores diferente das faces
    A = 1,
    J = 11,
    Q,
    K
} SpecialCard;

char* trim_space(char* str);        //apaga espaços no começo e fim da string
int scanf_num(int min, int max);      //scanf para numeros
char scanf_sn ();       //scanf para perguntas de sim ou nao
int scanf_bet(int* currency);        //scanf para as apostas
int initialize_hands(int** player_hand, int** dealer_hand, int** player_card_count, int** dealer_card_count, int max_hand);     //inicializa alguns jogos com baralho
void draw (int* hand, int* count);      //compra carta
void print_card(int card);      //printa carta
void print_hand (int* hand, int* count, int total);     //printa mao

#endif