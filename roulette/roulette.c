#include "roulette.h"

int temp;

//input tipo de aposta do player
//acerta valores por ponteiros
void player_bet (Bet* bet, int bet_count, int* currency) {
    int bet_type = 0;                                                                           // tipo de aposta que player escolheu
    char s1[50], s2[50], s3[50];                                                                //vai ser usado para concluir um printf depois

    printf ("\nEscolha qual aposta fazer:\n\n");
    printf ("[1] Simples\n[2] Baixo | Alto\n[3] Vermelho | Preto\n[4] Par | Ímpar\n[5] Dúzia\n\nDigite aqui: ");             //printa opcoes de apostas

    while (bet_type < 1 || bet_type > 5) {                                                                      //verifica se input é valido
        temp = scanf (" %d", &bet_type);
        while (getchar() != '\n');      //limpa buffer

        if (temp != 1 || bet_type < 1 || bet_type > 5) {
            printf ("Comando inválido. Digite o número que corresponde a aposta escolhida.\n\nDigite aqui: ");
        }
    }

    //aposta simples
    if (bet_type == 1) {                                                                     
        printf ("\nVocê escolheu Aposta Simples.\nDigite em qual número deseja apostar (0-36): ");
        int n = -1;                                                                                 //numero da aposta
        while (n < 0 || n > 36) {                                                                   //roleta europeia tem apenas um 0, entao num vai de 0 a 36
            temp = scanf ("%d", &n);                                                                  
            if (temp != 1 || n < 0 || n > 36) {                                                     //verifica input
                printf ("\nNúmero inválido. Digite um número de 0 a 36.\n\nDigite aqui: ");
            }
            while (getchar() != '\n');
        }
        bet[bet_count].type = bet_type;                                                             //arruma valores em na struct bet
        bet[bet_count].number = n;
        bet_value(bet, bet_count, currency);                                                        //arruma valor da aposta em bet

        return;
    }
    //aposta duzia
    else if (bet_type == 5) {
        printf ("\nVocê escolheu Dúzia.\n");
        printf ("Digite [1] para apostar entre 1-12.\nDigite [2] para apostar entre 13-24.\nDigite [3] para apostar entre 25-36.\n\nDigite aqui: ");

        int n;                                                                                      //escolha da aposta
        temp = scanf ("%d", &n);
        while (1) {
            if (temp != 1  || n < 1 || n > 3) {
                printf ("\nNúmero inválido. Digite [1], [2] ou [3].\n\nDigite aqui: ");
            }
            else {
                break;
            }
        }
        bet[bet_count].type = bet_type;                                                                    //arruma valores em na struct bet
        bet[bet_count].number = n;
        bet_value(bet, bet_count, currency);                                                        //arruma valor da aposta em bet
        return;
    }
    
    //todas as proximas apostas tem esqueleto igual só muda o printf, portanto printf no if e o resto tudo junto

    //templates para substituir em um printf depois
    //aposta alto/baixo
    else if (bet_type == 2) {
        strcpy(s1, "Alto/Baixo"); 
        strcpy(s2, "Alto");      
        strcpy(s3, "Baixo");      
    } 
    //aposta vermelho/preto
    else if (bet_type == 3) {
        strcpy(s1, "Vermelho/Preto");
        strcpy(s2, "Vermelho");
        strcpy(s3, "Preto");
    } 
    //aposta par/impar
    else if (bet_type == 4) {
        strcpy(s1, "Par/Ímpar");
        strcpy(s2, "Par");
        strcpy(s3, "Ímpar");
    }

    printf ("\nVocê escolheu %s.\n\nDigite [1] para apostar em %s.\nDigite [2] para apostar em %s.\n\nDigite aqui: ", s1, s2, s3);   //printa as opcoes
    
    int n;
    while (1) {                                                                             //input opcoes                                              
        temp = scanf ("%d", &n);

        if (temp != 1 || n < 1 || n > 2) {                                                  //verifica input
            printf ("\nNúmero inválido. Digite [1] ou [2].\n\nDigite aqui: ");
            while (getchar() != '\n');
        }
        else {
            break;
        }
    }
    bet[bet_count].type = bet_type;
    bet[bet_count].number = n;
    bet_value (bet, bet_count, currency);
}

//valor da aposta
int bet_value (Bet* bet, int bet_count, int* currency) {
    int value = scanf_bet(currency);        //input valor da aposta

    bet[bet_count].value = value;           //acerta valor da aposta na variavel bets
    *currency -= value;     //diminui do saldo a quantidade da aposta
}

//mensagem de rolando a roleta
void waiting_message(int result) {
    printf("\nGirando a roleta");
    fflush(stdout);     //garante que a mensagem seja printada imaediatamente

    for (int i = 0; i < 5; i++) {       //loop para suspense
        sleep(1);       //espera 1 segundo
        printf(".");    //printa um ponto
        fflush(stdout);     //garante que seja printado imediatamente
    }

    printf("\n\nResultado da Roleta: %d\n\n", result);    //printa resultado

    sleep(1);       //espera 1 segundo
}

//verifica resultado das apostas e acerta o saldo por ponteiros
void check_bets(Bet* bets, int bet_count, int result, int* currency) {
    int red[18] = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};      //vetor com numeros vermelhos para comparar com resultado
    int black[18] = {2, 4, 6, 8, 10, 11, 13, 15, 17, 20, 22, 24, 26, 28, 29, 31, 33, 35};   //vetor com numeros pretos para comparar com resultado
    
    print_lines();
    printf ("Suas apostas\n");                      //printa interface
    print_lines();

    for (int i = 0; i < bet_count; i++) {
        if (bets[i].type == 1) {        //aposta simples
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
        else if (bets[i].type == 2) {       //aposta alto/baixo
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
        else if (bets[i].type == 3) {       //aposta vermleho/preto
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
        else if (bets[i].type == 4) {       //aposta par/impar
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
        else {      //aposta duzia
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

//checa se numero está no array
int in_array (int* array, int number, int size) {
    int in = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] == number) {
            in = 1;
            break;      //se encontrou o numero, quebra o loop
        }
    }
    return in;
}

//printa linhas para ficar bonito terminal
void print_lines() {
    for (int i = 0; i < 70; i++) {
        printf ("-");
    }
    printf ("\n");
}

void start_roulette(int* currency) {
    Bet* bets = (Bet *)malloc(MAX_BETS * sizeof(Bet));                          //aloca as apostas
    if (bets == NULL) {                                       // Check if memory allocation failed
        return;
    }

    int bet_count = 0;                                                          // qtd de apostas
    char choice;                                                                //escolha do player

    //rodada de apostas
    while (1) {
        player_bet(bets, bet_count, currency);
        bet_count ++;

        if (bet_count > MAX_BETS) {                                             //verifica se passou max apostas
            printf ("\nVocê chegou no limite de apostas nessa rodada.\n");
            break;
        }

        if (*currency < 1) {
            printf ("\nVocê não tem mais saldo para fazer outras apostas.\n");
            break;
        }

        printf ("\nVocê quer fazer mais apostas para esta rodada? [S] Sim | [N] Não\n\nDigite aqui: ");
        
        choice = scanf_sn();        //input escolha do player

        if (choice == 'N') {
            break;
        }
    }

    //sorteia o numero
    int result = rand() % 37;

    waiting_message(result);        //printa o resultado

    check_bets(bets, bet_count, result, currency);

    free(bets);
}