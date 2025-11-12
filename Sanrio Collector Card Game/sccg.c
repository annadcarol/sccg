#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CARTAS 100
#define MAX_COL 200

// Cartas
typedef enum {
    COMUM,
    INCOMUM,
    RARA,
    EPICA,
    LENDARIA
} Raridade;

const char *nome_raridade[] = {"Comum", "Incomum", "Rara", "Épica", "Lendária"};
const int valor_raridade[] = {50, 75, 100, 150, 200};

typedef struct {
    char nome[50];
    Raridade raridade;
    int qntd;
    int fav;
} Carta;

// Cartas disponíveis no jogo
Carta catalogo[MAX_CARTAS];
int qtd_catalogo = 0;

// Coleção do jogador
Carta col[MAX_COL];
int qtd_col = 0;

// Recursos iniciais do jogador
int dinheiro = 0;
int giros = 10;

// Funções
// Carregar cartas com arquivo externo.
void carregarCartas (const char *arquivo){
    FILE *f = fopen(arquivo, "r");
    if (!f) {
        printf("Não foi possível abrir o arquivo de cartas.\n");
        exit(1);
    }
    char nome[50];
    int raridade;
    while (fscanf(f, " %49[^,],%d", nome, &raridade) == 2) {
        strcpy(catalogo[qtd_catalogo].nome, nome);
        catalogo[qtd_catalogo].raridade = raridade;
        catalogo[qtd_catalogo].qntd = 0;
        catalogo[qtd_catalogo].fav = 0;
        qtd_catalogo++;
    }
    fclose(f);

}

// Comprar giros
void comprarGiro(){
    while (dinheiro >= 100) {
        printf("Deseja comprar giros?\n");
        printf("1. Sim\n");
        printf("2. Não.\n");
        int escolha;
        scanf("%d", &escolha);
        switch (escolha){
            case 1:
            dinheiro -= 100;
            giros++;
            printf("Agora você tem %d giros.\n", giros);
            break;
            case 2:
            return;
            default:
            printf("Opção inválida.");
        }
    }
}

// Função auxiliar para verificar se a carta já existe na coleção.
void encontrarCarta (int idx){
    int encontrar = 0;
    for (int i = 0; i < qtd_col; i++) {
        if (strcmp(col[i].nome, catalogo[idx].nome) == 0) {
            col[i].qntd++;
            encontrar = 1;
            break;
        }
    }
    if (!encontrar) {
        col[qtd_col] = catalogo[idx];
        col[qtd_col].qntd = 1;
        qtd_col++;
    }
}

//Girar uma carta aleatória.
void girarCarta (){
    if (giros <= 0) {
        if (dinheiro >= 100){
            comprarGiro();
        } else {
          printf("\nVocê não tem giros disponíveis.\n");
          return;  
        }
    }
    giros--;
    int random = rand() % qtd_catalogo;
    encontrarCarta(random);
    printf("\nVocê girou:\n");
    printf("%s\n", catalogo[random].nome);
    printf("[%s]\n", nome_raridade[catalogo[random].raridade]);
    printf("%s foi adicionado a sua coleção.\n", catalogo[random].nome);
}

// Favoritar carta.
void favoritar (){
    printf("\nDigite o número da carta para favoritar.");
    int numCarta;
    scanf("%d", &numCarta);
    if (numCarta > 0 && numCarta <= qtd_col) {
        int idx = numCarta - 1;
        col[idx].fav = !col[idx].fav;
        printf("\nA carta '%s' foi %s!\n",
            col[idx].nome,
            col[idx].fav ? "favoritada" : "desfavoritada"
        );
    }
}

// Vender carta.
void vender (){
    printf("\nDigite o número da carta para vender.");
    int numCarta;
    scanf("%d", &numCarta);
    if (numCarta > 0 && numCarta <= qtd_col) {
        int idx = numCarta - 1;
        if (col[idx].qntd > 0) {
            dinheiro += valor_raridade[col[idx].raridade];
            col[idx].qntd--;
            printf("Você vendeu 1x %s por $%d.\n", col[idx].nome, valor_raridade[col[idx].raridade]);
            if (col[idx].qntd == 0) {
                for (int j = idx; j < qtd_col - 1; j++) {
                    col[j] = col[j + 1];
                }
                qtd_col--;
            }
        }
    }
}

// Visualizar coleção, favoritar e vender cartas.
void visualizarCol(){
    if (qtd_col == 0){
        printf("\nA coleção está vazia.\n");
        return;
    }
    printf("\n=== Sua Coleção ===\n");
    for (int i = 0; i < qtd_col; i++) {
        printf ("%d. %s [%s] x%d %s\n",
            i + 1, 
            col[i].nome, 
            nome_raridade[col[i].raridade],
            col[i].qntd,
            col[i].fav ? "<3" : ""
        );
    }
    int escolha;
    printf("\n=== Opções ===\n");
    printf("1. Favoritar carta.\n");
    printf("2. Vender carta.\n");
    printf("3. Voltar.\n");
    scanf("%d", &escolha);
    switch (escolha) {
        case 1:
        favoritar();
        break;
        case 2:
        vender();
        break;
        case 3:
        return;
        break;
        default:
        printf("Opção inválida.");
    }
    
}

// Main
int main (){
    srand (time(NULL));
    carregarCartas("cartas.txt");
    int opcao;
    
    do {
        printf("\n=== MENU ===\n");
        printf("Dinheiro: $%d | Giros: %d\n", dinheiro, giros);
        printf("1. Girar carta ($100)\n");
        printf("2. Visualizar coleção\n");
        printf("0. Sair.\n");
        scanf("%d", &opcao);
        getchar();

        switch (opcao){
            case 1:
            girarCarta();
            break;
            case 2:
            visualizarCol();
            break;
            case 0:
            printf("Saindo...\n");
            break;
            default:
            printf("Opção inválida.\n");
        }
    } while (opcao != 0);
    return 0;
}