#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MISSAO 120
#define MAX_JOGADORES 6

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    char *missao; // alocada dinamicamente
} Jogador;

// ------------------ PROTÓTIPOS ------------------ 
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirTerritorios(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoriaTerritorios(Territorio* mapa);
void liberarMemoriaJogadores(Jogador* jogadores, int numJogadores);

void atribuirMissao(char* destino, char* missoes[], int totalMissoes,
                    Territorio* mapa, int n, const char* corJogador, const char* nomeJogador);
int verificarMissao(const char* missao, Territorio* mapa, int n);

char** criarTemplatesMissao(int *total);
int contarTerritoriosCor(Territorio* mapa, int n, const char* cor);
int contarTerritoriosSeguidos(Territorio* mapa, int n, const char* cor, int alvo);
int totalTropasCor(Territorio* mapa, int n, const char* cor);

// ------------------ IMPLEMENTAÇÕES ------------------ 

void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nCadastro do Territorio %d\n", i + 1);

        printf("Digite o nome do Territorio: ");
        scanf(" %[^\n]", mapa[i].nome);

        printf("Digite a cor do exercito: ");
        scanf(" %[^\n]", mapa[i].cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n==============================\n");
    printf("LISTA DE TERRITORIOS\n");
    printf("==============================\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do exercito: %s\n", mapa[i].cor);
        printf("Quantidade de tropas: %d\n", mapa[i].tropas);
    }
    printf("\n==============================\n");
}

void atacar(Territorio* atacante, Territorio* defensor) {
    // Simula lançamento de dado 1-6 para cada lado
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\nAtaque de %s (%s) para %s (%s)\n",
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);
    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Ataque bem-sucedido!\n");
        // defensor muda de cor para a cor do atacante
        strcpy(defensor->cor, atacante->cor);
        // defensor recebe metade das tropas do atacante (arredondamento trunc)
        defensor->tropas = atacante->tropas / 2;
        // atacante mantém suas tropas (opcional: poderia perder tropas ao mover)
    } else {
        printf("Ataque falhou!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// Libera vetor de territorios alocado dinamicamente 
void liberarMemoriaTerritorios(Territorio* mapa) {
    free(mapa);
}

//Libera missões alocadas dos jogadores e o array de jogadores (se alocado externamente) 
void liberarMemoriaJogadores(Jogador* jogadores, int numJogadores) {
    for (int i = 0; i < numJogadores; i++) {
        if (jogadores[i].missao != NULL) {
            free(jogadores[i].missao);
            jogadores[i].missao = NULL;
        }
    }
}

/* Cria templates (strings base) para missões.
   Retorna array alocado com ponteiros para literais (não precisam de free) e seta total. */
char** criarTemplatesMissao(int *total) {
    *total = 5;
    char **templates = malloc(sizeof(char*) * (*total));
    templates[0] = "POSSUIR %d TERRITORIOS DA COR %s";          // precisa número e cor do jogador
    templates[1] = "CONQUISTAR %d SEGUIDOS DA COR %s";          // precisa número e cor do jogador
    templates[2] = "TER AO MENOS %d TROPAS DA COR %s";         // precisa número e cor do jogador
    templates[3] = "ELIMINAR TODAS AS TROPAS DA COR %s";       // precisa cor alvo
    templates[4] = "CONTROLAR O TERRITORIO %s";                // precisa nome do território específico
    return templates;
}

// Conta quantos territórios tem uma cor específica 
int contarTerritoriosCor(Territorio* mapa, int n, const char* cor) {
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) cnt++;
    }
    return cnt;
}

/* Conta se existe uma sequência de 'alvo' territórios consecutivos com cor dada.
   Simples: sequência no vetor (índices adjacentes). */
int contarTerritoriosSeguidos(Territorio* mapa, int n, const char* cor, int alvo) {
    int consec = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) {
            consec++;
            if (consec >= alvo) return 1; // cumpriu
        } else {
            consec = 0;
        }
    }
    return 0;
}

