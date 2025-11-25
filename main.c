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
#define MAX_INPUT 50

char texto[MAX_INPUT + 1] = "";
int cursor = 0;

// Cartas
typedef enum {
    COMUM,
    INCOMUM,
    RARA,
    EPICA,
    LENDARIA,
    ESPECIAL
} Raridade;

const char *nome_raridade[] = {"Comum", "Incomum", "Rara", "Épica", "Lendária", "Especial"};
const int valor_raridade[] = {50, 75, 100, 150, 200};
int Ox = height/4;

typedef struct {
    char nome[50];
    Raridade raridade;
    char imagem[50];
    int qntd;
    int fav;
} Carta;

char *nome_carta;
char *imagem_carta;
Raridade raridade_carta;
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
        printf("Não foi possível abrir o arquivo de cartas.\n");
        exit(1);
    }
    char nome[50];
    int raridade;
    char imagem[50];
    while (fscanf(f," %49[^,],%d,%49[^\r\n]", nome, &raridade, imagem) == 3) {
        strcpy(catalogo[qtd_catalogo].nome, nome);
        catalogo[qtd_catalogo].raridade = raridade;
        strcpy(catalogo[qtd_catalogo].imagem, imagem);
        catalogo[qtd_catalogo].qntd = 0;
        catalogo[qtd_catalogo].fav = 0;
        qtd_catalogo++;
    }
    fclose(f);
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

void girarCarta (){
   if (giros <= 0) {
        giros = giros;
    }else{
        giros--;
        int random = rand() % qtd_catalogo;
        encontrarCarta(random);

        nome_carta = catalogo[random].nome;
        imagem_carta = catalogo[random].imagem;
        raridade_carta = catalogo[random].raridade;
    }
    
    
}

