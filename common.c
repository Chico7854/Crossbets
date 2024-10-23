#include "common.h"

//apaga espaços no começo e fim da string
char* trim_space(char* str) {
    char *end;

    //apaga espaços no começo da string
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    // Remove o caractere de nova linha, se houver
    if ((end = strchr(str, '\n')) != NULL) {
        *end = '\0';
    }

    //apaga espaços no fim da string
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    *(end+1) = '\0';

    return str;
}

//verifica se input esta entre os numeros válidos
int scanf_num(int min, int max) {
    int choice = INVALID;     //escolha de numero do user

    while (choice == INVALID) {     //while loop ate scanf ser valido
        char input[10];     //input do user

        //verificacao se input é numero
        if (fgets(input, sizeof(input), stdin)) {     // Lê a entrada do usuário e verifica se fgets funcionou

            // Limpa o buffer se o input for maior que o tamanho do buffer
            if (!strchr(input, '\n')) {
                while (getchar() != '\n');
            }

            //limpa espaços sobrando
            char trimmed_input[10];
            strcpy(trimmed_input, trim_space(input));

            // Verifica se a entrada contém apenas dígitos
            bool isnumber = true;
            for (int i = 0; trimmed_input[i] != '\0' && trimmed_input[i] != '\n'; i++) {
                if (!isdigit(trimmed_input[i])) {
                isnumber = false;      //se for invalido
                }
            }     

            if (isnumber) {        //se input for numero
                sscanf(trimmed_input, "%d", &choice);        //converte a string para um inteiro 
                if (choice < min || choice > max) {       //verifica se valor esta em intervalo invalido
                    choice = INVALID;       //se for invalido retorna ao valor INVALID para rodar loop de novo
                }
            }
        }

        if (choice == INVALID) {        //verifica se input foi validado
            if ((max - min) == 1) {     //se tiver apenas duas opçoes de escolha
                printf ("\nComando inválido. Digite [%d] ou [%d].\n\nDigite aqui: ", min, max);
            }
            else {      //se tem 3 opcoes ou mais de escolha
                printf ("\nComando inválido. Digite um número de [%d] a [%d].\n\nDigite aqui: ", min, max);
            }
        }
    }
}

//scanf para pergunta de sim ou nao
char scanf_sn () {
    char choice = INVALID;

    printf("[S] [N]\n\nDigite aqui: ");

    while (choice == INVALID) {     //verifica input
        char input[10];

        if (fgets(input, sizeof(input), stdin)) {     //le entrada do usuario e verifica se fgets funcionou
            if (!strchr(input, '\n')) {        //verifica se tem algo a mais no buffer, se achar \n na string signifca q nao tem
                while (getchar() != '\n');      //limpa buffer
            }

            char trimmed_input[10];
            strcpy(trimmed_input, trim_space(input));       //apaga espaços extras

            int x = strlen(trimmed_input);

            if (strlen(trimmed_input) == 1) {        //verifica se input tem apenas 1 caractere
                sscanf(trimmed_input, "%c", &choice);        //converte input para caractere

                choice = toupper(choice);       //converte caractere para maiusculo
                
                if (choice != 'S' && choice != 'N') {       //verifica se input é S ou N
                    choice = INVALID;       //se for invalido retorna para valor do while
                }
            }
        }

        if (choice == INVALID) {
            printf ("\nComando inválido. Digite [S] ou [N].\n\nDigite aqui: ");
        }
    }

    return choice;
}

//scanf para receber apostas
int scanf_bet(int* currency) {
    int bet_value = INVALID;     

    printf ("\nSaldo atual: R$%d\n", *currency);
    printf ("Digite o valor da aposta.\n\nDigite aqui: R$");

    //validação do input
    while (bet_value == INVALID) {
        char input[10];     //input do user

        //verificacao se input é numero
        if (fgets(input, sizeof(input), stdin)) {     // Lê a entrada do usuário e verifica se fgets funcionou

            // Limpa o buffer se o input for maior que o tamanho do buffer
            if (!strchr(input, '\n')) {
                while (getchar() != '\n');
            }

            char trimmed_input[10];
            strcpy(trimmed_input, trim_space(input));       //limpa espaços sobrando

            // Verifica se a entrada contém apenas dígitos
            bool isnumber = true;
            for (int i = 0; trimmed_input[i] != '\0' && trimmed_input[i] != '\n'; i++) {
                if (!isdigit(trimmed_input[i])) {
                    isnumber = false;      //se for invalido
                }
            }     

            if (isnumber) {
                sscanf(trimmed_input, "%d", &bet_value);
                if (bet_value < 0 || bet_value > *currency) {       //verifica se valor esta em intervalo invalido
                    bet_value = INVALID;        //se for invalido retorna ao valor INVALID para rodar loop de novo
                }
            }
        }
        
        if (bet_value == INVALID) {     //verifica se input foi validado
            printf ("Comando inválido. Tente novamente.\n\nDigite aqui: R$");       //se input esta incorreto printa e repete loop
        }
    }
}

//aloca memória para jogos com baralho e uma mão de cartas para player e outra para dealer
int initialize_hands(int** player_hand, int** dealer_hand, int** player_card_count, int** dealer_card_count, int max_hand) {
    *player_hand = (int *)malloc(max_hand * sizeof(int));                       //inicializa mao de jogador
    *dealer_hand = (int *)malloc(max_hand * sizeof(int));                       //incializa mao do dealer
    *player_card_count = (int *)calloc(1, sizeof(int));                            //contagem cartas player e atribui 0
    *dealer_card_count = (int *)calloc(1, sizeof(int));                            //contagem cartas dealer e atribui 0

    if (*player_hand == NULL || *dealer_hand == NULL || *player_card_count == NULL || *dealer_card_count == NULL) {
        free(*player_hand);
        free(*dealer_hand);
        free(*player_card_count);
        free(*dealer_card_count);

        printf ("Alocação de memória falhou.\n");
        return MEMORY_ERROR;
    }
    else {
        return true;
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