#include "raylib.h"
#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define Width 600
#define height 1200

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

char *nome_carta;
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


typedef enum{
    INICIO,
    MENU,
    INVENTARIO,
    MISSOES,
    VENDAS,
    GIROS
}Tela;


void carregarCartas(const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (!f) {
        DrawText("Erro: não foi possível abrir1 \n",20, 20, 70, RED);
        exit(1);
    }

    char linha[256];
    char nome[50];
    int raridade;

    while (fgets(linha, sizeof(linha), f)) {

        // Remove o '\n' se existir
        linha[strcspn(linha, "\n")] = 0;

        // Tenta ler no formato: NOME, NUMERO
        if (sscanf(linha, " %49[^,], %d", nome, &raridade) != 2) {
            DrawText("Erro: não foi possível abrir2 \n",20, 20, 70, RED);
            continue;
        }

        // Valida raridade
        if (raridade < 0 || raridade > 4) {
             DrawText("Erro: não foi possível abrir3 \n",20, 20, 70, RED);
            continue;
        }

        // Evita estourar o catálogo
        if (qtd_catalogo >= MAX_CARTAS) {
             DrawText("Erro: não foi possível abrir 4\n",20, 20, 70, RED);
            break;
        }

        // Copia os dados para o catálogo
        strcpy(catalogo[qtd_catalogo].nome, nome);
        catalogo[qtd_catalogo].raridade = raridade;
        catalogo[qtd_catalogo].qntd = 0;
        catalogo[qtd_catalogo].fav = 0;

        qtd_catalogo++;
    }

    fclose(f);
}
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

void girarCarta (){
   
    int random = rand() % qtd_catalogo;
    encontrarCarta(random);
    //printf("\nVocê girou:\n");

    nome_carta = catalogo[random].nome;
    
    //printf("[%s]\n", nome_raridade[catalogo[random].raridade]);
    //printf("%s foi adicionado a sua coleção.\n", catalogo[random].nome);
}

int main(void)
{
    srand (time(NULL));
    carregarCartas("cartas.txt");

    const int screenHeight = height;
    const int screenWidth = Width;
    int counter = 0;
    int size = 20;
    Color color = RED;
    Tela tela = INICIO;
    InitWindow(screenHeight, screenWidth, "Jogo legal");
    Font PixelSans = LoadFont("imagens/fontes/static/PixelifySans-Bold.ttf");
    

    Image kitty = LoadImage("imagens/Hello Kitty.png");
    ImageResize(&kitty, 300, 376);
    Texture Tkitty = LoadTextureFromImage(kitty);
    Texture2D background = LoadTexture("imagens/BGH.png");\

  
    

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {

        Vector2 mousePos = GetMousePosition();

        
       
        BeginDrawing();
        
            ClearBackground(WHITE);
            DrawTexture(background, 0, 0, WHITE);
            Vector2 position = {20,20};
            if(tela == INICIO)
            {
                 if(colisao(mousePos,height*0.27, Width*0.70,508, 166))
                    {
                        if(tela == INICIO)
                        {
                              tela = MENU; 
                        }
                   
                    }
                DrawTextEx(PixelSans, "CARLOS LINDO", position, 50, 1 , BLACK);
            
                Desenha("imagens/jogar.png",height*0.27,Width*0.70,508,166);   
            
                DrawTexture(Tkitty,Width*0.70, height*0.01,WHITE);

            }else if (tela == MENU)
            {
                int tW = 300;
                int tH = 82;

                Rectangle rec = {height*0.1, Width*0.15, 900, 400};
                //Vector2 position = {height*0.3,Width*0.15};

                counter =  0;
                if(colisao(mousePos,height*0.20, Width*0.4,tW, tH))
                    {
                        tela = GIROS;
                   
                    }
                if(colisao(mousePos,height*0.20, Width*0.6,tW, tH))
                    {
                        tela = INVENTARIO;
                   
                    }
                if(colisao(mousePos,height*0.5, Width*0.4,tW, tH))
                    {
                        tela = MISSOES;
                   
                    }
                if(colisao(mousePos,height*0.5, Width*0.6,tW, tH))
                    {
                        tela = INICIO;
                   
                    }
                DrawText("TELA DE MENU",20,20,size,color);
                
                DrawRectangleRec(rec, RED);
                DrawRectangleLinesEx(rec, 10, BLACK);
                Desenha("imagens/Menu Principal.png",height*0.35,Width*0.2, 300, 50);
                //DrawTextEx(PixelSans, "MENU PRINCIPAL", position, 70, 1 , WHITE);
                //Desenha("imagens/Frame menu.png",20,20,tW, tH);
                Desenha("imagens/Girar Carta-menu.png",height*0.20,Width*0.4,tW, tH);
                Desenha("imagens/Vizualizar Colecao.png",height*0.20,Width*0.6,tW, tH);
                Desenha("imagens/Missoes.png",height*0.50,Width*0.4,tW, tH);
                Desenha("imagens/Sair.png",height*0.50,Width*0.6,tW, tH); 
        
            }else if(tela == GIROS)
            {
                int tW = 300;
                int tH = 82;
             if(colisao(mousePos,height*0.5, Width*0.6,tW, tH))
                {
                        tela = MENU;
                   
                }
              if(counter == 0)
              {
                 girarCarta();
                 counter=1;
              }
              DrawText(nome_carta, 30, 30, 70, RED);
              //DrawText("SE VOCE ESTA LENDO ISSO QUER DIZER QUE O CODIGO FUNCIONA", 40, 40, 90, RED);
              Desenha("imagens/Sair.png",height*0.50,Width*0.6,tW, tH); 
            }
            
            
           
        
        EndDrawing();
    }
}