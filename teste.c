#include "raylib.h"

int main() {
    InitWindow(800, 450, "Raylib Test");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("VAI CORINTHIANS!!!    ", 200, 200, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
