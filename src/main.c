#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"



int main()
{
    int screenWidth = 640;
    int screenHeight = 360;
    InitWindow(screenWidth, screenHeight, "Best game I've ever seen");


    Camera2D camera = {0};
    camera.zoom = 1.0f;

    Texture2D sprite = LoadTexture("assets/person.png");
    SetTargetFPS(60);

   
    while(!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }
        

        BeginDrawing();
            ClearBackground(WHITE);
            BeginMode2D(camera);

            DrawRectangle(100, 100, 100, 100, BEIGE);



            EndMode2D();
        EndDrawing();

        

    }
    CloseWindow();
    return 0;
}