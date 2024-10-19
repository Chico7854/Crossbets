#ifndef BACCARAT_H
#define BACCARAT_H

#include "../common.h"

#define MAX_HAND_BACCARAT 3      //numero maximo de cartas na mão

static void draw (int* hand, int* count);      //compra carta
static void print_hand(int* hand, int* count);     //printa mao
static int hand_score (int* hand, int* count);     //valor da mao
void start_baccarat (int* currency);        //simula baccarat


#endif 