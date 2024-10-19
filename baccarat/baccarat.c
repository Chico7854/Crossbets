#include "baccarat.h"
//regras da wikipedia
/*
versao punto banco
bet
bet on pairs
deal cards
count score
draw third card
compare results
pay bet, if draw 9:1
*/

//compra cartas
static void draw (int* hand, int* count) {
    hand[*count] = (rand() % 10) + 1;       //valor maximo é 10, se tirar 10 depois ramdomiza qual carta é
    (*count) ++;
}

//printa mao
static void print_hand(int* hand, int* count) {
    for (int i = 0; i < *count; i++) {
        int carta = hand[i];       //carta sendo iterada

        if (carta == 1) {     //se carta for um A
            printf ("[A] ");
        }
        else if (carta == 10) {        //caso carta tem valor 10, ramdomiza qual carta vai ser, 10 ou J ou Q ou K
            int random = rand() % 4;

            if (random == 0) {
                printf ("[10] ");
            }
            else if (random == 1) {
                printf ("[J] ");
            }
            else if (random == 2) {
                printf ("[Q] ");
            }
            else {
                printf ("[K] ");
            }
        }
        else {
            printf ("[%d] ", carta);     //se carta for qlq outra apenas printa valor da carta   
        }
    }

    printf ("(Total: %d)\n", hand_score(hand, count));      //printa resultado total da mao
}

//calcula valor da mao
static int hand_score (int* hand, int* count) {
    int result = 0;

    for (int i = 0; i < *count; i ++) {
        result += hand[i];      //soma valor das cartas
    }

    return result % 10;     //retorna módulo de 10 do resultado, regra padrao de baccard
}



