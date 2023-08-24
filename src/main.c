#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"
#include "samath.h"
#include "world.h"

#define MOUSE_OVERLAY (Color){ 211, 176, 131, 100 }   




int main()
{
    int screenWidth = 256;
    int screenHeight = 144;

    InitWindow(screenWidth, screenHeight, "Best game I've ever seen");


    Camera2D camera = {0};
    camera.zoom = 1.0f;

    Texture2D sprite = LoadTexture("assets/sand.png");
    SetTargetFPS(60);

    Vector2i world_size = {.x = 16, .y = 9};
    Vector2i tile_size = {.x = 16, .y = 16};

    Tile *world = calloc(world_size.x*world_size.y, sizeof(Tile));

    LoadTileTextures();

    while(!WindowShouldClose())
    {
        
        Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
        Vector2i selected_tile = {.x= floor_to_muiltiple(mouse_pos.x, TILE_SIZE), .y=floor_to_muiltiple(mouse_pos.y, TILE_SIZE)};
        Vector2i selected_tile_index =  {.x= selected_tile.x/TILE_SIZE, .y=selected_tile.y /TILE_SIZE};

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            printf("%i, %i\n", selected_tile_index.x, selected_tile_index.y);

            ChangeTile(world, world_size, selected_tile_index);
        }

        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        BeginDrawing();
            ClearBackground(WHITE);
            BeginMode2D(camera);
            DrawRectangle(selected_tile.x, selected_tile.y, TILE_SIZE, TILE_SIZE, MOUSE_OVERLAY);
            RenderWorld(world, world_size);


            EndMode2D();
        EndDrawing();



    }
    CloseWindow();
    return 0;
}