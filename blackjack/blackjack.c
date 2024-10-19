#include "blackjack.h"

//criar um jogo que seja possível contar cartas, usa o mesmo baralho até acabar as cartas
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

//compra carta
//ajusta em ponteiro a mao e a contagem
static void draw (int* hand, int* hand_index) {
    int card = (rand() % 13) + 1;                         //em cassinos é padrão 6 baralhos, entao não tem problema se vier até 24 cartas de mesmo valor, logo não há problema com esse método mesmo gerando repetidas cartas
    hand[*hand_index] = card;                             //1, 11, 12, 13, representam A, J, Q, K, respectivamente
    *hand_index += 1;
}

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

//printa carta
void print_card (int card) {
    if (card == 1) {
        printf ("[A] ");
    }
    else if (card == 11) {
        printf ("[J] ");
    }
    else if (card == 12) {
        printf ("[Q] ");
    }
    else if (card == 13) {
        printf ("[K] ");
    }
    else {
        printf ("[%d] ", card);
    }
}

//printa só a mão
static void print_hand (int* hand, int* count) {
    for (int i = 0; i < *count; i++) {
        print_card (hand[i]);
    }

    int total = hand_score(hand, count);

    printf ("(Total: %d)", total);
    printf ("\n");
}

//verifica se hand tem natural blackjack
//retorna 1 se tem, 0 se não
/*
int natural_blackjack (Cards* hand, int count) {
    int score = hand_score(hand, count);

    if (score == 21) {
        return 1;
    }

    return 0;
}*/

//arruma saldo
void update_currency(int* currency, int bet_value, int result) {
    if (result == 0) {      //se player perder
        *currency -= bet_value;
    }
    else if (result == 1) {     //se player ganha
        *currency += bet_value;
    }
}

//começa o jogo
//retorna o quanto a bet deve ser multiplicada pra entrar no saldo
void start_blackjack(int* currency) {
    int* player_hand;
    int* dealer_hand;
    int* player_card_count;
    int* dealer_card_count;

    if (!initialize_hands(&player_hand, &dealer_hand, &player_card_count, &dealer_card_count, MAX_HAND_BLACKJACK)) {
        return;
    }   

    int player_score = 0;                                                           //score da mao do player
    int dealer_score = 0;                                                           //score da mao do dealer

    //aposta
    int bet_value = scanf_bet(currency);        //input valor da aposta

    //começo da rodada
    draw (player_hand, player_card_count);
    draw (dealer_hand, dealer_card_count);
    draw (player_hand, player_card_count);
    draw (dealer_hand, dealer_card_count);

    printf ("\n");

    //vez do player
    while (1) {                                 //loop acaba quando player nao comprar amis cartas
        printf ("Carta do Dealer: ");                                //printa carta do dealer que fica a mostra
        print_card(dealer_hand[0]);
        printf ("\n");

        printf ("Suas cartas: ");
        print_hand(player_hand, player_card_count);                                                 //printa cartas do player

        printf ("\nComprar carta? ");

        char player_action = scanf_sn();        //input escolha do player                                                                      

        printf ("\n");      //melhor visualizacao no terminal

        if (player_action == 'S') {                                                 //se player comprar carta
            draw (player_hand, player_card_count);      //compra carta

            printf ("Você comprou: ");                                              //printa qual carta ele comprou
            fflush(stdout);     //printa buffer

            sleep(1);       //espera 1 segundo

            print_card (player_hand[*player_card_count - 1]);       //printa carta comprada
            printf ("\n\n");

            sleep(1);       //espera 1 segundo
        }
        else {
            break;      //se player nao comrpar carta
        }

        player_score = hand_score (player_hand, player_card_count);     //valor da mao do player

        if (player_score > 21) {                                                    //verifica se player estourou 21 pontos
            printf ("Você estourou 21 pontos.\nVocê perdeu. :(\n");
            update_currency (currency, bet_value, 0);       //update saldo, ultimo valor é 0 pq player perdeu

            free (player_hand);     //desaloca memória
            free (dealer_hand);
            free (player_card_count);
            free (dealer_card_count);

            return;
        }
    }

    //vez do dealer
    dealer_score = hand_score (dealer_hand, dealer_card_count);

    while (dealer_score < 17) {                                                     //vez do dealer, regra diz que dealer joga até 17 pontos
        printf ("Cartas do Dealer: ");
        print_hand(dealer_hand, dealer_card_count);        //printa mao do dealer
        
        draw (dealer_hand, dealer_card_count);                                      //compra carta

        printf ("Dealer comprou: ");
        fflush(stdout);     //printa buffer

        sleep (1);      //espera 1 seg

        print_card (dealer_hand[*dealer_card_count - 1]);                           //printa carta comprada
        printf ("\n\n");

        dealer_score = hand_score (dealer_hand, dealer_card_count);                 

        if (dealer_score > 21) {
            sleep(1);       //espera 1 segundo

            printf ("Dealer estourou 21 pontos.\nVocê ganhou! :)\n");               //verifica se dealer estourou 21 pontos  
            update_currency (currency, bet_value, 1);       //update saldo, ultimo valor é 1 pq player ganhou    

            free (player_hand);     //desacloca memória
            free (dealer_hand);
            free (player_card_count);
            free (dealer_card_count);

            return;
        }
    }

    //comparacao final
    printf ("Cartas do Dealer: ");                                  //printa as duas maos
    print_hand (dealer_hand, dealer_card_count);
    printf ("Suas cartas: ");
    print_hand (player_hand, player_card_count);
    printf ("\n");

    sleep(1);       //espera 1 segundo

    player_score = hand_score (player_hand, player_card_count);

    if (player_score > dealer_score) {                              //se player ganha
        printf ("Você ganhou! :)\n");
        update_currency (currency, bet_value, 1);       //update saldo
    }
    else if (player_score < dealer_score) {                         //se player perde
        printf ("Você perdeu.\n");
        update_currency (currency, bet_value, 0);       //update saldo
    }       
    else {                                                          //se player empata
        printf ("Você empatou.\n");     //nao precisar dar update no saldo pq empate n perde nem ganha
    }

    free (player_hand);
    free (dealer_hand);
    free (player_card_count);
    free (dealer_card_count);
}