//simula baccarat
void start_baccarat (int* currency) {
    int* dealer_hand;
    int* player_hand;
    int* dealer_card_count;
    int* player_card_count;

    if (!initialize_hands(&player_hand, &dealer_hand, &player_card_count, &dealer_card_count, MAX_HAND_BACCARAT)) {
        return;
    }

    int player_total;       //valor da mao do player
    int dealer_total;        //valor da mao do dealer
    int player_choice;     //escolha do player em quem apostar

    //aposta
    int bet_value = scanf_bet(currency);        //valor da aposta

    printf ("\nEscolha qual aposta fazer:\n\n");
    printf ("[1] Vitória do Jogador\n[2] Vitória do Banco\n[3] Empate\n\nDigite aqui: ");      //print opcoes de aposta

    while (1) {     //verificacao de input
        player_choice = scanf_num();        //input escolha do jogador

        if (player_choice != INVALID) {
            if (player_choice >= 1 && player_choice <= 3) {
                break;      //input valido
            }
        }

        printf ("\nComando inválido. Digite um número de [1] a [3].\n\nDigite aqui: ");
    }
    
    //dealer compra 2 cartas
    draw(dealer_hand, dealer_card_count);
    draw(dealer_hand, dealer_card_count);
    //player compra 2 cartas
    draw(player_hand, player_card_count);
    draw(player_hand, player_card_count);

    //printa mao do player
    sleep(1);       //espera 1 segundo

    printf ("\nCartas do Jogador: ");
    print_hand(player_hand, player_card_count);
    
    //printa mao do dealer
    printf ("Cartas do Banco: ");
    print_hand(dealer_hand, dealer_card_count);

    //verifica se alguem tem natural baccard
    player_total = hand_score(player_hand, player_card_count);      //calcula total do player
    dealer_total = hand_score(dealer_hand, dealer_card_count);      //calcula total do dealer

    if (player_total < 8 && dealer_total < 8)  {        //se ambos NAO tiverem natural o jogo continua

        //verifica se player compra terceira carta
        if (player_total > 5) {     //player nao compra carta
            if (dealer_total < 6 ) {        //verifica se dealer compra terceira carta
                draw(dealer_hand, dealer_card_count);

                dealer_total = (hand_score(player_hand, player_card_count));        //atualiza total

                sleep(1);       //espera 1 segundo

                printf ("\nBanco comprou terceira carta: [%d] ", dealer_hand[*dealer_card_count - 1]);       //printa qual carta dealer comprou
                printf ("(Novo total: %d)\n", dealer_total);        //printa novo total
            }
        }
        else {     //player compra terceira carta
            draw (player_hand, player_card_count);

            player_total = hand_score(player_hand, player_card_count);      //atualiza total

            int player_third = player_hand[*player_card_count - 1];      //variavel guarda terceira carta do player

            sleep(1);       //espera 1 segundo

            printf ("\nPlayer comprou terceira carta: [%d] ", player_third);       //printa qual carta player comprou
            printf ("(Novo total: %d)\n", player_total);        //printa novo total

            //verifica se dealer compra terceira carta
            int drew_card = 0;      //variavel para verificar se carta foi comprada

            if (dealer_total <= 2) {
                draw(dealer_hand, dealer_card_count);       //compra terceira carta
                drew_card ++;       //atualiza variavel
            }
            else if (dealer_total == 3) {
                if (player_third != 8) {        //se terceira carta do player nao é 8, compra
                    draw(dealer_hand, dealer_card_count);
                    drew_card ++;
                }
            }
            else if (dealer_total == 4) {       
                if (player_third >= 2 && player_third <= 7) {       //se terceira carta do player ta entre 2-7, compra
                    draw(dealer_hand, dealer_card_count);
                    drew_card ++;
                }
            }
            else if (dealer_total == 5) {
                if (player_third >= 4 && player_third <= 7) {       //se terceira carta do player ta entre 4-7, compra
                    draw(dealer_hand, dealer_card_count);
                    drew_card ++;
                }
            }
            else if (dealer_total == 6) {
                if (player_third == 6 || player_third == 7) {       //se terceira carta do player é 6 ou 7, compra
                    draw(dealer_hand, dealer_card_count);
                    drew_card ++;
                }
            }

            if (drew_card == 1) {       //se dealer comprou carta
                dealer_total = (hand_score(dealer_hand, dealer_card_count));        //atualiza total

                sleep(1);       //espera 1 segundo

                printf ("\nBanco comprou terceira carta: [%d] ", dealer_hand[*dealer_card_count - 1]);       //printa qual carta dealer comprou

                printf ("(Novo total: %d)\n", dealer_total);        //printa novo total
            }
        }
    }

    //printa resultado final
    sleep(1);       //espera 1 segundo

    printf ("\nResultado Final:\n");

    printf ("Cartas do Jogador: ");     //printa resultado do player
    print_hand(player_hand, player_card_count);

    printf ("Cartas do Banco: ");     //printa resultado do dealer
    print_hand(dealer_hand, dealer_card_count);

    //printa quem ganhou
    int result;     //guarda resultado para comparar com aposta

    sleep(1);       //espera 1 segundo

    if (player_total > dealer_total) {
        printf ("\nJogador ganhou. ");
        result = 1;
    }
    else if (dealer_total > player_total) {
        printf ("\nBanco ganhou. ");
        result = 2;
    }
    else {
        printf ("\nJogador e Banco empataram. ");
        result = 3;
    }

    //verifica aposta
    int bet_result;     //guarda resultado da aposta, variavel para nao ter que ficar repetindo printf
    if (player_choice == 1) {      //apostou vitoria do player
        if (result == 1) {      //ganhou aposta
            bet_result = 1;
            *currency += bet_value;     //ajusta saldo
        }
        else {      //perdeu aposta
            bet_result = 0;
            *currency -= bet_value;     //ajusta saldo
        }
    }
    else if (player_choice == 2) {     //apostou na vitoria do dealer
        if (result == 2) {      //ganhou a aposta
            bet_result = 1;
            *currency += bet_value;     //ajusta saldo
        }
        else {      //perdeu a aposta
            bet_result = 0;
            *currency -= bet_value;     //ajusta saldo
        }
    }
    else {      //apostou no empate
        if (result == 3) {      //ganhou a aposta
            bet_result = 1;
            *currency += 8 * bet_value;     //ajusta saldo, cashout 8:1
        }
        else {      //perdeu a aposta
            bet_result = 0;
            *currency -= bet_value;
        }
    }

    //printa resutlado da aposta
    if (bet_result == 1) {
        printf ("Você ganhou a aposta! =D\n");
    }
    else {
        printf ("Você perdeu a aposta. :(\n");
    }
    
    free(player_card_count);
    free(dealer_card_count);
    free(player_hand);
    free(dealer_hand);
}