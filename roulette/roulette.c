#include "roulette.h"

static int MAX_BETS = 46;

//tipos de apostas
typedef enum {
    SIMPLES = 1,
    ALTOBAIXO,
    VERMELHOPRETO,
    PARIMPAR,
    DUZIA
} Apostas;

//cartela de apostas
typedef struct {
    Apostas type;
    int number;
    int value;
} Bet;

//FUNÇÕES UTILITÁRIAS
//são chamadas varias vezes para evitar repetição
static int bet_value (Bet* bet, int bet_count, int* currency);     //input valor da aposta e arruma saldo
static void print_lines();     //printa linhas estéticas no terminal
static bool in_array (int* array, int number, int size);        //verifica se number está no array

//FUNÇÕES ORGANIZACIONAIS
//são chamadas apenas uma vez mas ajudam a manter o fluxo de main() organizado e conciso
static void player_bet (Bet* bet, int bet_count, int* currency);       //input aposta do player
static void waiting_message(int result);       //mensagem da roleta rodando
static void check_bets(Bet* bets, int bet_count, int result, int* currency);       //verifica apostas vencedoras



//valor da aposta
static int bet_value (Bet* bet, int bet_count, int* currency) {
    int value = scanf_bet(currency);        //input valor da aposta

    bet[bet_count].value = value;
    *currency -= value;     //diminui do saldo a quantidade da aposta
}

//printa linhas cosméticas no terminal
static void print_lines() {
    for (int i = 0; i < 70; i++) {
        printf ("-");
    }
    printf ("\n");
}

//checa se numero está no array
static bool in_array (int* array, int number, int size) {
    bool in = false;
    int count = 0;
    while (!in && count < size) {
        if (array[count] == number) {
            in = true;
        }

        count ++;
    }
    return in;
}

//input tipo de aposta do player
//acerta valores por ponteiros
static void player_bet (Bet* bet, int bet_count, int* currency) {
    Apostas player_choice;       // tipo de aposta que player escolheu
    char s1[50], s2[50], s3[50];        //vai ser usado para concluir um printf depois

    printf ("\nEscolha qual aposta fazer:\n\n");
    printf ("[1] Simples\n[2] Baixo | Alto\n[3] Vermelho | Preto\n[4] Par | Ímpar\n[5] Dúzia\n\nDigite aqui: ");        //printa opcoes de apostas

    player_choice = scanf_num(1, 5);      //input escolha de aposta

    //aposta simples
    if (player_choice == SIMPLES) {                                                                     
        printf ("\nVocê escolheu Aposta Simples.\nDigite em qual número deseja apostar (0-36): ");

        int bet_num = scanf_num(0, 36);        //input numero da aposta

        bet[bet_count].type = player_choice;                                                             //arruma valores em na struct bet
        bet[bet_count].number = bet_num;
        bet_value(bet, bet_count, currency);                                                        //arruma valor da aposta em bet

        return;
    }
    //aposta duzia
    else if (player_choice == DUZIA) {
        printf ("\nVocê escolheu Dúzia.\n");
        printf ("Digite [1] para apostar entre 1-12.\nDigite [2] para apostar entre 13-24.\nDigite [3] para apostar entre 25-36.\n\nDigite aqui: ");

        int bet_num = scanf_num(1, 3);        //input numero da aposta

        bet[bet_count].type = player_choice;                                                                    //arruma valores em na struct bet
        bet[bet_count].number = bet_num;
        bet_value(bet, bet_count, currency);                                                        //arruma valor da aposta em bet
        return;
    }
    
    //todas as proximas apostas tem esqueleto igual
    //aposta alto/baixo
    else if (player_choice == ALTOBAIXO) {
        strcpy(s1, "Alto/Baixo"); 
        strcpy(s2, "Alto");      
        strcpy(s3, "Baixo");      
    } 
    //aposta vermelho/preto
    else if (player_choice == VERMELHOPRETO) {
        strcpy(s1, "Vermelho/Preto");
        strcpy(s2, "Vermelho");
        strcpy(s3, "Preto");
    } 
    //aposta par/impar
    else if (player_choice == PARIMPAR) {
        strcpy(s1, "Par/Ímpar");
        strcpy(s2, "Par");
        strcpy(s3, "Ímpar");
    }

    printf ("\nVocê escolheu %s.\n\nDigite [1] para apostar em %s.\nDigite [2] para apostar em %s.\n\nDigite aqui: ", s1, s2, s3);   //printa as opcoes
    
    int bet_num = scanf_num(1, 2);        //input escolha de aposta

    bet[bet_count].type = player_choice;
    bet[bet_count].number = bet_num;
    bet_value (bet, bet_count, currency);
}

//mensagem de rolando a roleta
static void waiting_message(int result) {
    printf("\nGirando a roleta");
    fflush(stdout);

    for (int i = 0; i < 5; i++) {
        sleep(1);
        printf(".");
        fflush(stdout);
    }

    printf("\n\nResultado da Roleta: %d\n\n", result);

    sleep(1);
}

