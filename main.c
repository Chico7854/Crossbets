#include "common.h"
#include "blackjack/blackjack.h"
#include "roulette/roulette.h"
#include "baccarat/baccarat.h"

typedef enum {
    BLACKJACK = 1,
    ROULETTE,       //2
    BACCARAT        //3
} Game;

typedef enum {
    POBRE = 1,
    PLAYAGAIN_SIM,
    PLAYAGAIN_NAO
} PlayAgain;

int game_selection();        //simula interface de escolha de jogo
int play_again(int* currency, int game_choice);     //verifica saldo e pergunta para player se quer continuar jogando jogo


int main () {
    srand(time(NULL));

    int* currency = (int *)malloc(sizeof(int));      //saldo

    if (currency == NULL) {
        printf ("Alocação de memória falhou.\n");
        return 1;
    }

    *currency = 1000000;            //1 milhao de saldo

    printf ("\nBem Vind@ ao Cassino Crossbets!\n\nSeu saldo inicial é de R$1.000.000.\n");

    PlayAgain game_result = PLAYAGAIN_SIM;

    while (game_result == PLAYAGAIN_SIM) {
        Game game_choice = game_selection();

        if (game_choice == BLACKJACK) {
            printf ("\nBem vind@ ao Blackjack da Crossbets!\n");
 
            while (game_result == PLAYAGAIN_SIM) {
                if (start_blackjack(currency) == MEMORY_ERROR) {
                    return 1;
                }

                game_result = play_again(currency, game_choice);
            }
        }
        else if (game_choice == ROULETTE) {
            printf ("\nBem Vind@ a Roleta da CrossBets!\n");
            
            while (game_result == PLAYAGAIN_SIM) {
                if(start_roulette(currency) == MEMORY_ERROR) {
                    return 1;
                }

                game_result = play_again(currency, game_choice);     
            }
        }
        else if (game_choice == BACCARAT) {
            printf ("\nBem Vind@ ao Baccarat Punto Banco da CrossBets!\n");
            
            while (game_result == PLAYAGAIN_SIM) {
                if(start_baccarat(currency) == MEMORY_ERROR) {
                    return 1;
                }

                game_result = play_again(currency, game_choice);     
            }
        }

        //verifica se player nao quer continuar jogando
        if (game_result == PLAYAGAIN_NAO) {
            printf ("\nDeseja jogar outro jogo? ");
            
            char choice = scanf_sn();       //input escolha do player

            if (choice == 'S') {        //se player quiser jogar outro jogo
                game_result = PLAYAGAIN_SIM;
            }
        }
    }

    printf ("\nSaldo final: R$%d\n\nObrigado por jogar no Cassino CrossBets. =D\n\n", *currency);

    free (currency);

    return 0;
}

//interface de escolha de jogo
int game_selection() {
    printf ("Escolha qual jogo você deseja jogar.\n\n");
    
    for (int i = 0; i < 15; i++) {      //printa interface
        printf ("-");
    }

    printf ("\n[1] Blackjack\n[2] Roleta\n[3] Baccarat\n");

    for (int i = 0; i < 15; i++) {      //printa interface
        printf ("-");
    }

    printf ("\n\nDigite sua escolha: ");

    int player_choice = scanf_num(1, 3);
}

//verifica saldo e pergunta para player se quer continuar jogando jogo
int play_again (int* currency, int game_choice) {
    sleep(1); 

    if (*currency == 0) {       //se zerou saldo
        printf ("\nVocê perdeu todo seu dinheiro.\n");

        return POBRE;       //retorna POBRE se saldo ta zerado
    }

    char game[10];

    if (game_choice == BLACKJACK) {
        strcpy (game, "Blackjack");
    }
    else if (game_choice == ROULETTE) {
        strcpy (game, "Roleta");
    }
    else {
        strcpy (game, "Baccarat");
    }
    printf ("\nSaldo atual: R$%d\n\nDeseja jogar %s novamente? ", *currency, game);

    char choice = scanf_sn();       //input escolha do player

    if (choice == 'S') {         //jogar novamente
        return PLAYAGAIN_SIM;
    }
    else {      //parar de jogar o jogo
        return PLAYAGAIN_NAO;
    }
}