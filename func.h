#include "raylib.h"

int colisao(Vector2 mousePos, int x, int y, int w, int h)
{
    
    Rectangle rec = {x, y, w, h};
    DrawRectangle(x,y,w,h, RED);
    if(CheckCollisionPointRec(mousePos, rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return true;
    }

    return false;
}


void Desenha(const char *imagePath,int Width, int height, int tamW, int tamH)
{
    Image image = LoadImage(imagePath);
    ImageResize(&image, tamW, tamH);
    Texture texture = LoadTextureFromImage(image);
    DrawTexture(texture, Width, height, WHITE);
    UnloadImage(image);
    
  
}

void DrawTextOutline(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color outlineColor, Color fillColor)
{

    float offset = 2.0f; 

    DrawTextEx(font, text, (Vector2){position.x - offset, position.y}, fontSize, spacing, outlineColor); // Esquerda
    DrawTextEx(font, text, (Vector2){position.x + offset, position.y}, fontSize, spacing, outlineColor); // Direita
    DrawTextEx(font, text, (Vector2){position.x, position.y - offset}, fontSize, spacing, outlineColor); // Cima
    DrawTextEx(font, text, (Vector2){position.x, position.y + offset}, fontSize, spacing, outlineColor); // Baixo

   
    DrawTextEx(font, text, position, fontSize, spacing, fillColor);
}