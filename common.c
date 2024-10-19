#include "common.h"

//verifica se input esta entre os numeros válidos
int multiple_choice_numbers(int min, int max) {
    int choice;     //escolha do jogador

    while (1) {
        choice = scanf_num();       //input escolha

        if (choice != INVALID) {        //verifica se é numero
            if (choice >= min && choice <= max) {       //verifica se é uma opção válida
                return choice;
            }
        }

        if ((max - min) == 1) {     //se tiver apenas duas opçoes de escolha
            printf ("\nComando inválido. Digite [%d] ou [%d].\n\nDigite aqui: ", min, max);
        }
        else {      //se tem 3 opcoes ou mais de escolha
            printf ("\nComando inválido. Digite um número de [%d] a [%d].\n\nDigite aqui: ", min, max);
        }
    }
}

//scanf para numeros
int scanf_num () {
    char input[10];

    while (1) {
        fgets(input, sizeof(input), stdin);     // Lê a entrada do usuário

        // Limpa o buffer se o input for maior que o tamanho do buffer
        if (!strchr(input, '\n')) {
            while (getchar() != '\n');
        }

        // Verifica se a entrada contém apenas dígitos
        for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
            if (!isdigit(input[i])) {
                return INVALID;      //se for invalido
            }
        }

        int player_choice;

        sscanf(input, "%d", &player_choice);        //converte a string para um inteiro      
        
        return player_choice;
    }
}

//scanf para pergunta de sim ou nao
char scanf_sn () {
    char choice[3];

    printf("[S] [N]\n\nDigite aqui: ");

    while (1) {     //verifica input
        if (fgets(choice, sizeof(choice), stdin) != NULL) {
            if (choice[1] == '\n') {        //verifica se input tem apenas 1 caractere
                choice[0] = toupper(choice[0]);       //converte caracterre para maiusculo

                if (choice[0] == 'S' || choice[0] == 'N') {     //verifica se input é S ou N
                    break;      //input correto
                }
            }   
        }

        if (!strchr(choice, '\n')) {        //verifica se tem algo a mais no buffer, se achar \n na string signifca q nao tem
            while (getchar() != '\n');      //limpa buffer
        }

        printf ("\nComando inválido. Digite [S] ou [N].\n\nDigite aqui: ");
    }

    return choice[0];
}

//scanf para receber apostas
int scanf_bet(int* currency) {
    int bet_value;     

    printf ("\nSaldo atual: R$%d\n", *currency);
    printf ("Digite o valor da aposta.\n\nDigite aqui: R$");

    while (1) {
        bet_value = scanf_num();
        if (bet_value != INVALID) {       //verifica se input é numero
            if (bet_value >= 0 && bet_value <= *currency) {     //verifica se input esta me intervalo valido
                return bet_value;       //se input esta correto retorna valor da aposta
            }
        }
         
        printf ("Comando inválido. Tente novamente.\n\nDigite aqui: R$");       //se input esta incorreto printa e repete loop
    }
}

//aloca memória para jogos com baralho e uma mão de cartas para player e outra para dealer
int initialize_hands(int** player_hand, int** dealer_hand, int** player_card_count, int** dealer_card_count, int max_hand) {
    *player_hand = (int *)malloc(max_hand * sizeof(int));                       //inicializa mao de jogador
    *dealer_hand = (int *)malloc(max_hand * sizeof(int));                       //incializa mao do dealer
    *player_card_count = (int *)calloc(1, sizeof(int));                            //contagem cartas player e atribui 0
    *dealer_card_count = (int *)calloc(1, sizeof(int));                            //contagem cartas dealer e atribui 0

    //verifica se foi alocada memória
    if (*player_hand == NULL || *dealer_hand == NULL || *player_card_count == NULL || *dealer_card_count == NULL) {
        free(*player_hand);
        free(*dealer_hand);
        free(*player_card_count);
        free(*dealer_card_count);

        printf ("Alocação de memória falhou.\n");
        return 0;       //se falhar alocação
    }
    else {
        return 1;       //se alocar corretamente
    }
}

//compra carta
void draw (int* hand, int* count) {
    hand[*count] = (rand() % 13) + 1;       //valor maximo é 10, se tirar 10 depois ramdomiza qual carta é
    (*count) ++;
}

//printa carta
void print_card(int card) {
    if (card > 1 && card < 11) {     //se carta for normal
        printf ("[%d] ", card);     //printa valor da carta
    }
    else {      //se carta for da corte ou ais
        SpecialCard special_card = card;
        
        if (special_card == A) {
            printf ("[A] ");
        }
        else if (special_card == J) {
            printf ("[J] ");
        }
        else if (special_card == Q) {
            printf ("[Q] ");
        }
        else if (special_card == K) {
            printf ("[K] ");
        }
    }
}

//printa mao do player com total da soma
void print_hand (int* hand, int* count, int total) {
    for (int i = 0; i < *count; i++) {
        print_card (hand[i]);
    }

    printf ("(Total: %d)\n", total);
}