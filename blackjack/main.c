#include "blackjack.h"

int main() {
    srand(time(NULL));
    
    int currency = 10000;                       //saldo
    int bet = -1;                               //aposta da rodada e atribui -1 pra poder validar scanf

    printf ("Saldo: R$%d\n", currency);         //printa saldo
    printf ("Quanto vai apostar?\n");
    while (bet <= 0 || bet > currency) {
        printf ("R$");
        scanf ("%d", &bet);                     //input aposta
    }

    int result = start_game();                  //simula o jogo e guarda resultado

    currency += result * bet;                   //atualiza saldo

    printf ("Saldo: R$%d\n", currency);         //printa saldo
}