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

//cria um deck
//retorna array de struct Cards
Cards* create_deck() {
    int i, j, k;
    
    Cards *deck = (Cards *)malloc(DECK_SIZE * sizeof(Cards));

    if (deck == NULL) {
        return NULL;
    }

    char* cards[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int values[] = {11, 2, 3, 4, 5,  6, 7, 8, 9, 10, 10, 10, 10};
    int index;

    for (i = 0; i < DECK_COUNT; i++) {              //diferentes baralhos
        for (j = 0; j < 13; j++) {                  //diferentes valores de cartas
            for (k = 0; k < 4; k++) {               //diferentes nipes
                index = (i * 52) + (j * 4) + k;
                deck[index].card = cards[j];
                deck[index].value = values[j];
            }
        }
    }

    deck = shuffle(deck);                           //embaralha o deck

    return deck;
}

//cria mão para jogar
//retorna array de Cards
Cards* create_hand() {
    Cards* hand = (Cards *)malloc(MAX_HAND * sizeof(Cards));

    if (hand == NULL ) {
        return NULL;
    }

    return hand;
}

//embaralha o deck
//retorna array de struct cards
//utiliza algoritmo Fisher-Yates de shuffle
Cards* shuffle(Cards* deck) {
    if (deck == NULL) {
        return NULL;
    }

    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        Cards temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }

    return deck;
}

//calcula valor da mão
int hand_score(Cards* hand, int count) {
    int score = 0;
    int aces = 0;

    for (int i = 0; i < count; i++) {
        score += hand[i].value;
        if (hand[i].value == 11) {
            aces++;
        }
    }

    while (score > 21 && aces > 0) {
        score -= 10;
        aces --;
    }

    return score;
}

//compra carta
//ajusta em ponteiro contagem de cartas das maos e do deck
void draw (Cards* deck, Cards* hand, int* hand_index, int* deck_index) {
    hand[*hand_index] = deck[*deck_index];
    *hand_index += 1;
    *deck_index += 1;
}


//printa informações para o player
void print (Cards* player, Cards* dealer, int* pcount) {
    printf ("Carta do Dealer: ");                           //printa carta do dealer
    printf ("%s ", dealer[0].card);
    printf ("\n");

    printf ("Suas Cartas: ");                               //printa cartas do player
    for (int i = 0; i < *pcount; i++) {
        printf ("%s ", player[i].card);
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
void start_game() {
    Cards* deck = create_deck();                            //cria o deck
    Cards* player_hand = create_hand();                     //inicializa mao de jogador
    Cards* dealer_hand = create_hand();                     //incializa mao do dealer
    int* player_card_count = (int *)malloc(sizeof(int));                      
    int* dealer_card_count = (int *)malloc(sizeof(int));
    int* index_deck = (int *)malloc(sizeof(int));           //onde esta o baralho, index_deck ++ é igual a comprar uma carta]
    *player_card_count = 0;
    *dealer_card_count = 0;
    *index_deck = 0;

    //começo da rodada
    draw (deck, player_hand, player_card_count, index_deck);
    draw (deck, dealer_hand, dealer_card_count, index_deck);
    draw (deck, player_hand, player_card_count, index_deck);
    draw (deck, dealer_hand, dealer_card_count, index_deck);

    /*while () {
        print (player_hand, dealer_hand, player_card_count);
        printf ("Comprar carta? [s][n]\n");
        char player_action = 0;                                
        while (player_action != 's' || player_action != 'n') {
            scanf ("%c", &player_action);                           //perguntar ação ao player em loop, apenas 's' ou 'n'
        }

        if (player_action == 's') {

        }
    }*/

    free (deck);
    free (player_hand);
    free (dealer_hand);
    free (player_card_count);
    free (dealer_card_count);
    free (index_deck);

}