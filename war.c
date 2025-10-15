#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função para cadastrar os territórios
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

// Função para exibir os territórios
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

// Função que simula um ataque entre territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1; // 1 a 6
    int dadoDefensor = rand() % 6 + 1;

    printf("\nAtaque de %s para %s\n", atacante->nome, defensor->nome);
    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Ataque bem-sucedido!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
    } else {
        printf("Ataque falhou!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// Função para liberar a memória alocada
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

int main() {
    srand(time(NULL)); // inicializa aleatoriedade

    int numTerritorios;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &numTerritorios);

    // Alocação dinâmica do vetor de territórios
    Territorio* mapa = (Territorio*) calloc(numTerritorios, sizeof(Territorio));

    // Cadastro e exibição inicial
    cadastrarTerritorios(mapa, numTerritorios);
    exibirTerritorios(mapa, numTerritorios);

    // Simulação de ataque
    int atacante, defensor;
    printf("\nEscolha o numero do territorio atacante (1 a %d): ", numTerritorios);
    scanf("%d", &atacante);
    printf("Escolha o numero do territorio defensor (1 a %d): ", numTerritorios);
    scanf("%d", &defensor);

    // Ajuste de índices para ponteiros (0 a n-1)
    atacar(&mapa[atacante - 1], &mapa[defensor - 1]);

    // Exibir resultado pós-ataque
    exibirTerritorios(mapa, numTerritorios);

    // Liberar memória
    liberarMemoria(mapa);

    printf("\nFim do programa!\n");

    return 0;
}