// Soma total de tropas de uma cor 
int totalTropasCor(Territorio* mapa, int n, const char* cor) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) total += mapa[i].tropas;
    }
    return total;
}

/* Atribui uma missão ao jogador.
   destino deve apontar para buffer já alocado com tamanho >= MAX_MISSAO.
   missoes[] são templates; totalMissoes é o número de templates.
   mapa/n usados para escolher cores/nomes válidos ao montar a missão.
   corJogador e nomeJogador ajudam a personalizar missões que mencionam o jogador.
*/
void atribuirMissao(char* destino, char* missoes[], int totalMissoes,
                    Territorio* mapa, int n, const char* corJogador, const char* nomeJogador) {
    // escolher template aleatoriamente
    int idx = rand() % totalMissoes;
    char buffer[MAX_MISSAO];
    memset(buffer, 0, sizeof(buffer));

    if (strcmp(missoes[idx], "POSSUIR %d TERRITORIOS DA COR %s") == 0) {
        // escolher alvo entre 1 e n (pequeno)
        int alvo = (n >= 3) ? (rand() % 3 + 1) : 1;
        snprintf(buffer, MAX_MISSAO, missoes[idx], alvo, corJogador);
    } else if (strcmp(missoes[idx], "CONQUISTAR %d SEGUIDOS DA COR %s") == 0) {
        int alvo = (n >= 3) ? (rand() % 3 + 2) : 2; // 2 ou 3
        snprintf(buffer, MAX_MISSAO, missoes[idx], alvo, corJogador);
    } else if (strcmp(missoes[idx], "TER AO MENOS %d TROPAS DA COR %s") == 0) {
        int alvo = (n * 2 > 0) ? (rand() % (n*2) + 1) : 1;
        snprintf(buffer, MAX_MISSAO, missoes[idx], alvo, corJogador);
    } else if (strcmp(missoes[idx], "ELIMINAR TODAS AS TROPAS DA COR %s") == 0) {
        // escolher uma cor alvo diferente da corJogador, se possível
        // procura primeira cor diferente no mapa
        char corAlvo[MAX_COR] = "VERMELHO"; // fallback
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (strcmp(mapa[i].cor, corJogador) != 0) {
                strcpy(corAlvo, mapa[i].cor);
                found = 1;
                break;
            }
        }
        // se não encontrou (todos mesmos), escolhe corJogador mesmo (tarefa impossível, mas ok)
        snprintf(buffer, MAX_MISSAO, missoes[idx], corAlvo);
    } else if (strcmp(missoes[idx], "CONTROLAR O TERRITORIO %s") == 0) {
        // escolher um território aleatório do mapa
        int escolha = rand() % n;
        snprintf(buffer, MAX_MISSAO, missoes[idx], mapa[escolha].nome);
    } else {
        // fallback: missão genérica
        snprintf(buffer, MAX_MISSAO, "POSSUIR 1 TERRITORIO DA COR %s", corJogador);
    }

    // copia para destino (presume buffer grande o suficiente)
    strcpy(destino, buffer);
}

