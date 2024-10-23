#include "blackjack.h"
//regras da bicyclecards

static int MAX_HAND = 11;       //maximo de cartas que pode ter na mão

//resultado dos jogos e funções
typedef enum {
    PERDEU = 0,
    GANHOU,
    EMPATOU,
    NAOPERDEU,
    SALDONATURAL     //blackjack natural usado apenas para saldo
} Result;       

//opcoes e cenarios possiveis no começo do jogo
typedef enum {
    NORMAL = 0,     //nao tem blackjack natural nem split nem double
    SPLIT,       //dividir mao
    DOUBLE,      //dobrar mao
    STARTNATURAL        //variavel para verificar se teve start com blackjack natural
} StartActions;

//FUNÇÕES UTILITÁRIAS
//são chamadas varias vezes para evitar repetição
static int hand_score(int* hand, int* count);              //calcula valor da mao
static void print_start_interface(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count);      //printa maos, printa apenas uma carta do dealer
static void print_full_interface(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count);        //printa todas as cartas do player e do dealer
static void update_currency(int* currency, int bet_value, Result result);      //update no saldo
static Result player_draw_round(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count, int* currency, int bet_value);       //simula rodada de compras de cartas
static void compare_scores(int* currency, int bet_value, int player_score, int dealer_score);     //compara valor das maos

//FUNÇÕES ORGANIZACIONAIS
//são chamadas apenas uma vez mas ajudam a manter o fluxo de main() organizado e conciso
static bool natural_blackjack(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count, int* currency, int bet_value);      //verifica exsitencia de blackjack natural
static StartActions round_start(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count, int* currency, int bet_value);     //simula começo do jogo
static Result dealer_turn(int* dealer_hand, int* dealer_card_count, int* currency, int bet_value);     //simula vez do dealer
static int game_split(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count, int* currency, int bet_value);        //simula jogo se player escolhe dividir a mão



//calcula valor da mão
static int hand_score(int* hand, int* count) {
    int score = 0;
    int aces = 0;                               //guarda quantos aces na mao para calcular se A vale 1 ou 11
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

    while (score <= 11 && aces > 0) {           //muda valor de A para 11 se possivel
        score += 10;
        aces --;
    }

    return score;
}

//printa interface com apenas uma carta do dealer e mao inteira do player
static void print_start_interface(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count) {
    printf ("Carta do Dealer: ");       
    print_card(dealer_hand[0]);     //printa carta do dealer que fica a mostra
    printf ("\n");

    printf ("Suas cartas: ");
    print_hand(player_hand, player_card_count, hand_score(player_hand, player_card_count));     //printa cartas do player

    printf ("\n");
}

//printa interface com maos inteiras
static void print_full_interface(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count) {
    printf ("Cartas do Dealer: ");      
    print_hand(dealer_hand, dealer_card_count, hand_score(dealer_hand, dealer_card_count));     //printa mao do dealer

    printf ("Suas cartas: ");
    print_hand(player_hand, player_card_count, hand_score(player_hand, player_card_count));     //printa mao do player

    printf ("\n");
}

//atualiza saldo
static void update_currency(int* currency, int bet_value, Result result) {
    //se player perder
    if (result == PERDEU) {      
        *currency -= bet_value;
    }
    //se player ganhar
    else if (result == GANHOU) {     
        *currency += bet_value;
    }
    //se player ganhar com blackjack natural, pagamento 1.5:1
    else if (result == SALDONATURAL) {       
        *currency += bet_value * 0.5;        //parte do valor vai ser perdido pq transforma float pra int, mas é centavos entao ninguem se importa
    }
}

//simula rodada de compra de cartas do player
static Result player_draw_round(int *player_hand, int *player_card_count, int *dealer_hand, int *dealer_card_count, int* currency, int bet_value) {
    char player_action = 0;     //escolha do player
    int player_score;
    Result result = NAOPERDEU;      //controla se player estourou 21 pontos

    //loop enquanto player continuar comprando cartas ou não estourar 21 ponto
    while (player_action != 'N' && result != PERDEU) {     
        player_score = hand_score(player_hand, player_card_count);     //valor da mao do player

        print_start_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);      //printa mãos

        printf ("Comprar carta? ");

        player_action = scanf_sn();        //input se player compra carta

        printf ("\n");

        //verifica se player escolheu comprar carta
        if (player_action == 'S') {
            draw (player_hand, player_card_count);      //compra carta

            printf ("Você comprou: ");
            fflush(stdout);     //flush buffer

            sleep(1);       //espera 1 segundo

            print_card (player_hand[*player_card_count - 1]);       //printa carta comprada
            printf ("\n\n");

            player_score = hand_score(player_hand, player_card_count);      //valor da mao do player

            sleep(1);       //espera 1 segundo
        }

        //verifica se player estourou 21 pontos
        if (player_score > 21) {                                                    
            printf ("Você estourou 21 pontos.\nVocê perdeu. :(\n");

            result = PERDEU;

            update_currency (currency, bet_value, result);       //update saldo
        }
    }

    return result;      //retorna se player estourou 21
}

