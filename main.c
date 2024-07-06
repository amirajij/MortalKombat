#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

// Definição das estruturas e constantes globais
typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct HistoryNode {
    char value[10];
    struct HistoryNode* next;
} HistoryNode;

typedef struct {
    Node* vida;
    Node* estamina;
    HistoryNode* historico;
    int historico_size;
    int multiplicador;
} Jogador;

int tabela_de_ataques[12][12] = {
    {0, 4, 8, 12, 16, -20, -16, 12, -8, -4, 0, 20},
    {-4, 0, 4, 8, 12, 16, -20, -16, -12, 16, 0, 20},
    {-8, -4, 0, 4, 8, 12, 16, -20, -16, 12, 0, 20},
    {-12, -8, -4, 0, 4, -8, 12, 16, 20, -16, 0, 20},
    {-16, -12, -8, -4, 0, 4, -8, 12, 16, 20, 0, 20},
    {20, -16, -12, 8, -4, 0, 4, 8, 12, -16, 0, 20},
    {16, 20, -16, -12, 8, -4, 0, -8, -4, -8, 0, 20},
    {-12, 16, 20, -16, -12, -8, 8, 0, 16, -20, 0, 20},
    {8, 12, 16, -20, -16, -12, 4, -16, 0, 20, 0, 20},
    {4, -16, -12, 16, -20, 16, 8, 20, -20, 0, 0, 20},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-20, -20, -20, -20, -20, -20, -20, -20, -20, -20, 0, 0}
};

int ataques[12] = {
    'Z', 'P', 'A', 'E', 'T', 'R', 'C', 'B', 'O', 'M', 'D', ' '
};

// Implementação das funções auxiliares
Node* addNode(Node* head, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Erro ao alojar a memória.\n");
        exit(1);
    }
    newNode->value = value;
    newNode->next = head;
    return newNode;
}

HistoryNode* addHistoryNode(HistoryNode* head, const char* value, int* historico_size) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    HistoryNode* current;
    if (!newNode) {
        printf("Erro ao alojar memória.\n");
        exit(1);
    }
    strcpy(newNode->value, value);
    newNode->next = NULL;

    if (head == NULL) {
        *historico_size = 1;
        return newNode;
    }

    if (*historico_size >= 20) {
        // Remove o primeiro nó
        HistoryNode* temp = head;
        head = head->next;
        free(temp);
        (*historico_size)--;
    }

    current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    (*historico_size)++;

    return head;
}

