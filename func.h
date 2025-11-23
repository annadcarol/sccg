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
    
    
    
  
}
void DrawOutlinedText(const char *text, int posX, int posY, int fontSize, Color color, int outlineSize, Color outlineColor) {
    DrawText(text, posX - outlineSize, posY - outlineSize, fontSize, outlineColor);
    DrawText(text, posX + outlineSize, posY - outlineSize, fontSize, outlineColor);
    DrawText(text, posX - outlineSize, posY + outlineSize, fontSize, outlineColor);
    DrawText(text, posX + outlineSize, posY + outlineSize, fontSize, outlineColor);
    
}