#include "baccarat.h"
//baseado nas regras da wikipedia, versao punto banco (mais jogada em cassinos)

static int MAX_HAND = 3;        //numero maximo de cartas na mão

typedef enum {
    PERDEU = 0,
    GANHOU
} BetResult;

typedef enum {
    JOGADOR = 1,
    BANCO,
    EMPATE
} Winner;

static int hand_score (int* hand, int* count);     //valor da mao
static void first_draw_round (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count);      //simula compra de cartas inciais
static void third_card_draw (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count);       //verifica e simula comrpa da terceira carta
static int final_result (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count);       //verifica e printa resultado final

//calcula valor da mao
static int hand_score (int* hand, int* count) {
    int result = 0;

    for (int i = 0; i < *count; i ++) {
        result += hand[i];      //soma valor das cartas
    }

    return result % 10;     //retorna módulo de 10 do resultado, regra padrao de baccard
}

//verifica aposta e atualiza saldo, retorna se venceu ou nao
int bet_result(int player_choice, int result, int* currency, int bet_value) {
    if (result != EMPATE) {     //se jogador ou banco ganharam
        if (player_choice == result) {      //se player acertou aposta
            *currency += bet_value;
            return GANHOU;
        }
        else {      //se player errou aposta
            *currency -= bet_value;
            return PERDEU;
        }
    }
    else {      //se jogar e banco empataram
        if (player_choice == result) {      //se player acertou aposta
            *currency += 8 * bet_value;
            return GANHOU;
        }
        else {      //se player errou aposta
            *currency -= bet_value;
            return PERDEU;
        }
    }
}

//simula a compra de cartas das maos iniciais do delaer e do player
static void first_draw_round (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count) {
    //dealer compra 2 cartas
    draw(dealer_hand, dealer_card_count);
    draw(dealer_hand, dealer_card_count);

    //player compra 2 cartas
    draw(player_hand, player_card_count);
    draw(player_hand, player_card_count);

    //atualiza pontuação das maos
    int player_total = hand_score(player_hand, player_card_count);       //valor da mao do player
    int dealer_total = hand_score(dealer_hand, dealer_card_count);        //valor da mao do dealer

    //printa mao do player
    sleep(1);       //espera 1 segundo

    printf ("\nCartas do Jogador: ");
    print_hand(player_hand, player_card_count, player_total);
    
    //printa mao do dealer
    printf ("Cartas do Banco: ");
    print_hand(dealer_hand, dealer_card_count, dealer_total);
}

//simula turno de verificacao e compra de terceira carta
static void third_card_draw (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count) {
    //verifica se alguem tem natural baccard
    int player_total = hand_score(player_hand, player_card_count);      //calcula total do player
    int dealer_total = hand_score(dealer_hand, dealer_card_count);      //calcula total do dealer

    if (player_total < 8 && dealer_total < 8)  {        //se ambos NAO tiverem natural baccard o jogo continua
        //verifica se player compra terceira carta
        if (player_total > 5) {     //player nao compra carta
            if (dealer_total < 6 ) {        //verifica se dealer compra terceira carta
                draw(dealer_hand, dealer_card_count);       //dealer compra terceira carta

                dealer_total = (hand_score(player_hand, player_card_count));        //atualiza total

                sleep(1);       //espera 1 segundo

                printf ("\nBanco comprou terceira carta: ");       
                print_card(dealer_hand[*dealer_card_count - 1]);        //printa qual carta dealer comprou
                printf ("(Novo total: %d)\n", dealer_total);        //printa novo total
            }
        }
        else {     //player compra terceira carta
            draw (player_hand, player_card_count);

            player_total = hand_score(player_hand, player_card_count);      //atualiza total

            int player_third = player_hand[*player_card_count - 1];      //variavel guarda terceira carta do player

            sleep(1);       //espera 1 segundo

            printf ("\nPlayer comprou terceira carta: ");
            print_card(player_third);       //printa qual carta player comprou
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

                printf ("\nBanco comprou terceira carta: ");       
                print_card(dealer_hand[*dealer_card_count - 1]);        //printa qual carta dealer comprou
                printf ("(Novo total: %d)\n", dealer_total);        //printa novo total
            }
        }
    }
}

//verifica e printa resultado final
static int final_card_result (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count) {
    //printa resultado final
    sleep(1);       //espera 1 segundo

    int player_total = hand_score(player_hand, player_card_count);
    int dealer_total = hand_score(dealer_hand, dealer_card_count);

    printf ("\nResultado Final:\n");

    printf ("Cartas do Jogador: ");     //printa resultado do player
    print_hand(player_hand, player_card_count, player_total);

    printf ("Cartas do Banco: ");     //printa resultado do dealer
    print_hand(dealer_hand, dealer_card_count, dealer_total);

    //printa quem ganhou
    Winner result;     //guarda resultado para comparar com aposta

    sleep(1);       //espera 1 segundo

    if (player_total > dealer_total) {
        printf ("\nJogador ganhou. ");
        result = JOGADOR;
    }
    else if (dealer_total > player_total) {
        printf ("\nBanco ganhou. ");
        result = BANCO;
    }
    else {
        printf ("\nJogador e Banco empataram. ");
        result = EMPATE;
    }

    return result;
}

//rodada de verificação da aposta
static void bet_update_round (int player_choice, int result, int* currency, int bet_value) {
    BetResult player_bet;     //guarda resultado da aposta, variavel para nao ter que ficar repetindo printf
    if (player_choice == JOGADOR) {      //se apostou vitoria do player
        player_bet = bet_result(player_choice, result, currency, bet_value);      //verifica se player ganhou aposta
    }
    else if (player_choice == BANCO) {     //se apostou na vitoria do dealer
        player_bet = bet_result(player_choice, result, currency, bet_value);      //verifica se player ganhou aposta
    }
    else {      //se apostou no empate
        player_bet = bet_result(player_choice, result, currency, bet_value);      //verifica se player ganhou aposta
    }

    //printa resultado da aposta
    if (player_bet == GANHOU) {     //se player ganhou aposta
        printf ("Você ganhou a aposta! =D\n");
    }
    else {      //se player perdeu aposta
        printf ("Você perdeu a aposta. :(\n");
    }
}
//simula baccarat
int start_baccarat (int* currency) {
    int* dealer_hand;       //mao do dealer
    int* player_hand;       //mao do player
    int* dealer_card_count;     //contagem de cartas do dealer
    int* player_card_count;     //contagemd e cartas do player

    if (!initialize_hands(&player_hand, &dealer_hand, &player_card_count, &dealer_card_count, MAX_HAND)) {      //verifica se alocou memória
        return MEMORY_ERROR;
    }

    //aposta
    int bet_value = scanf_bet(currency);        //valor da aposta

    printf ("\nEscolha qual aposta fazer:\n\n");
    printf ("[1] Vitória do Jogador\n[2] Vitória do Banco\n[3] Empate\n\nDigite aqui: ");      //print opcoes de aposta

    int player_choice = multiple_choice_numbers(1, 3);      //input escolha de aposta do jogador
    
    //compra de cartas das maos iniciais
    first_draw_round (player_hand, dealer_hand, player_card_count, dealer_card_count);

    //verifica se compra terceira carta e entao compra
    third_card_draw(player_hand, dealer_hand, player_card_count, dealer_card_count);

    //verifica e printa resultado final
    Winner result = final_card_result(player_hand, dealer_hand, player_card_count, dealer_card_count);

    //verifica aposta
    bet_update_round(player_choice, result, currency, bet_value);
    
    free(player_card_count);
    free(dealer_card_count);
    free(player_hand);
    free(dealer_hand);
}