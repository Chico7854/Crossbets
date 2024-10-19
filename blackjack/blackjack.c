#include "blackjack.h"
//regras da bicyclecards

//criar jogo
    //criar baralho
    //input aposta
    //dar cartas pro dealer e pro player
    //ver se player ou dealer tem natural backjack, se player tem natural pagar 1.5x, se dealer tem acaba, se ambos reseta, (se dealer tem Ace, insurance, mas programar depoisd o projeto acabar)
    //verifica se pode haver splitting(programar depois de projeto terminado)
    //verifica se pode haver double
    //printa cartas e pergunta ação
        //se compra carta
            //compra carta
            //verifica se estourou, se não estourou pergunta ação (lopping)
        //se não compra carta
            //dealer revela carta, se mao >= 17 para e ve resultado, senão compra carta ate >=17 ou estourar
    //atualizar saldo dependendo resultado e printar
    //perguntar se quer jogar de novo
    //se não quiser jogar de novo, free malloc

static int MAX_HAND = 11;       //maximo de cartas que pode ter na mão

typedef enum {
    PERDEU = 0,
    GANHOU,
    EMPATOU
} Result;

static int hand_score(int* hand, int* count);              //calcula valor da mao
static void update_currency(int* currency, int bet_value, int result);      //update no saldo
static int player_turn(int* player_hand, int* player_card_count, int* dealer_hand, int* currency, int bet_value);        //simula vez do player
static int dealer_turn(int* dealer_hand, int* dealer_card_count, int* currency, int bet_value);     //simula vez do dealer
static void final_result(int* currency, int bet_value, int player_score, int dealer_score);     //simula final do jogo

//calcula valor da mão
static int hand_score(int* hand, int* count) {
    int score = 0;
    int aces = 0;                               //guarda quantos aces na mao para acalcular se A vale 1 ou 11
    int card;                                   //variavel temporaria pra guardar carta sendo iterada

    for (int i = 0; i < *count; i++) {
        card = hand[i];
        if (card > 10) {                        //se for J, Q, K, valor = 10
            card = 10;
        }

        score += card;

        if (card == 1) {
            aces++;
        }
    }

    while (score <= 11 && aces > 0) {           //muda valor de Ais para 11 se possivel
        score += 10;
        aces --;
    }

    return score;
}

//verifica se hand tem natural blackjack
/*
int natural_blackjack (Cards* hand, int count) {
    int score = hand_score(hand, count);

    if (score == 21) {
        return 1;
    }

    return 0;
}*/

//arruma saldo
static void update_currency(int* currency, int bet_value, int result) {
    if (result == 0) {      //se player perder
        *currency -= bet_value;
    }
    else if (result == 1) {     //se player ganha
        *currency += bet_value;
    }
}

//simula vez do player
static int player_turn(int* player_hand, int* player_card_count, int* dealer_hand, int* currency, int bet_value) {
    while (1) {     //loop acaba quando player nao comprar mais cartas
        int player_score = hand_score (player_hand, player_card_count);     //valor da mao do player

        printf ("Carta do Dealer: ");       
        print_card(dealer_hand[0]);     //printa carta do dealer que fica a mostra
        printf ("\n");

        printf ("Suas cartas: ");
        print_hand(player_hand, player_card_count, player_score);     //printa cartas do player

        printf ("\nComprar carta? ");

        char player_action = scanf_sn();        //input player compra carta ou nao

        printf ("\n");      //melhor visualizacao no terminal

        if (player_action == 'S') {     //se player comprar carta
            draw (player_hand, player_card_count);      //compra carta

            printf ("Você comprou: ");      //printa qual carta player comprou
            fflush(stdout);     //printa buffer

            sleep(1);       //espera 1 segundo

            print_card (player_hand[*player_card_count - 1]);       //printa carta comprada
            printf ("\n\n");

            player_score = hand_score(player_hand, player_card_count);

            sleep(1);       //espera 1 segundo
        }
        else {
            break;      //se player nao comprar carta quebra loop
        }

        if (player_score > 21) {                                                    //verifica se player estourou 21 pontos
            printf ("Você estourou 21 pontos.\nVocê perdeu. :(\n");

            Result resultado = PERDEU;

            update_currency (currency, bet_value, resultado);       //update saldo

            return resultado;
        }
    }
}