//compara valores das maos
static void  compare_scores(int* currency, int bet_value, int player_score, int dealer_score) {
    sleep(1);

    //se player ganha
    if (player_score > dealer_score) {
        printf ("Você ganhou! =D\n");

        update_currency (currency, bet_value, GANHOU);
    }
    //se player perde
    else if (player_score < dealer_score) {     
        printf ("Você perdeu. :(\n");

        update_currency (currency, bet_value, PERDEU);
    }      
    //se player empata 
    else {      
        printf ("Você empatou.\n");
    }
}

//verifica se player ou dealer tem blackjack natural
static bool natural_blackjack(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count, int* currency, int bet_value) {
    //soma valor das maos
    int player_score = hand_score(player_hand, player_card_count);
    int dealer_score = hand_score(dealer_hand, dealer_card_count);

    bool natural = false;       //controla se há blackjack natural

    //verifica se player tem 21 pontos e dealer NÃO tem
    if (player_score == 21 && dealer_score != 21) {
        print_full_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);       //printa maos

        printf ("Você tem um Blackjack Natural. Você ganhou! =D\n");

        update_currency (currency, bet_value, SALDONATURAL);      //update saldo

        natural = true;
    }
    //verifica se delaer tem 21 pontos e player NÃO tem
    else if (dealer_score == 21 && player_score != 21) {
        print_full_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);       //printa maos

        printf ("Dealer tem um Blackjack Natural. Você perdeu.\n");

       update_currency(currency, bet_value, PERDEU);        //update saldo

       natural = true;
    }
    //verifica se player E dealer tem 21 pontos
    else if (player_score == 21 && dealer_score == 21) {
        print_full_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);       //printa maos

        printf ("Você e o Dealer tem Blackjack Natural. Você empatou.\n");

        natural = true;
    }

    return natural;     //retorna se alguem tirou blackjack natural
}

//simula começo do jogo
static StartActions round_start(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count, int* currency, int bet_value) {
    //compra de cartas
    draw (player_hand, player_card_count);
    draw (dealer_hand, dealer_card_count);
    draw (player_hand, player_card_count);
    draw (dealer_hand, dealer_card_count);

    printf ("\n");

    //insurance, player pode apostar se dealer tem blackjack ou nao, custo de metade da aposta original
    //verifica se carta virada do dealer é um A
    if (dealer_hand[0] == 1) {
        printf ("Carta do Dealer: [A]\n\nVocê deseja fazer uma aposta de seguro? ");

        int choice = scanf_sn();        //input do player

        printf ("\n");

        //verifica se player aceitou insurance
        if (choice == 'S') {
            //verifica se dealer tem blackjack
            if (dealer_hand[1] == 10) {
                *currency += bet_value;     //update saldo com aposta da insurance
            }
            else {      //se dealer nao tem blackjack
                *currency -= bet_value * 0.5;       //update saldo com o gasto da aposta da insurance
            }
        }
    }

    //verifica se alguem tem natural blackjack
    if (natural_blackjack(player_hand, player_card_count, dealer_hand, dealer_card_count, currency, bet_value)) {
        return STARTNATURAL;        //jogo acabou, entao retorna
    }

    //valor da mao do player
    int player_score = hand_score(player_hand, player_card_count);

    //verifica possiveis starts como double e split e pergunta pro player se ele quer ou nao fazer tais opcoes
    char player_choice_sn;      //escolha que o player vai fazer, perguntas sim ou nao
    StartActions start = NORMAL;     //input tipo de jogo que player escolher

    //verifica se pode dividir mao do player (se as cartas do player sao iguais)
    if (player_hand[0] == player_hand[1]) {
        //verifica se ambas as cartas forem 5, player pode comprar carta, dar split, ou dar double
        if (player_hand[0] == 5) {      
            print_start_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);        //printa maos

            printf ("Você tem dois [5] na sua mão. ");
            
            //verifica se player tem saldo para dobrar aposta
            //se player não tem saldo
            if (*currency - (2 * bet_value) < 0) {      
                printf ("Mas você não tem saldo para dobrar nem dividir a mão.\n");
            }
            //se player tem saldo
            else {      
                printf ("Deseja dividir ou dobrar mão? ");

                player_choice_sn = scanf_sn();      //input player

                printf ("\n");

                //verifica se player quiser dividir/dobrar
                if (player_choice_sn == 'S') {
                    printf ("Digite sua escolha:\n\n[1] Dividir\n[2] Dobrar\n\nDigite aqui: ");
                }

                int split_or_double = scanf_num(1, 2);      //input player quer dividir OU dobrar

                //verifica se player escolher split
                if (split_or_double == 1) {     
                    start = SPLIT;
                }
                //se player escolher double
                else {      
                    start = DOUBLE;
                }
            }
        }
        //verifica se player tem split, mas não double
        else {      
            print_start_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);        //printa maos

            printf ("Você tem duas cartas iguais. ");
            
            //verifica se player tem saldo para dividir a mao
            //se nao tiver saldo
            if (*currency - (2 * bet_value) < 0) {      
                printf ("Dividir mão? ");

                player_choice_sn = scanf_sn();      //input escolha do player se divide mao

                printf ("\n");

                //verifica se player escolher split
                if (player_choice_sn == 'S') {      
                    start = SPLIT;
                }
            }
        }
    }
    //verifica se pode dobrar mao do player (soma das cartas == 9, 10, 11)
    else if (player_score >= 9 && player_score <= 11) {
        print_start_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);        //printa maos
        
        printf ("O total da sua mão está entre 9 e 11. ");

        //verifica se tem saldo para dobrar
        //se nao tem saldo
        if (*currency - (2 * bet_value) < 0) {      
            printf ("Mas você não tem dinheiro para dobrar aposta.\n");
        }
        //se tem saldo
        else {      
            printf ("Dobrar mão? ");

            player_choice_sn = scanf_sn();      //input do player se dobra mao

            printf ("\n");

            //verifica se player escolher double
            if (player_choice_sn == 'S') {      
                start = DOUBLE;
            }
        }
    }

    return start;        //retorna tipo de jogo q player esocolheu, normal, double ou split
}

