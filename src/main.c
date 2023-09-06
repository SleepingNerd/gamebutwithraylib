#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"
#include "samath.h"
#include "world.h"
#include "sanimation.h"
#include "player.h"

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
    //SetTargetFPS(60);

    Vector2i tile_size = {.x = 16, .y = 16};
    World world;
    world.size.x = (screenWidth/16);
    world.size.y = (screenHeight/16);
    world.arr = calloc(world.size.x*world.size.y, sizeof(Tile));

    Animation test_anim = LoadAnimation("assets/player/player.png", 5, 1.0f);
    AnimationManager anim_m_test = {.frame = 0, .delta = 0};
    LoadTileTextures();

    Player player = {.speed=40, .rect = {.height = 16, .width = 16, .x = 0, .y=0}, .state=0, .velocity={0}, .jump_force=0, .gravity=0, .t_size = {0}};
    CalculateJump(&player, 40, 1);
    CalculateTileSize(&player);

    while(!WindowShouldClose())
    {
        UpdateAnimationManager(&anim_m_test, &test_anim, GetFrameTime());

        ProcessInput(&player, IsKeyDown(KEY_LEFT), IsKeyDown(KEY_RIGHT), IsKeyDown(KEY_UP));
        MoveAndUpdate(&player, GetFrameTime(), world);


        Vector2i selected_tile = {.x= floor_to_muiltiple(GetMousePosition().x/winWidth*screenWidth, TILE_SIZE), .y=floor_to_muiltiple(GetMousePosition().y/winHeight*screenHeight, TILE_SIZE)};
        Vector2i selected_tile_index =  {.x= selected_tile.x/TILE_SIZE, .y=selected_tile.y /TILE_SIZE};

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {

            ChangeTile(world.arr, world.size, selected_tile_index);
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
            DrawFPS(10, 10);

            DrawRectangle(selected_tile.x, selected_tile.y, TILE_SIZE, TILE_SIZE, MOUSE_OVERLAY);
            RenderWorld(world.arr, world.size);
            DrawTexture(test_anim.frame_arr[anim_m_test.frame], player.rect.x, player.rect.y, WHITE);
            DrawRectangleRec(player.rect, BLACK);




        EndTextureMode();

            DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=screenWidth, .height=-screenHeight}, (Rectangle){.x=0, .y=0, .width=winWidth, .height=winHeight}, (Vector2){.x = 0, .y = 0}, 0.0f, WHITE);


        BeginDrawing();
            
        EndDrawing();



    }
    free(world.arr);
    CloseWindow();
    return 0;
}