//simula vez do dealer
static int dealer_turn(int* dealer_hand, int* dealer_card_count, int* currency, int bet_value) {
    int dealer_score = hand_score (dealer_hand, dealer_card_count);     //calcula total da mao do dealer

    while (dealer_score < 17) {                                                     //na vez do dealer regra diz que dealer joga até 17 pontos
        printf ("Cartas do Dealer: ");
        print_hand(dealer_hand, dealer_card_count, dealer_score);        //printa mao do dealer
        
        draw (dealer_hand, dealer_card_count);                                      //compra carta

        printf ("Dealer comprou: ");
        fflush(stdout);     //printa buffer

        sleep (1);      //espera 1 seg

        print_card (dealer_hand[*dealer_card_count - 1]);                           //printa carta comprada
        printf ("\n\n");

        dealer_score = hand_score (dealer_hand, dealer_card_count);     //calcula novo total do dealer depois da compra de carta

        if (dealer_score > 21) {
            Result resultado = GANHOU;

            sleep(1);       //espera 1 segundo

            printf ("Dealer estourou 21 pontos.\nVocê ganhou! :)\n");               //verifica se dealer estourou 21 pontos  
            update_currency (currency, bet_value, resultado);       //update saldo, ultimo valor é 1 pq player ganhou    

            return resultado;
        }
    }

    return -1;      //pode retornar qlq valor que nao seja GANHOU (1), pois vai ser comparado se player ganhou ou nao
}

//calcula resultado final e simula fim do jogo
void final_result(int* currency, int bet_value, int player_score, int dealer_score) {
    
    Result resultado;

    if (player_score > dealer_score) {                              //se player ganha
        resultado = GANHOU;

        printf ("Você ganhou! :)\n");

        update_currency (currency, bet_value, resultado);       //update saldo
    }
    else if (player_score < dealer_score) {                         //se player perde
        resultado = PERDEU;

        printf ("Você perdeu.\n");

        update_currency (currency, bet_value, resultado);       //update saldo
    }       
    else {                                                          //se player empata
        resultado = EMPATOU;
        
        printf ("Você empatou.\n");     //nao precisar dar update no saldo pq empate n perde nem ganha
    }
}

//começa o jogo
//retorna o quanto a bet deve ser multiplicada pra entrar no saldo
int start_blackjack(int* currency) {
    //inicializa maos e contagem de cartas
    int* player_hand;       
    int* dealer_hand;
    int* player_card_count;
    int* dealer_card_count;

    if (!initialize_hands(&player_hand, &dealer_hand, &player_card_count, &dealer_card_count, MAX_HAND)) {        //se alocação da erro
        return MEMORY_ERROR;
    }   

    //aposta
    int bet_value = scanf_bet(currency);        //input valor da aposta

    //começo da rodada
    draw (player_hand, player_card_count);
    draw (dealer_hand, dealer_card_count);
    draw (player_hand, player_card_count);
    draw (dealer_hand, dealer_card_count);

    printf ("\n");

    //vez do player
    if (player_turn(player_hand, player_card_count, dealer_hand, currency, bet_value) == PERDEU) {      //se player estourou 21 e perdeu
        //desaloca memória
        free (player_hand);
        free (dealer_hand);
        free (player_card_count);
        free (dealer_card_count);

        return 0;
    }

    //vez do dealer
    if (dealer_turn(dealer_hand, dealer_card_count, currency, bet_value) == GANHOU) {       //se dealer estourou 21 e player ganhou
        //desaloca memória
        free (player_hand);
        free (dealer_hand);
        free (player_card_count);
        free (dealer_card_count);

        return 0;
    }

    //calcula total final
    int player_score = hand_score(player_hand, player_card_count);
    int dealer_score = hand_score(dealer_hand, dealer_card_count);

    //printa resultado das maos
    printf ("Cartas do Dealer: ");                                  //printa as duas maos
    print_hand (dealer_hand, dealer_card_count, dealer_score);
    printf ("Suas cartas: ");
    print_hand (player_hand, player_card_count, player_score);
    printf ("\n");

    sleep(1);       //espera 1 segundo

    //comparacao de resultado
    final_result(currency, bet_value, player_score, dealer_score);

    free (player_hand);
    free (dealer_hand);
    free (player_card_count);
    free (dealer_card_count);
}