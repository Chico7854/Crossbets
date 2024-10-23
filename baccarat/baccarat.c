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

//FUNÇÕES UTILITÁRIAS
//são chamadas varias vezes para evitar repetição
static int hand_score (int* hand, int* count);     //valor da mao
static int bet_result(int player_choice, int result, int* currency, int bet_value);     //verifica aposta

//FUNÇÕES ORGANIZACIONAIS
//são chamadas apenas uma vez mas ajudam a manter o fluxo de main() organizado e conciso
static void first_draw_round (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count);      //simula compra de cartas inciais
static void third_card_draw (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count);       //verifica e simula comrpa da terceira carta
static int final_card_result (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count);       //verifica e printa resultado final
static void bet_update_round (int player_choice, int result, int* currency, int bet_value);     //rodada de verificação da aposta

//calcula valor da mao
static int hand_score (int* hand, int* count) {
    int result = 0;

    for (int i = 0; i < *count; i ++) {
        result += hand[i];      //soma valor das cartas
    }

    return result % 10;     //retorna módulo de 10 do resultado, regra padrao de baccard
}

//verifica aposta e atualiza saldo, retorna se venceu ou nao
static int bet_result(int player_choice, int result, int* currency, int bet_value) {
    //verifica se jogador ou banco ganharam
    if (result != EMPATE) {     
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

//simula a compra de cartas das maos iniciais do dealer e do player
static void first_draw_round (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count) {
    //dealer compra 2 cartas
    draw(dealer_hand, dealer_card_count);
    draw(dealer_hand, dealer_card_count);

    //player compra 2 cartas
    draw(player_hand, player_card_count);
    draw(player_hand, player_card_count);

    //atualiza pontuação das maos
    int player_total = hand_score(player_hand, player_card_count);
    int dealer_total = hand_score(dealer_hand, dealer_card_count);

    //printa mao do player
    sleep(1);

    printf ("\nCartas do Jogador: ");
    print_hand(player_hand, player_card_count, player_total);
    
    //printa mao do dealer
    printf ("Cartas do Banco: ");
    print_hand(dealer_hand, dealer_card_count, dealer_total);
}

//simula turno de verificacao e compra de terceira carta
static void third_card_draw (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count) {
    //atualiza pontuação
    int player_total = hand_score(player_hand, player_card_count);
    int dealer_total = hand_score(dealer_hand, dealer_card_count);

    //verifica se alguem tem natural baccard
    if (player_total < 8 && dealer_total < 8)  {        
        //verifica se player compra terceira carta
        if (player_total > 5) {     //se player nao compra carta
            //verifica se dealer compra terceira carta
            if (dealer_total < 6 ) {           
                draw(dealer_hand, dealer_card_count);       

                dealer_total = (hand_score(player_hand, player_card_count));        

                sleep(1);

                //printa qual carta dealer comprou
                printf ("\nBanco comprou terceira carta: ");       
                print_card(dealer_hand[*dealer_card_count - 1]);        
                printf ("(Novo total: %d)\n", dealer_total);
            }
        }
        else {     //se player compra terceira carta
            draw (player_hand, player_card_count);

            player_total = hand_score(player_hand, player_card_count);

            int player_third = player_hand[*player_card_count - 1];      //variavel guarda terceira carta do player

            sleep(1);

            //printa qual carta player comprou
            printf ("\nPlayer comprou terceira carta: ");
            print_card(player_third);       
            printf ("(Novo total: %d)\n", player_total);

            //verifica se dealer compra terceira carta
            bool drew_card = false;

            if (dealer_total <= 2) {
                draw(dealer_hand, dealer_card_count);
                drew_card = true;
            }
            else if (dealer_total == 3) {
                if (player_third != 8) {
                    draw(dealer_hand, dealer_card_count);
                    drew_card = true;
                }
            }
            else if (dealer_total == 4) {       
                if (player_third >= 2 && player_third <= 7) {
                    draw(dealer_hand, dealer_card_count);
                    drew_card = true;
                }
            }
            else if (dealer_total == 5) {
                if (player_third >= 4 && player_third <= 7) {
                    draw(dealer_hand, dealer_card_count);
                    drew_card = true;
                }
            }
            else if (dealer_total == 6) {
                if (player_third == 6 || player_third == 7) {
                    draw(dealer_hand, dealer_card_count);
                    drew_card = true;
                }
            }

            //verifica se dealer comprou carta
            if (drew_card) {       
                dealer_total = (hand_score(dealer_hand, dealer_card_count));

                sleep(1);

                //printa qual carta dealer comprou
                printf ("\nBanco comprou terceira carta: ");       
                print_card(dealer_hand[*dealer_card_count - 1]);        
                printf ("(Novo total: %d)\n", dealer_total);
            }
        }
    }
}

//verifica e printa resultado final
static int final_card_result (int* player_hand, int* dealer_hand, int* player_card_count, int* dealer_card_count) {
    //printa resultado final
    sleep(1);

    int player_total = hand_score(player_hand, player_card_count);
    int dealer_total = hand_score(dealer_hand, dealer_card_count);

    printf ("\nResultado Final:\n");
    
    //printa resultado do player
    printf ("Cartas do Jogador: ");     
    print_hand(player_hand, player_card_count, player_total);

    //printa resultado do dealer
    printf ("Cartas do Banco: ");     
    print_hand(dealer_hand, dealer_card_count, dealer_total);

    Winner result;

    sleep(1);

    //printa vencedor
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
    BetResult player_bet;

    if (player_choice == JOGADOR) {
        player_bet = bet_result(player_choice, result, currency, bet_value);
    }
    else if (player_choice == BANCO) {     
        player_bet = bet_result(player_choice, result, currency, bet_value);
    }
    else if (player_choice == EMPATE) {
        player_bet = bet_result(player_choice, result, currency, bet_value);
    }

    //printa resultado da aposta
    if (player_bet == GANHOU) {
        printf ("Você ganhou a aposta! =D\n");
    }
    else if (player_bet == PERDEU) {      //se player perdeu aposta
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

    int player_choice = scanf_num(1, 3);      //input escolha de aposta do jogador
    
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