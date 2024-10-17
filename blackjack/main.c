#include "blackjack.h"

int main() {
    srand(time(NULL));
    
    int* currency = (int *)malloc(sizeof(int));
    *currency = 10000;

    start_game(currency);
}