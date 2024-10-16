#ifndef _ROULETTE_
#define _ROULETTE_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_BETS 46     //numero máximo de aposta

typedef struct {
    int type;
    int number;
    int value;
} Bet;

void player_bet (Bet* bet, int bet_count, int* currency);
int bet_value (Bet* bet, int bet_count, int* currency);
void waiting_message(int result);
void check_bets(Bet* bets, int bet_count, int result, int* currency);
int in_array (int* array, int number, int size);
void print_lines();
void start_game(int* currency);

#endif