//simula vez do dealer
static Result dealer_turn(int* dealer_hand, int* dealer_card_count, int* currency, int bet_value) {
    int dealer_score = hand_score (dealer_hand, dealer_card_count);     //calcula total da mao do dealer

    while (dealer_score < 17) {     //na vez do dealer regra diz que dealer joga até 17 pontos
        printf ("Cartas do Dealer: ");
        print_hand(dealer_hand, dealer_card_count, dealer_score);
        
        draw (dealer_hand, dealer_card_count);

        printf ("Dealer comprou: ");
        fflush(stdout);

        sleep (1);

        print_card (dealer_hand[*dealer_card_count - 1]);
        printf ("\n\n");

        dealer_score = hand_score (dealer_hand, dealer_card_count);

        if (dealer_score > 21) {
            sleep(1);

            Result resultado = GANHOU;

            update_currency (currency, bet_value, resultado);

            return resultado;
        }
    }

    return NAOPERDEU;
}

//simula jogo se player divide a mão
static int game_split(int* player_hand, int* player_card_count, int* dealer_hand, int* dealer_card_count, int* currency, int bet_value) {
    int* player_hand_split = (int *)malloc(MAX_HAND * sizeof(int));     //segunda mao do player

    if (player_hand_split == NULL) {
        printf ("Falha em alocar memória");
        return MEMORY_ERROR;
    }
    player_hand_split[0] = player_hand[0];      //copia carta para a segunda mao

    //nas regras da bicycle se player dividiu mao e a carta é um A, entao ele compra apenas uma carta
    //verifica se carta é A
    if (player_hand[0] == 1) {
        printf ("Sua carta é um [A]. Você compra apenas uma carta.\n");

        draw (player_hand, player_card_count);

        printf ("Você comprou: ");
        fflush(stdout);

        sleep(1);

        print_card (player_hand[*player_card_count - 1]);
        printf ("\n\n");
    }
    else {
        //joga a primeira mao
        printf ("\nJogando a primeira mão:\n\n");
        Result result_1 = player_draw_round(player_hand, player_card_count, dealer_hand, dealer_card_count, currency, bet_value);        //simula primeira mao
        int player_score_1 = hand_score(player_hand, player_card_count);

        *player_card_count = 1;     //zera a contagem para a segunda mao

        //joga a segunda mão
        printf ("\nJogando a segunda mão:\n\n");
        Result result_2 = player_draw_round(player_hand_split, player_card_count, dealer_hand, dealer_card_count, currency, bet_value);     //simula segunda mao
        int player_score_2 = hand_score(player_hand_split, player_card_count);

        //verifica se as duas maos estouraram
        if (result_1 == PERDEU && result_2 == PERDEU) {
            printf ("\nAmbas as suas mãos estouraram. :(\n");
        }
        //se pelo menos alguma mao nao estourou
        else {
            //vez do dealer
            Result dealer_result = dealer_turn(dealer_hand, dealer_card_count, currency, bet_value);
            
            //verifica se dealer estourou mão
            if (dealer_result == GANHOU) {
                sleep(1);

                printf ("\nDealer estourou 21 pontos. ");

                //verifica se player estourou mao 1
                if (result_1 == PERDEU) {
                    printf ("Sua segunda mão ganhou! =D\n");
                    update_currency(currency, bet_value, GANHOU);
                }
                //se player estourou mao 2
                else if (result_2 == PERDEU) {
                    printf ("Sua primeira mão ganhou! =D\n");
                    update_currency(currency, bet_value, GANHOU);
                }
                //se player nao estourou nenhuma mao
                else {
                    printf ("Você ganhou suas duas mãos! =D\n");
                    update_currency(currency, bet_value, GANHOU);       //update saldo duas vezes, uma pra cada mão
                    update_currency(currency, bet_value, GANHOU);
                }
            }

            int dealer_score = hand_score(dealer_hand, dealer_card_count);

            //compara resultados
            //verifica se player não estourou 21 na primeira mao
            if (result_1 != PERDEU) {
                printf ("\nComparando primeira mão:\n\n");
                print_full_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);

                compare_scores(currency, bet_value, player_score_1, dealer_score);     //compara maos e decide vencedor
            }
            
            //verifica se player não estourou 21 na segunda mao
            if (result_2 != PERDEU) {
                printf ("\nComparando segunda mão:\n\n");
                print_full_interface(player_hand_split, player_card_count, dealer_hand, dealer_card_count);

                compare_scores(currency, bet_value, player_score_2, dealer_score);     //compara maos e decide vencedor
            }
        }
    }

    free (player_hand);
    free (player_hand_split);
    free (dealer_hand);
    free (player_card_count);
    free (dealer_card_count);

    return 0;
}

