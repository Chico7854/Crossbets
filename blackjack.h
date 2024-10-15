#ifndef _CROSSBETS_
#define _CROSSBETS_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DECK_SIZE 52
#define MAX_HAND 11     //maximo de cartas que pode ter na mão

void draw (int* hand, int* hand_index);
int hand_score(int* hand, int* count);
void print_card (int card);
void print_hand (int* hand, int* count);
int start_game();


#endif