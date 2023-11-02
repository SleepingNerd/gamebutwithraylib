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
    RenderTexture2D static_world = LoadRenderTexture(screenWidth, screenHeight);


    Vector2 camera_offset = {.x = 0, .y =0};
    //Vector2 camera_middle = {.x = 0, .y = 0};

 
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

    Player player = {.speed=50, .rect = {.height = 16, .width = 8, .x = 248, .y=136}, .state=0, .velocity={0}, .jump_force=-100, .gravity=130, .p_offset={0}, .slide_up=3, .grounded = false};
    player.facing = RIGHT;

    CalculateSize(&player, 8, 15);
    


    Rectangle player_img_rect = {.height=16, .width=8,.x=0,.y=0};


    float delta_t;

    BeginTextureMode(static_world);
    ClearBackground(WHITE);
    RenderWorld(world.arr, world.size);
    EndTextureMode;


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

        // Player should always be in the middle of the screen, therefore
        // For these calculations to be accurate the player size should be divisible by 2
        // Player_pos = 
        //

        float camera_target_x = (player.rect.x - screenWidth/2-player.rect.width/2);
        float camera_target_y = (player.rect.y - screenHeight/2-player.rect.height/2);

        // Normalize vector from player
        Vector2 normalized = Vector2Normalize((Vector2){.x = camera_target_x-camera_offset.x, .y = camera_target_y-camera_offset.y});


        //printf("/ %f, %f\n", normalized.x, normalized.y);
        //camera_offset.x += normalized.x*20*delta_t;
        //camera_offset.y += normalized.y*20*delta_t;
        camera_offset.x = camera_target_x;
        camera_offset.y = camera_target_y;



        printf("%f, %f\n", camera_offset.x, camera_target_x);




        Vector2i selected_tile = {.x= floor_to_muiltiple(GetMousePosition().x/winWidth*screenWidth, TILE_SIZE), .y=floor_to_muiltiple(GetMousePosition().y/winHeight*screenHeight, TILE_SIZE)};
        Vector2i selected_tile_index =  {.x= selected_tile.x/TILE_SIZE, .y=selected_tile.y /TILE_SIZE};

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            BeginTextureMode(static_world);
            ChangeTile(world.arr, world.size, selected_tile_index);
            EndTextureMode();
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
            DrawTextureRec(static_world.texture, (Rectangle){.x = 0, .y = 0, .width=screenWidth, .height=-screenHeight}, (Vector2){0, 0}, WHITE);

            DrawFPS(10, 10);
            //printf("%f\n", player.rect.width);
            //DrawRectangle(player.rect.x, player.rect.y, player.rect.width, player.rect.height , DARKGREEN);
            
            player_img_rect.width= fabs(player_img_rect.width)*player.facing;


            Vector2 offset = {.x = (int)(player.rect.x+camera_offset.x), .y= (int)(player.rect.y-camera_offset.y)};
            DrawTextureRec(test_anim.frame_arr[anim_m_test.frame],player_img_rect, offset, BLACK);

            
            


        EndTextureMode();

            DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=screenWidth, .height=-screenHeight}, (Rectangle){.x=0, .y=0, .width=winWidth, .height=winHeight}, (Vector2){.x = 0, .y = 0}, 0.0f, WHITE);


        BeginDrawing();
            
        EndDrawing();



    }
    free(world.arr);
    CloseWindow();
    return 0;
}