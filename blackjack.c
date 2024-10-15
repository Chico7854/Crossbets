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
void draw (int* hand, int* hand_index) {
    int card = (rand() % 13) + 1;                         //em cassinos é padrão 6 baralhos, entao não tem problema se vier até 24 cartas de mesmo valor, logo não há problema com esse método mesmo gerando repetidas cartas
    hand[*hand_index] = card;                             //1, 11, 12, 13, representam A, J, Q, K, respectivamente
    *hand_index += 1;
}

//calcula valor da mão
int hand_score(int* hand, int* count) {
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
        printf ("A ");
    }
    else if (card == 11) {
        printf ("J ");
    }
    else if (card == 12) {
        printf ("Q ");
    }
    else if (card == 13) {
        printf ("K ");
    }
    else {
        printf ("%d ", card);
    }
}

//printa só a mão
void print_hand (int* hand, int* count) {
    for (int i = 0; i < *count; i++) {
        print_card (hand[i]);
    }
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

//começa o jogo
//retorna 0 se perdeu e 1 se ganhou
int start_game() {
    int* player_hand = (int *)malloc(MAX_HAND * sizeof(int));                       //inicializa mao de jogador
    int* dealer_hand = (int *)malloc(MAX_HAND * sizeof(int));                       //incializa mao do dealer
    int* player_card_count = (int *)malloc(sizeof(int));                            //contagem cartas player
    int* dealer_card_count = (int *)malloc(sizeof(int));                            //contagem cartas dealer
    int player_score = 0;                                                           //score da mao do player
    int dealer_score = 0;                                                           //score da mao do dealer
    *player_card_count = 0;
    *dealer_card_count = 0;
    char player_action = 0;                                     //variavel vai ser usada para checar se player vai comprar carta ou nao

    //começo da rodada
    draw (player_hand, player_card_count);
    draw (dealer_hand, dealer_card_count);
    draw (player_hand, player_card_count);
    draw (dealer_hand, dealer_card_count);

    //vez do player
    while (player_action != 'n') {
        printf ("Carta do Dealer: %d\n", dealer_hand[0]);                                //printa carta do dealer que fica a mostra
        printf ("Suas cartas: ");
        print_hand(player_hand, player_card_count);                                                 //printa cartas do player
        printf ("Comprar carta? [s][n]\n");

        player_action = 0;                                                          //redefine a variavel para que nao fique salvo o "s" e quebre o prox while

        while (player_action != 's' && player_action != 'n') {
            scanf (" %c", &player_action);                                          //perguntar ação ao player em loop, apenas 's' ou 'n'
        }                                                                           

        if (player_action == 's') {                                                 //se player comprar carta
            draw (player_hand, player_card_count);
            printf ("Você comprou: ");                                              //printa qual carta ele comprou
            print_card (player_hand[*player_card_count - 1]);
        }

        player_score = hand_score (player_hand, player_card_count);

        if (player_score > 21) {                                                    //verifica se player estourou 21 pontos
            printf ("Você estourou 21 pontos.\nVocê perdeu. :(\n");
            return 0;
        }
    }

    //vez do dealer
    dealer_score = hand_score (dealer_hand, dealer_card_count);

    while (dealer_score < 17) {                                                     //vez do dealer, regra diz que dealer joga até 17 pontos
        printf ("Cartas do Dealer: ");
        print_hand (dealer_hand, dealer_card_count);
        
        draw (dealer_hand, dealer_card_count);                                      //compra carta

        printf ("Dealer comprou: ");
        print_card (dealer_hand[*dealer_card_count - 1]);                           //printa carta comprada
        printf ("\n");

        dealer_score = hand_score (dealer_hand, dealer_card_count);                 

        if (dealer_score > 21) {
            printf ("Dealer estourou 21 pontos.\nVocê ganhou! :)\n");               //verifica se dealer estourou 21 pontos      
            return 1;
        }
    }

    //comparacao final
    printf ("Cartas do Dealer: ");                                  //printa as duas maos
    print_hand (dealer_hand, dealer_card_count);
    printf ("Suas cartas: ");
    print_hand (player_hand, player_card_count);

    if (player_score > dealer_score) {
        printf ("Você ganhou! :)\n");
        return 1;
    }
    else if (player_score < dealer_score) {
        printf ("Você perdeu.\n");
        return 0;
    }

    free (player_hand);
    free (dealer_hand);
    free (player_card_count);
    free (dealer_card_count);
}