void freeList(Node* head) {
    Node* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void freeHistoryList(HistoryNode* head) {
    HistoryNode* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void limparHistorico(Jogador* jogador) {
    freeHistoryList(jogador->historico); 
    jogador->historico = NULL;
    jogador->historico_size = 0;
}

int calcularDano(char attack1, char attack2) {
    int index1 = -1, index2 = -1;
    int i;
    for (i = 0; i < 12; i++) {
        if (ataques[i] == attack1) index1 = i;
        if (ataques[i] == attack2) index2 = i;
    }
    if (index1 == -1 || index2 == -1) return 0; // Ataque invalido
    return tabela_de_ataques[index1][index2]; // Jogador1 - coluna | Jogador2 - linha
}

// Implementação das funções do jogo
Jogador* inicializarJogadores() {
    Jogador* player = (Jogador*)malloc(sizeof(Jogador));
    if (!player) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    player->vida = addNode(NULL, 1000);
    player->estamina = addNode(NULL, 1000);
    player->historico = NULL;
    player->historico_size = 0;
    player->multiplicador = 1;
    return player;
}

void atualizarMultiplicador(Jogador* jogador) {
    int estamina = jogador->estamina->value;
    if (estamina > 750) jogador->multiplicador = 1;
    else if (estamina > 500) jogador->multiplicador = 2;
    else if (estamina > 250) jogador->multiplicador = 3;
    else jogador->multiplicador = 4;
}

int verififcarCombo(const char* jogada) {
    const char* combos_validos[] = {"ARROZAO", "DADBAD", "STTEACC", "TATAPAAA"};
    int i;
    for (i = 0; i < 4; i++) {
        if (strcmp(jogada, combos_validos[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void adicionarHistorico(Jogador* jogador, const char* action) {
    jogador->historico = addHistoryNode(jogador->historico, action, &jogador->historico_size);
}

void processarDescansar(Jogador* jogador) {
    jogador->estamina = addNode(jogador->estamina, jogador->estamina->value + 25);
    if (jogador->estamina->value > 1000) jogador->estamina = addNode(jogador->estamina, 1000);
}

void processarDefender(Jogador* jogador){
    jogador->estamina = addNode(jogador->estamina, jogador->estamina->value - 10);
    if (jogador->estamina->value < 0) jogador->estamina = addNode(jogador->estamina, 0);
    jogador->vida = addNode(jogador->vida, jogador->vida->value + (10 * jogador->multiplicador));
    if (jogador->vida->value > 1000) jogador->vida = addNode(jogador->vida, 1000);
}

void processarCombo(Jogador* jogador1, Jogador* jogador2, const char* combo1, const char* combo2) {
    if (strcmp(combo1, "ARROZAO") == 0) {
        jogador1->estamina = addNode(jogador1->estamina, jogador1->estamina->value - 500);
        jogador2->vida = addNode(jogador2->vida, jogador2->vida->value - 500);
    } else if (strcmp(combo1, "DADBAD") == 0) {
        jogador1->estamina = addNode(jogador1->estamina, jogador1->estamina->value - 400);
        jogador2->vida = addNode(jogador2->vida, jogador2->vida->value - 400);
    } else if (strcmp(combo1, "STTEACC") == 0) {
        jogador1->estamina = addNode(jogador1->estamina, jogador1->estamina->value - 300);
        jogador2->vida = addNode(jogador2->vida, jogador2->vida->value - 300);
    } else if (strcmp(combo1, "TATAPAAA") == 0) {
        jogador1->estamina = addNode(jogador1->estamina, jogador1->estamina->value - 200);
        jogador2->vida = addNode(jogador2->vida, jogador2->vida->value - 200);
    }

    if (strcmp(combo2, "ARROZAO") == 0) {
        jogador2->estamina = addNode(jogador2->estamina, jogador2->estamina->value - 500);
        jogador1->vida = addNode(jogador1->vida, jogador1->vida->value - 500);
    } else if (strcmp(combo2, "DADBAD") == 0) {
        jogador2->estamina = addNode(jogador2->estamina, jogador2->estamina->value - 400);
        jogador1->vida = addNode(jogador1->vida, jogador1->vida->value - 400);
    } else if (strcmp(combo2, "STTEACC") == 0) {
        jogador2->estamina = addNode(jogador2->estamina, jogador2->estamina->value - 300);
        jogador1->vida = addNode(jogador1->vida, jogador1->vida->value - 300);
    } else if (strcmp(combo2, "TATAPAAA") == 0) {
        jogador2->estamina = addNode(jogador2->estamina, jogador2->estamina->value - 200);
        jogador1->vida = addNode(jogador1->vida, jogador1->vida->value - 200);
    }

    adicionarHistorico(jogador1, combo1);
    adicionarHistorico(jogador2, combo2);

    printf("[%s,%s]\n", combo1, combo2);
}

void aplicarCheatCode(Jogador* jogador1, Jogador* jogador2, const char* codigo) {
    if (strcmp(codigo, "Modo-Jesus") == 0) {
        jogador1->vida = addNode(jogador1->vida, 1000);
        jogador1->estamina = addNode(jogador1->estamina, 1000);
        jogador2->vida = addNode(jogador2->vida, 1000);
        jogador2->estamina = addNode(jogador2->estamina, 1000);
        limparHistorico(jogador1);
        limparHistorico(jogador2);
    } else if (strncmp(codigo, "Alt-F4", 6) == 0 && isdigit(codigo[6])) {
        int x = atoi(&codigo[6]); // Converte string para um int
        if (x > 0) {
            jogador1->estamina = addNode(jogador1->estamina, jogador1->estamina->value + x);
            if (jogador1->estamina->value > 1000) jogador1->estamina = addNode(jogador1->estamina, 1000);
        }
    } else if (strncmp(codigo, "Kebab", 5) == 0 && isdigit(codigo[5])) {
        int x = atoi(&codigo[5]);
        if (x > 0) {
            jogador2->estamina = addNode(jogador2->estamina, jogador2->estamina->value + x);
            if (jogador2->estamina->value > 1000) jogador2->estamina = addNode(jogador2->estamina, 1000);
        }
    } else if (strncmp(codigo, "Hiroshima", 9) == 0 && isdigit(codigo[9])) {
        int x = atoi(&codigo[9]);
        if (x > 0) {
            jogador1->vida = addNode(jogador1->vida, jogador1->vida->value + x);
            if (jogador1->vida->value > 1000) jogador1->vida = addNode(jogador1->vida, 1000);
        }
    } else if (strncmp(codigo, "Nood-Mode", 9) == 0 && isdigit(codigo[9])) {
        int x = atoi(&codigo[9]);
        if (x > 0) {
            jogador2->vida = addNode(jogador2->vida, jogador2->vida->value + x);
            if (jogador2->vida->value > 1000) jogador2->vida = addNode(jogador2->vida, 1000);
        }
    }
}

void processarAtaques(Jogador* jogador1, Jogador* jogador2, const char* jogada1, const char* jogada2) {
    size_t i = 0, j = 0;
    size_t length1 = strlen(jogada1);
    size_t length2 = strlen(jogada2);
    size_t max_length = (length1 > length2) ? length1 : length2;
    char ataque1[2];
    char ataque2[2];
    int dano;

    while (i < max_length) {
        char char1 = (i < length1) ? jogada1[i] : ' ';
        char char2 = (i < length2) ? jogada2[j] : ' ';

        dano = calcularDano(char1, char2);
        if (dano > 0) {
            jogador2->vida = addNode(jogador2->vida, jogador2->vida->value - (dano * jogador2->multiplicador));
            if (jogador2->vida->value < 0) jogador2->vida = addNode(jogador2->vida, 0);
        } else if (dano < 0) {
            jogador1->vida = addNode(jogador1->vida, jogador1->vida->value + (dano * jogador1->multiplicador));
            if (jogador1->vida->value < 0) jogador1->vida = addNode(jogador1->vida, 0);
        }

        if (char1 != 'D' && char1 != ' ') {
            jogador1->estamina = addNode(jogador1->estamina, jogador1->estamina->value - 25);
            if (jogador1->estamina->value < 0) jogador1->estamina = addNode(jogador1->estamina, 0);
        }
        if (char2 != 'D' && char2 != ' ') {
            jogador2->estamina = addNode(jogador2->estamina, jogador2->estamina->value - 25);
            if (jogador2->estamina->value < 0) jogador2->estamina = addNode(jogador2->estamina, 0);
        }

        if (char1 == 'D') {
            processarDefender(jogador1);
        }
        if (char2 == 'D') {
            processarDefender(jogador2);
        }

        if (char1 == ' ') {
            processarDescansar(jogador1);
        }
        if (char2 == ' ') {
            processarDescansar(jogador2);
        }

        ataque1[0] = char1;
        ataque1[1] = '\0';
        adicionarHistorico(jogador1, ataque1);
        ataque2[0] = char2;
        ataque2[1] = '\0';
        adicionarHistorico(jogador2, ataque2);
        
        printf("[%c,%c]", char1, char2);
        i++;
        j++;
    }

    printf("\n");
}

void processarJogadas(Jogador* jogador1, Jogador* jogador2, const char* jogada1, const char* jogada2) {
    int combo1 = verififcarCombo(jogada1);
    int combo2 = verififcarCombo(jogada2);

    if (combo1 && combo2) {
        processarCombo(jogador1, jogador2, jogada1, jogada2);
    } else if (combo1 && !combo2) {
        processarCombo(jogador1, jogador2, jogada1, " ");
        processarDescansar(jogador2);
    } else if (!combo1 && combo2) {
        processarCombo(jogador1, jogador2, " ", jogada2);
        processarDescansar(jogador1);
    } else {
        processarAtaques(jogador1, jogador2, jogada1, jogada2);
    }

    if (jogador1->vida->value <= 0) {
        printf("Jogador 2 venceu o jogo!\n");
        return;
    } else if (jogador2->vida->value <= 0) {
        printf("Jogador 1 venceu o jogo!\n");
        return;
    }
}

void exibirHistorico(Jogador* jogador) {
    HistoryNode* current = jogador->historico;
    while (current != NULL) {
        printf("%s", current->value);
        current = current->next;
    }
    printf("\n");
}

int entradaValida(const char* jogada) {
    if (verififcarCombo(jogada)) {
        return 1;
    }

    if (strlen(jogada) > 4) {
        return 0;
    }

    for (int i = 0; jogada[i] != '\0'; i++) {
        int valido = 0;
        for (int j = 0; j < 12; j++) {
            if (jogada[i] == ataques[j] || jogada[i] == ' ') {
                valido = 1;
                break;
            }
        }
        if (!valido) return 0;
    }
    return 1;
}

int main() {
    Jogador* jogador1 = inicializarJogadores();
    Jogador* jogador2 = inicializarJogadores();
    char jogada1[100], jogada2[100];
    int combo1, combo2;

    while (jogador1->vida->value > 0 && jogador2->vida->value > 0) {
        atualizarMultiplicador(jogador1);
        atualizarMultiplicador(jogador2);

        while (1) {
            printf("P#1 [%d|%d] (x%d)\n", jogador1->vida->value, jogador1->estamina->value, jogador1->multiplicador);
            printf("H: ");
            exibirHistorico(jogador1);
            printf("I: ");
            if (scanf("%s", jogada1) != 1) {
                // Liberta memória antes de sair
                freeList(jogador1->vida);
                freeList(jogador1->estamina);
                freeHistoryList(jogador1->historico);
                free(jogador1);
                
                freeList(jogador2->vida);
                freeList(jogador2->estamina);
                freeHistoryList(jogador2->historico);
                free(jogador2);
                return 0;
            }

            if (strncmp(jogada1, "Modo-Jesus", 10) == 0 ||
                strncmp(jogada1, "Alt-F4", 6) == 0 ||
                strncmp(jogada1, "Kebab", 5) == 0 ||
                strncmp(jogada1, "Hiroshima", 9) == 0 ||
                strncmp(jogada1, "Nood-Mode", 9) == 0) {
                aplicarCheatCode(jogador1, jogador2, jogada1);
                continue;  // Permitir nova leitura
            }

            combo1 = verififcarCombo(jogada1);
            if (combo1) {
                if (jogador1->estamina->value < 750) {
                    printf("Estamina insuficiente\n");
                    continue;
                }
            }

            if (!entradaValida(jogada1)) {
                printf("Entrada invalida\n");
                // Liberta memória antes de sair
                freeList(jogador1->vida);
                freeList(jogador1->estamina);
                freeHistoryList(jogador1->historico);
                free(jogador1);
                
                freeList(jogador2->vida);
                freeList(jogador2->estamina);
                freeHistoryList(jogador2->historico);
                free(jogador2);
                return 0;
            }
            break;
        }

        while (1) {
            printf("P#2 [%d|%d] (x%d)\n", jogador2->vida->value, jogador2->estamina->value, jogador2->multiplicador);
            printf("H: ");
            exibirHistorico(jogador2);
            printf("I: ");
            if (scanf("%s", jogada2) != 1) {
                // Liberta memória antes de sair
                freeList(jogador1->vida);
                freeList(jogador1->estamina);
                freeHistoryList(jogador1->historico);
                free(jogador1);
                
                freeList(jogador2->vida);
                freeList(jogador2->estamina);
                freeHistoryList(jogador2->historico);
                free(jogador2);
                return 0;
            }

            if (strncmp(jogada2, "Modo-Jesus", 10) == 0 ||
                strncmp(jogada2, "Alt-F4", 6) == 0 ||
                strncmp(jogada2, "Kebab", 5) == 0 ||
                strncmp(jogada2, "Hiroshima", 9) == 0 ||
                strncmp(jogada2, "Nood-Mode", 9) == 0) {
                aplicarCheatCode(jogador1, jogador2, jogada2);
                continue;  // Permite nova leitura
            }

            combo2 = verififcarCombo(jogada2);
            if (combo2) {
                if (jogador2->estamina->value < 750) {
                    printf("Estamina insuficiente\n");
                    continue;
                }
            }

            if (!entradaValida(jogada2)) {
                printf("Entrada invalida\n");
                // Libera memória antes de sair
                freeList(jogador1->vida);
                freeList(jogador1->estamina);
                freeHistoryList(jogador1->historico);
                free(jogador1);
                
                freeList(jogador2->vida);
                freeList(jogador2->estamina);
                freeHistoryList(jogador2->historico);
                free(jogador2);
                return 0;
            }
            break;
        }

        processarJogadas(jogador1, jogador2, jogada1, jogada2);
        if (jogador1->vida->value <= 0 || jogador2->vida->value <= 0) break;
    }

    // Liberta memória antes de sair do programa
    freeList(jogador1->vida);
    freeList(jogador1->estamina);
    freeHistoryList(jogador1->historico);
    free(jogador1);

    freeList(jogador2->vida);
    freeList(jogador2->estamina);
    freeHistoryList(jogador2->historico);
    free(jogador2);

    return 0;
}