/* Verifica se a missão foi cumprida.
   Formatos esperados (produzidos por atribuirMissao):
   - "POSSUIR %d TERRITORIOS DA COR <cor>"
   - "CONQUISTAR %d SEGUIDOS DA COR <cor>"
   - "TER AO MENOS %d TROPAS DA COR <cor>"
   - "ELIMINAR TODAS AS TROPAS DA COR <cor>"
   - "CONTROLAR O TERRITORIO <nome>"
   Retorna 1 se cumprida, 0 caso contrário.
*/
int verificarMissao(const char* missao, Territorio* mapa, int n) {
    if (missao == NULL) return 0;

    // checar prefixos
    if (strncmp(missao, "POSSUIR ", 8) == 0) {
        int alvo;
        char tmp[64];
        // ex: "POSSUIR 3 TERRITORIOS DA COR Verde"
        if (sscanf(missao, "POSSUIR %d TERRITORIOS DA COR %63[^\n]", &alvo, tmp) == 2) {
            int cnt = contarTerritoriosCor(mapa, n, tmp);
            return (cnt >= alvo) ? 1 : 0;
        }
    } else if (strncmp(missao, "CONQUISTAR ", 10) == 0) {
        int alvo;
        char tmp[64];
        if (sscanf(missao, "CONQUISTAR %d SEGUIDOS DA COR %63[^\n]", &alvo, tmp) == 2) {
            return contarTerritoriosSeguidos(mapa, n, tmp, alvo);
        }
    } else if (strncmp(missao, "TER AO MENOS ", 13) == 0) {
        int alvo;
        char tmp[64];
        if (sscanf(missao, "TER AO MENOS %d TROPAS DA COR %63[^\n]", &alvo, tmp) == 2) {
            int total = totalTropasCor(mapa, n, tmp);
            return (total >= alvo) ? 1 : 0;
        }
    } else if (strncmp(missao, "ELIMINAR TODAS AS TROPAS DA COR ", 32) == 0) {
        char tmp[64];
        if (sscanf(missao, "ELIMINAR TODAS AS TROPAS DA COR %63[^\n]", tmp) == 1) {
            int total = totalTropasCor(mapa, n, tmp);
            return (total == 0) ? 1 : 0;
        }
    } else if (strncmp(missao, "CONTROLAR O TERRITORIO ", 23) == 0) {
        char tmp[64];
        if (sscanf(missao, "CONTROLAR O TERRITORIO %63[^\n]", tmp) == 1) {
            // procurar o territorio com esse nome e verificar se ele foi conquistado (cor pode ser qualquer jogador)
            for (int i = 0; i < n; i++) {
                if (strcmp(mapa[i].nome, tmp) == 0) {
                    // missão cumprida se algum jogador controla (pode ser jogador que não é dono original)
                    // aqui consideramos cumprida se o território existe e tem tropas > 0
                    return (mapa[i].tropas > 0) ? 1 : 0;
                }
            }
        }
    }

    return 0; // se não casou com nenhum formato, retorna 0
}

// ------------------ MAIN: fluxo do jogo simplificado ------------------ 