//começa o jogo
int start_blackjack(int* currency) {
    int* player_hand;       
    int* dealer_hand;
    int* player_card_count;
    int* dealer_card_count;

    if (!initialize_hands(&player_hand, &dealer_hand, &player_card_count, &dealer_card_count, MAX_HAND)) {
        return MEMORY_ERROR;
    }   

    //aposta
    int bet_value = scanf_bet(currency);

    //começo da rodada
    StartActions start_actions = round_start(player_hand, player_card_count, dealer_hand, dealer_card_count, currency, bet_value);

    //verifica se alguem teve blackjack natural
    if (start_actions == STARTNATURAL) {
        free (player_hand);
        free (dealer_hand);
        free (player_card_count);
        free (dealer_card_count);

        return 0;
    }

    //verifica se player escolheu DOUBLE
    if (start_actions == DOUBLE) {      
        bet_value = bet_value * 2;
        start_actions = NORMAL;     //DOUBLE altera apenas a aposta, depois disso jogo volta ao normal
    }

    //verifica se player escolheu SPLIT
    if (start_actions == SPLIT) {
        game_split(player_hand, player_card_count, dealer_hand, dealer_card_count, currency, bet_value);
    }
    //se player não divide mão
    else {
        //vez do player
        if (player_draw_round(player_hand, player_card_count, dealer_hand, dealer_card_count, currency, bet_value) == PERDEU) {
            free (player_hand);
            free (dealer_hand);
            free (player_card_count);
            free (dealer_card_count);

            return 0;
        }

        //vez do dealer
        if (dealer_turn(dealer_hand, dealer_card_count, currency, bet_value) == GANHOU) {
            printf ("Dealer estourou 21 pontos. Você ganhou! =D\n");

            free (player_hand);
            free (dealer_hand);
            free (player_card_count);
            free (dealer_card_count);

            return 0;
        }

        int player_score = hand_score(player_hand, player_card_count);
        int dealer_score = hand_score(dealer_hand, dealer_card_count);

        print_full_interface(player_hand, player_card_count, dealer_hand, dealer_card_count);

        sleep(1);

        //comparacao de resultado
        compare_scores(currency, bet_value, player_score, dealer_score);

        free (player_hand);
        free (dealer_hand);
        free (player_card_count);
        free (dealer_card_count);
    }

    return 0;
}