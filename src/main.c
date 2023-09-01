#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"
#include "samath.h"
#include "world.h"
#include "sanimation.h"

#define MOUSE_OVERLAY (Color){ 211, 176, 131, 100 }   




int main()
{
    int winWidth = 512;
    int winHeight = 288;

    InitWindow(winWidth, winHeight, "Best game I've ever seen");
    

    

    int screenWidth = 512;
    int screenHeight = 288;

    SetWindowMinSize(screenWidth, screenHeight);
    RenderTexture2D screen = LoadRenderTexture(screenWidth, screenHeight);

    Camera2D camera = {0};
    camera.zoom = 1.0f;
    Texture2D sprite = LoadTexture("assets/sand.png");
    SetTargetFPS(60);

    Vector2i world_size = {.x = (screenWidth/16), .y = (screenHeight/16)};
    Vector2i tile_size = {.x = 16, .y = 16};

    Tile *world = calloc(world_size.x*world_size.y, sizeof(Tile));

    Animation test_anim = LoadAnimation("assets/player/player.png", 5, 1.0f);
    AnimationManager anim_m_test = {.frame = 0, .delta = 0};
    LoadTileTextures();

    while(!WindowShouldClose())
    {
        anim_m_test.delta += GetFrameTime();
        if (anim_m_test.delta >= test_anim.time_between_frames)
        {
            anim_m_test.delta = 0;
            anim_m_test.frame += 1;
            if (anim_m_test.frame == test_anim.frames)
            {
                anim_m_test.frame = 0;
            }
        }


        printf("%i\n", anim_m_test.frame);
        Vector2i selected_tile = {.x= floor_to_muiltiple(GetMousePosition().x/winWidth*screenWidth, TILE_SIZE), .y=floor_to_muiltiple(GetMousePosition().y/winHeight*screenHeight, TILE_SIZE)};
        Vector2i selected_tile_index =  {.x= selected_tile.x/TILE_SIZE, .y=selected_tile.y /TILE_SIZE};

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            printf("%i, %i\n", selected_tile_index.x, selected_tile_index.y);

            ChangeTile(world, world_size, selected_tile_index);
        }

        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
            if (IsWindowFullscreen())
            {
                winHeight = GetMonitorHeight(GetCurrentMonitor());
                winWidth = GetMonitorWidth(GetCurrentMonitor());
            }
            else
            {
                winWidth = screenWidth;
                winHeight = screenHeight;
            }

        }


        BeginTextureMode(screen);
            ClearBackground(WHITE);
            DrawRectangle(selected_tile.x, selected_tile.y, TILE_SIZE, TILE_SIZE, MOUSE_OVERLAY);
            RenderWorld(world, world_size);
            DrawTexture(LoadTextureFromImage(test_anim.frame_arr[anim_m_test.frame]), 100, 100, WHITE);




        EndTextureMode();

            DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=screenWidth, .height=-screenHeight}, (Rectangle){.x=0, .y=0, .width=winWidth, .height=winHeight}, (Vector2){.x = 0, .y = 0}, 0.0f, WHITE);


        BeginDrawing();
            
        EndDrawing();



    }
    free(world);
    CloseWindow();
    return 0;
}