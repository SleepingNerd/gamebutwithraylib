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
    world.size.x = (screenWidth/TILE_SIZE);
    world.size.y = (screenHeight/TILE_SIZE);
    world.arr = calloc(world.size.x*world.size.y, sizeof(Tile));

    Animation test_anim = LoadAnimation("assets/player/player.png", 5, 0.3f);
    AnimationManager anim_m_test = {.frame = 0, .delta = 0};
    LoadTileTextures();

    Player player = {.speed=20, .rect = {.height = 16, .width = 8, .x = 40, .y=200}, .state=0, .velocity={0}, .jump_force=0, .gravity=0, .p_offset={0}, .slide_up=2};
    player.facing = RIGHT;
    CalculateJump(&player, 100, 1);
    CalculateSize(&player, 8, 16);
    


    Rectangle player_img_rect = {.height=16, .width=8,.x=0,.y=0};


    float delta_t;


    while(!WindowShouldClose())
    {
        delta_t = GetFrameTime(); 

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            delta_t /=2;
        }
        UpdateAnimationManager(&anim_m_test, &test_anim, GetFrameTime());

        ProcessInput(&player, IsKeyDown(KEY_LEFT), IsKeyDown(KEY_RIGHT), IsKeyDown(KEY_UP));
        MoveAndUpdate(&player, GetFrameTime(), world);


        Vector2i selected_tile = {.x= floor_to_muiltiple(GetMousePosition().x/winWidth*screenWidth, TILE_SIZE), .y=floor_to_muiltiple(GetMousePosition().y/winHeight*screenHeight, TILE_SIZE)};
        Vector2i selected_tile_index =  {.x= selected_tile.x/TILE_SIZE, .y=selected_tile.y /TILE_SIZE};

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {

            ChangeTile(world.arr, world.size, selected_tile_index);
        }
        


        if (IsKeyPressed(KEY_SPACE))
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
            //printf("%f\n", player.rect.width);
            //DrawRectangle(player.rect.x, player.rect.y, player.rect.width, player.rect.height , DARKGREEN);
            
            player_img_rect.width= fabs(player_img_rect.width)*player.facing;


            Vector2 offset = {.x = (int)player.rect.x, .y= (int)player.rect.y};
            DrawTextureRec(test_anim.frame_arr[anim_m_test.frame],player_img_rect, offset, WHITE);

            DrawRectangle(selected_tile.x, selected_tile.y, TILE_SIZE, TILE_SIZE, MOUSE_OVERLAY);
            RenderWorld(world.arr, world.size);


        EndTextureMode();

            DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=screenWidth, .height=-screenHeight}, (Rectangle){.x=0, .y=0, .width=winWidth, .height=winHeight}, (Vector2){.x = 0, .y = 0}, 0.0f, WHITE);


        BeginDrawing();
            
        EndDrawing();



    }
    free(world.arr);
    CloseWindow();
    return 0;
}