int main() {
    srand((unsigned) time(NULL)); // semente para rand()

    int numTerritorios;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &numTerritorios);
    if (numTerritorios <= 0) {
        printf("Numero de territorios invalido.\n");
        return 1;
    }

    // aloca dinamicamente o mapa
    Territorio* mapa = (Territorio*) calloc(numTerritorios, sizeof(Territorio));
    if (!mapa) {
        printf("Erro na alocacao de memoria.\n");
        return 1;
    }

    // cadastrar territórios
    cadastrarTerritorios(mapa, numTerritorios);

    // cadastrar jogadores (vamos permitir 2 a MAX_JOGADORES)
    int numJogadores;
    printf("\nQuantos jogadores? (2 a %d): ", MAX_JOGADORES);
    scanf("%d", &numJogadores);
    if (numJogadores < 2 || numJogadores > MAX_JOGADORES) numJogadores = 2;

    Jogador jogadores[MAX_JOGADORES];
    for (int i = 0; i < numJogadores; i++) {
        printf("\nDados do Jogador %d\n", i + 1);
        printf("Nome do jogador: ");
        scanf(" %[^\n]", jogadores[i].nome);
        printf("Cor do jogador (use a mesma grafia usada nos territorios): ");
        scanf(" %[^\n]", jogadores[i].cor);
        jogadores[i].missao = NULL; // serão alocadas abaixo
    }

    // criar templates de missões
    int totalTemplates;
    char **templates = criarTemplatesMissao(&totalTemplates);

    // atribuir missões a cada jogador (aloca buffer para missão e copia)
    for (int i = 0; i < numJogadores; i++) {
        jogadores[i].missao = (char*) malloc(MAX_MISSAO);
        if (!jogadores[i].missao) {
            printf("Erro alocando missao.\n");
            // liberar já alocado e sair
            liberarMemoriaJogadores(jogadores, i);
            liberarMemoriaTerritorios(mapa);
            free(templates);
            return 1;
        }
        atribuirMissao(jogadores[i].missao, templates, totalTemplates, mapa, numTerritorios, jogadores[i].cor, jogadores[i].nome);
        // mostrar missão ao jogador apenas uma vez (interface)
        printf("\n%s, sua missão é:\n>>> %s\n", jogadores[i].nome, jogadores[i].missao);
    }

    // exibir mapa inicial
    exibirTerritorios(mapa, numTerritorios);

    // gameplay simplificado: permite vários turnos de ataque até alguém cumprir sua missão
    int turno = 0;
    int vencedor = -1;
    while (1) {
        turno++;
        printf("\n========== TURNO %d ==========\n", turno);
        // mostrar jogadores
        for (int i = 0; i < numJogadores; i++) {
            printf("Jogador %d: %s (cor: %s)\n", i+1, jogadores[i].nome, jogadores[i].cor);
        }

        // pedir atacante (territorio index)
        int idxAtacante;
        printf("\nEscolha o numero do territorio atacante (1 a %d), ou 0 para encerrar: ", numTerritorios);
        scanf("%d", &idxAtacante);
        if (idxAtacante == 0) break;
        if (idxAtacante < 1 || idxAtacante > numTerritorios) {
            printf("Escolha invalida, tente de novo.\n");
            continue;
        }

        int idxDefensor;
        printf("Escolha o numero do territorio defensor (1 a %d): ", numTerritorios);
        scanf("%d", &idxDefensor);
        if (idxDefensor < 1 || idxDefensor > numTerritorios) {
            printf("Escolha invalida, tente de novo.\n");
            continue;
        }

        // valida: atacante nao pode atacar territorio da mesma cor
        if (strcmp(mapa[idxAtacante-1].cor, mapa[idxDefensor-1].cor) == 0) {
            printf("Ataque invalido: nao e permitido atacar um territorio da mesma cor.\n");
            continue;
        }

        // antes de atacar, ver quem é o dono (qual jogador tem a cor do atacante)
        // procurar jogador cuja cor bate com atacante->cor
        int jogadorAtacanteId = -1;
        for (int j = 0; j < numJogadores; j++) {
            if (strcmp(jogadores[j].cor, mapa[idxAtacante-1].cor) == 0) {
                jogadorAtacanteId = j;
                break;
            }
        }
        if (jogadorAtacanteId == -1) {
            // território pertence a "ninguém" (ou cor não registrada) — ainda permitimos o ataque como neutro
            printf("Territorio atacante nao esta associado a jogador cadastrado. Atacando como neutro...\n");
        } else {
            printf("Atacante: Jogador %s\n", jogadores[jogadorAtacanteId].nome);
        }

        // executar ataque
        atacar(&mapa[idxAtacante-1], &mapa[idxDefensor-1]);

        // exibir mapa atualizado
        exibirTerritorios(mapa, numTerritorios);

        // verificar missões para todos os jogadores silenciosamente
        for (int j = 0; j < numJogadores; j++) {
            if (verificarMissao(jogadores[j].missao, mapa, numTerritorios)) {
                vencedor = j;
                break;
            }
        }
        if (vencedor != -1) {
            printf("\n=====================\n");
            printf("Jogador %s cumpriu sua missão!\n", jogadores[vencedor].nome);
            printf("Missao: %s\n", jogadores[vencedor].missao);
            printf("=====================\n");
            break;
        }
    }

    if (vencedor == -1) {
        printf("\nNinguem cumpriu a missao. Jogo encerrado sem vencedor.\n");
    }

    // liberar memória
    liberarMemoriaJogadores(jogadores, numJogadores);
    liberarMemoriaTerritorios(mapa);
    free(templates); // templates foram alocados com malloc

    printf("\nFim do programa!\n");
    return 0;
}


