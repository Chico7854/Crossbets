#include "common.h"

//scanf para pergunta de sim ou nao
char scanf_sn () {
    char choice;
    
    while (1) {     //verifica se input é valido
        int temp = scanf (" %c", &choice);
        while (getchar() != '\n');

        if (temp == 1) {
            choice = toupper(choice);
            if (choice == 'S' || choice == 'N') {
                break;      //input valido
            }
        }
        printf ("\nComando inválido. Digite [S] ou[N].\n\nDigite aqui: ");
    }

    return choice;
}

//scanf para receber apostas
int scanf_bet(int* currency) {
    int bet_value;

    printf ("\nSaldo atual: R$%d\n", *currency);
    printf ("Digite o valor da aposta.\n\nDigite aqui: R$");

    while (1) {     //verifica input
        int temp = scanf ("%d", &bet_value);
        while (getchar() != '\n');      //limpa buffer

        if (temp == 1) {        //caso o imput esteja correto
            if (bet_value > 0 && bet_value <= *currency) {
                break;
            }
        }
        
        printf ("Comando inválido. Tente novamente.\n\nDigite aqui: ");
    }

    return bet_value;
}
