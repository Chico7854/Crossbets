#ifndef _CROSSBETS_
#define _CROSSBETS_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DECK_COUNT 6    //padrao cassinos
#define DECK_SIZE (DECK_COUNT * 52)   
#define MAX_HAND 11     //maximo de cartas que pode ter na mão

typedef struct {
    char *card;
    int value;
} Cards;

Cards* create_deck();
Cards* shuffle (Cards* deck);
Cards* create_hand();
int hand_score(Cards* hand, int count);
void draw (Cards* deck, Cards* hand, int* hand_index, int* deck_index);
void print (Cards* player, Cards* dealer, int* pcount);
void start_game();


#endif