//verifica resultado das apostas
static void check_bets(Bet* bets, int bet_count, int result, int* currency) {
    int red[18] = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};
    int black[18] = {2, 4, 6, 8, 10, 11, 13, 15, 17, 20, 22, 24, 26, 28, 29, 31, 33, 35};
    
    print_lines();
    printf ("Suas apostas\n");                      //printa interface
    print_lines();

    for (int i = 0; i < bet_count; i++) {
        if (bets[i].type == SIMPLES) {        //aposta simples
            printf ("%d. | Aposta Simples | Número %d | Aposta: R$%d | ", i + 1, bets[i].number, bets[i].value);       //printa informações da aposta
            if (bets[i].number == result) {
                printf ("Ganhou |\n");       //printa resultado da aspota
                print_lines();     //printa interface
                *currency += 36 * bets[i].value;        //pagamento 35:1 + 1 dele mesmo q foi descontado
            }
            else {
                printf ("Perdeu |\n");       //printa resultado
                print_lines();     //printa interface
            }
        }
        else if (bets[i].type == ALTOBAIXO) {       //aposta alto/baixo
            printf ("%d. | Aposta Alto/Baixo | Escolheu ", i + 1);      //printa informações da aposta
            if (bets[i].number == 1) {      //printa escolha
                printf ("Alto | Aposta: R$%d | ", bets[i].value);
                if (result > 18) {      //checa se é alto ou baixo
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 2 * bets[i].value;     //pagamento 1:1 + 1 dele mesmo q ja foi descontado
                }
                else {
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
            else {
                printf ("Baixo | Aposta: R$%d | ", bets[i].value);
                if (result < 19) {      //checa se é alto ou baixo
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 2 * bets[i].value;     //pagamento 1:1 + 1 dele mesmo q ja foi descontado
                }
                else {
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
        }
        else if (bets[i].type == VERMELHOPRETO) {       //aposta vermleho/preto
            printf ("%d. | Aposta Vermelho/Preto | Escolheu ", i + 1);
            if (bets[i].number == 1) {      //se escolheu vermelho
                printf ("Vermelho | Aposta: R$%d | ", bets[i].value);
                if (in_array(red, result, 18)) {       //verifica se é vermelho
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 2 * bets[i].value;     //pagamento 1:1 + 1 dele mesmo q ja foi descontado
                }
                else {      //se for preto
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
            else {      //se escolheu preto
                printf ("Preto | Aposta: R$%d | ", bets[i].value);
                if (in_array(black, result, 18)) {       //verifica se é preto
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 2 * bets[i].value;     //pagamento 1:1 + 1 dele mesmo q ja foi descontado
                }
                else {      //se for vermelho
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
        }
        else if (bets[i].type == PARIMPAR) {       //aposta par/impar
            printf ("%d. | Aposta Par/Ímpar | Escolheu ", i + 1);
            if (bets[i].number == 1) {      //se escolheu par
                printf ("Par | Aposta: R$%d | ", bets[i].value);
                if ((result % 2) == 0) {        //verifica se resultado é par
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 2 * bets[i].value;     //pagamento 1:1 + 1 dele mesmo q ja foi descontado
                }               //se resultado é impar
                else {
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
            else {      //se escolheu impar
                printf ("Ímpar | Aposta: R$%d | ", bets[i].value);
                if ((result % 2) != 0) {        //verifica se resultado é impar
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 2 * bets[i].value;     //pagamento 1:1 + 1 dele mesmo q ja foi descontado
                }               //se resultado é par
                else {
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
        }
        else if (bets[i].type == DUZIA) {      //aposta duzia
            printf ("%d. | Aposta em uma Dúzia | Escolheu ", i + 1);
            if (bets[i].number == 1) {      //se escolheu 1-12
                printf ("1-12 | Aposta: R$%d | ", bets[i].value);
                if (result >= 1 && result <= 12) {        //verifica resultado
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 3 * bets[i].value;     //pagamento 2:1 + 1 dele mesmo q ja foi descontado
                }               //se perdeu
                else {
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
            else if (bets[i].number == 2) {      //se escolheu 13-24
                printf ("13-24 | Aposta: R$%d | ", bets[i].value);
                if (result >= 13 && result <= 24) {        //verifica resultado
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 3 * bets[i].value;     //pagamento 2:1 + 1 dele mesmo q ja foi descontado
                }               //se perdeu
                else {
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
            else {      //se escolehu 25-36
                printf ("25-36 | Aposta: R$%d | ", bets[i].value);
                if (result >= 25) {        //verifica resultado
                    printf ("Ganhou |\n");
                    print_lines();     //printa interface
                    *currency += 3 * bets[i].value;     //pagamento 2:1 + 1 dele mesmo q ja foi descontado
                }               //se perdeu
                else {
                    printf ("Perdeu |\n");
                    print_lines();     //printa interface
                }
            }
        }
    }
}

//simula roleta
int start_roulette(int* currency) {
    Bet* bets = (Bet *)malloc(MAX_BETS * sizeof(Bet));
    if (bets == NULL) {
        printf ("Alocação de memória falhou.\n");
        return MEMORY_ERROR;
    }

    int bet_count = 0;
    char choice = 0;

    //rodada de apostas
    while (choice != 'N') {
        player_bet(bets, bet_count, currency);      //escolha da aposta
        bet_count ++;

        //verifica se passou maximo de apostas
        if (bet_count > MAX_BETS) {                                             
            printf ("\nVocê chegou no limite de apostas nessa rodada.\n");
            break;
        }

        //verifica se acabou saldo
        if (*currency < 1) {        
            printf ("\nVocê não tem mais saldo para fazer outras apostas.\n");
            break;
        }

        printf ("\nVocê quer fazer mais apostas para esta rodada?");
        
        choice = scanf_sn();        //input escolha do player
    }

    //sorteia o numero
    int result = rand() % 37;

    //printa resultado
    waiting_message(result);

    //verifica apostas
    check_bets(bets, bet_count, result, currency);

    free(bets);
}