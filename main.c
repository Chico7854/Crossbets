#include "common.h"
#include "blackjack/blackjack.h"
#include "roulette/roulette.h"
#include "baccarat/baccarat.h"

//enumera os jogos
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
int play_again(int* currency, int game_choice);     //

int main () {
    srand(time(NULL));

    int* currency = (int *)malloc(sizeof(int));      //saldo

    if (currency == NULL) {     //verifica de malloc deu certo
        printf ("Alocação de memória falhou.\n");
        return 1;
    }

    *currency = 1000000;            //1 milhao de saldo

    printf ("\nBem Vind@ ao Cassino Crossbets!\n\nSeu saldo inicial é de R$1.000.000.\n");

    while (1) {     //loop até jogador querer sair
        Game game_choice = game_selection();      //escolha de jogo

        if (game_choice == BLACKJACK) {      //blackjack
            printf ("\nBem vind@ ao Blackjack da Crossbets!\n");
 
            while (1) {      //loop apenas para jogo
                if(start_blackjack(currency) == MEMORY_ERROR) {     //abre blackjack
                    return 1;       //se alocacao de memoria falhar
                }

                if (play_again(currency, game_choice) == POBRE) {
                    return 2;
                }
            }
        }
        else if (game_choice == ROULETTE) {     //roleta
            printf ("\nBem Vind@ a Roleta da CrossBets!\n");
            
            while (1) {      //loop apenas para jogo
                if(start_roulette(currency) == MEMORY_ERROR) {     //abre roleta
                    return 1;       //se alocacao de memoria falhar
                }

                if (play_again(currency, game_choice) == POBRE) {
                    return 2;
                }
            }
        }
        else {      //baccarat
            printf ("\nBem Vind@ ao Baccarat Punto Banco da CrossBets!\n");
            
            while (1) {      //loop apenas para jogo
                if(start_baccarat(currency) == MEMORY_ERROR) {     //abre blackjack
                    return 1;       //se alocacao de memoria falhar
                }

                if (play_again(currency, game_choice) == POBRE) {
                    return 2;
                }
            }
        }

        printf ("\nDeseja jogar outro jogo?");       //printa se player quer jogar outro jogo
        
        char choice = scanf_sn();       //input escolha do player

        if (choice == 'N') {        //se nao quiser jogar outro jogo
            break;      //quebra loop e sai do jogo
        }
    }

    printf ("\nSaldo final: R$%d\n\nObrigado por jogar no Cassino CrossBets. =D\n\n", *currency);

    free (currency);

    return 0;
}

int game_selection() {
    printf ("Escolha qual jogo você deseja jogar.\n\n");
    
    for (int i = 0; i < 15; i++) {      //printa interface
        printf ("-");
    }

    printf ("\n[1] Blackjack\n[2] Roleta\n[3] Baccarat\n");        //printa opcoes de jogos

    for (int i = 0; i < 15; i++) {      //printa interface
        printf ("-");
    }

    printf ("\n\nDigite sua escolha: ");

    int player_choice;

    while (1) {         //verificacao input
        player_choice = scanf_num();        //input escolha do jogo

        if (player_choice != INVALID) {        //verifica se é valido
            if (player_choice >= 1 && player_choice <= 3) {
                break;          //se input for valido
            }
        }

        printf ("\nComando inválido. Digite um número de [1] a [3].\n\nDigite aqui: ");
    }

    return player_choice;
}

int play_again (int* currency, int game_choice) {
    sleep(1); 

    if (*currency == 0) {       //se zerou saldo
        sleep(1);       //espera 1 segundo

        printf ("\nVocê perdeu todo seu dinheiro. Obrigado por jogar no Cassino Crossbets. =D\n\n");

        free(currency);     //desaloca memória

        return POBRE;       //retorna 0 se saldo ta zerado
    }

    char game[10];

    //
    if (game_choice == BLACKJACK) {
        strcpy (game, "Blackjack");
    }
    else if (game_choice == ROULETTE) {
        strcpy (game, "Roleta");
    }
    else {
        strcpy (game, "Baccarat");
    }
    printf ("\nSaldo atual: R$%d\n\nDeseja jogar %s novamente?", *currency, game);

    char choice = scanf_sn();       //input escolha do player

    if (choice == 'S') {         //jogar novamente
        return PLAYAGAIN_SIM;
    }
    else {      //parar de jogar o jogo
        return PLAYAGAIN_NAO;
    }
}