int main(void)
{
    srand (time(NULL));
    carregarCartas("cartas.txt");

    const int screenHeight = height;
    const int screenWidth = Width;
    int counter = 0; 
    Tela tela = INICIO;
    InitWindow(screenHeight, screenWidth, "Jogo legal");
    Font PixelSans = LoadFont("imagens/fontes/static/PixelifySans-Bold.ttf");
    

    Image kitty = LoadImage("imagens/Hello Kitty.png");
    ImageResize(&kitty, 300, 376);
    Texture Tkitty = LoadTextureFromImage(kitty);
    Texture2D background = LoadTexture("imagens/BGH.png");
    UnloadImage(kitty);

  
    

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {

        Vector2 mousePos = GetMousePosition();

        
       
        BeginDrawing();
        
            ClearBackground(WHITE);
            DrawTexture(background, 0, 0, WHITE);
            if(tela == INICIO)
            {
                 if(colisao(mousePos,height*0.27, Width*0.70,508, 166))
                    {
                        if(tela == INICIO)
                        {
                              tela = MENU; 
                        }
                   
                    }
            
                Desenha("imagens/Jogar.png",height*0.27,Width*0.70,508,166);   
            
                DrawTexture(Tkitty,Width*0.70, height*0.01,WHITE);

            }else if (tela == MENU)
            {
                int tW = 300;
                int tH = 82;

                Rectangle rec = {height*0.1, Width*0.15, 900, 400};
                //Vector2 position = {height*0.3,Width*0.15};

                counter =  0;
                if(colisao(mousePos,height*0.20, Width*0.4,tW, tH)  && giros>0)
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
                
                DrawRectangleRec(rec, RED);
                DrawRectangleLinesEx(rec, 10, BLACK);
                Desenha("imagens/Menu Principal.png",height*0.35,Width*0.2, 300, 50);
                DrawTextOutline(PixelSans,TextFormat("Giros restantes: %d", giros) , (Vector2){height*0.15,Width*0.30},40, 1, BLACK, WHITE);
                DrawTextOutline(PixelSans,TextFormat("|| Dinheiro restantes: %d", dinheiro) , (Vector2){height*0.45,Width*0.30},40, 1, BLACK, WHITE);
                Desenha("imagens/Girar Carta-menu.png",height*0.20,Width*0.4,tW, tH);
                Desenha("imagens/Vizualizar Colecao.png",height*0.20,Width*0.6,tW, tH);
                Desenha("imagens/Missoes.png",height*0.50,Width*0.4,tW, tH);
                Desenha("imagens/Sair.png",height*0.50,Width*0.6,tW, tH); 
        
            }else if(tela == GIROS)
            {
                int tW = 184;
                int tH = 91;
                Rectangle rec = {height*0.1, Width*0.05, 1000, 550};
             if(colisao(mousePos,height*0.60, Width*0.5,tW, tH))
                {
                        tela = MENU;
                   
                }
              if(counter == 0)
              {
                 girarCarta();
                 counter=1;
              }
                DrawRectangleRec(rec, RED);
                DrawRectangleLinesEx(rec, 10, BLACK);
                DrawTextOutline(PixelSans, "VOCE GIROU:", (Vector2){height*0.60,Width*0.2},50, 1, BLACK,WHITE);
                DrawTextOutline(PixelSans, nome_carta, (Vector2){height*0.50,Width*0.25},100, 1, BLACK, WHITE);

              if(imagem_carta)
              {
                Desenha("imagens/Voltar.png",height*0.60,Width*0.5,tW, tH);
                Desenha(imagem_carta,height*0.15,Width*0.15,337, 420);

                if(raridade_carta == LENDARIA)
                {
                    DrawTextOutline(PixelSans, "< LENDARIA!! >", (Vector2){height*0.54,Width*0.4},55, 1, BLACK, VIOLET );
                }else if(raridade_carta == EPICA)
                {
                    DrawTextOutline(PixelSans, "< EPICA!! >", (Vector2){height*0.57,Width*0.4},55, 1,BLACK, DARKBLUE);
                }else if (raridade_carta == RARA)
                {
                    DrawTextOutline(PixelSans, "< RARA!! >", (Vector2){height*0.59,Width*0.4},55, 1,BLACK, GREEN);
                }else if(raridade_carta == INCOMUM)
                {
                    DrawTextOutline(PixelSans, "< INCOMUM!! >", (Vector2){height*0.55,Width*0.4},55, 1,BLACK, YELLOW);
                }else if(raridade_carta == COMUM)
                {
                    DrawTextOutline(PixelSans, "< COMUM!! >", (Vector2){height*0.57,Width*0.4},55, 1,BLACK, ORANGE);
                }
                
              }
            
              
            }else if(tela == INVENTARIO)
            {
                int tW = 184;
                int tH = 91;
                
                Rectangle rec = {height*0.1, Width*0.05, 1000, 550};
                
                DrawRectangleRec(rec, RED);
                DrawRectangleLinesEx(rec, 10, BLACK);
                if(colisao(mousePos,height*0.60, Width*0.7,tW, tH))
                {    
                        tela = MENU;
                   
                }
                if(colisao(mousePos,height*0.30,Width*0.75,249, 69) && tela == INVENTARIO)
                {    
                    tela = VENDAS;
                }
                if(colisao(mousePos,150, Width/2, 30, 30) || IsKeyPressed(KEY_LEFT))
                    {
                        Ox = Ox-50;
                    };
                if(colisao(mousePos,1050, Width/2, 30, 30)|| IsKeyPressed(KEY_RIGHT))
                    {
                        Ox = Ox+50;
                    };
                 // DrawTextEx(PixelSans, TextFormat("Carta: %s", col->qntd), (Vector2){60, 20}, 20, 1, RED);
                for(int i = 0 ; i<qtd_col; i++)
                {
                    int x = Ox + (i*250);
                    int y = Width*0.2;
                    Rectangle nRec = {150, Width/2, 30, 30};
                    DrawRectangleRec(nRec, BLACK);
                    Rectangle dRec = {1050, Width/2, 30, 30};
                    DrawRectangleRec(dRec, BLACK);
                    if(x<20+200)continue;
                    if(x>1000-200)continue;
                    DrawTextEx(PixelSans, TextFormat("Carta: %s", col[i].nome), (Vector2){x, y}, 20, 1, RED);
                    Desenha(col[i].imagem, x, y, 200, 241);
                    DrawTextEx(PixelSans, TextFormat("%d", col[i].qntd), (Vector2){x, y+25}, 20, 1, RED);
                }
                Desenha("imagens/Voltar.png",height*0.60,Width*0.70,tW, tH);
                Desenha("imagens/Vender Carta.png",height*0.30,Width*0.75,249, 69);

            }else if(tela==VENDAS)
            {
                    int key = GetCharPressed();
                    int tW = 184;
                    int tH = 91;
                

                    if(colisao(mousePos,height*0.60, Width*0.7,tW, tH))
                    {    
                        tela = INVENTARIO;
                   
                    }
                    if (key >= 32 && key <= 125)    
                    {
                        if (cursor < MAX_INPUT)
                        {
                            texto[cursor] = (char)key;
                            cursor++;
                            texto[cursor] = '\0';
                        }
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && cursor > 0)
                    {
                            cursor--;
                            texto[cursor] = '\0';
                    }
                    if(IsKeyPressed(KEY_ENTER))
                    {
                       for(int i = 0; i<qtd_col; i++)
                       {
                            if(strcmp(texto,col[i].nome)==0){

                                 int valor = valor_raridade[col[i].raridade];
                                 dinheiro += valor;
                                 col[i].qntd -= 1;
                                 

                                 if(col[i].qntd == 0){
                                    for(int j = i; j<qtd_col - 1; j++){
                                        col[j] = col[j+1];
                                    }
                                    qtd_col--;
                                    tela = INVENTARIO;
                                 }
                            }
                       }
                    }
                    
                    Rectangle Rvenda = {height*0.1, Width*0.05, 1000, 550};
                    DrawRectangleRec(Rvenda, DARKBLUE);
                    DrawRectangleLinesEx(Rvenda, 10, BLACK);
                    DrawTextOutline(PixelSans, "Qual carta voce\n deseja vender?", (Vector2){height*0.29,Width*0.1},70, 1,BLACK, WHITE);
                    DrawRectangle(350, 300, 500, 60, GRAY);
                    DrawText(texto, 350, 300, 50, BLACK);
                    Desenha("imagens/Voltar.png",height*0.60,Width*0.70,tW, tH);


            }
            
            
           
        
        EndDrawing();
    }
}