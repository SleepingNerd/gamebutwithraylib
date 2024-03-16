#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"
#include "samath.h"
#include "sanimation.h"
#include "world.h"
#include "player.h"
#include "camera.h"

#define MOUSE_OVERLAY (Color){ 211, 176, 131, 100 }   




int main()
{

    int winWidth = 640;
    int winHeight = 360;

    
    int screenWidth = 640;
    int screenHeight = 360;
   

    InitWindow(winWidth, winHeight, "Best game I've ever seen");
    SetWindowMinSize(screenWidth, screenHeight);

    RenderTexture2D screen = LoadRenderTexture(screenWidth, screenHeight);
    RenderTexture2D static_world = LoadRenderTexture(screenWidth, screenHeight);



    //Vector2 camera_middle = {.x = 0, .y = 0};

 
    Texture2D test = LoadTexture("assets/test.png");
    //SetTargetFPS(60);

    Vector2i tile_size = {.x = 16, .y = 16};
    World world;

    EmptyWorld(&world, (Vector2i){.x = 100, .y = 100});



    void *img_data = NULL;
    Image static_world_img = {.data = img_data, .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, .height = screenHeight, .width = screenWidth};


    View camera = {0};

    Animation test_anim = LoadAnimation("assets/player/player.png", 5, 0.3f);
    AnimationManager anim_m_test = {.frame = 0, .delta = 0};

    LoadTileTextures();

    Player player = {.speed=50, .rect = {.height = 16, .width = 8, .x = (winWidth/2)-4, .y=(winHeight/2)-8}, .state=0, .velocity={0}, .jump_force=-100, .gravity=130, .p_offset={0}, .slide_up=2, .grounded = false};
    player.facing = RIGHT;
    printf("??Ifjk");

    CalculateSize(&player, 8, 15);
    
    printf("??Ifjk");

    Color *image = calloc(4, sizeof(Color));
    image[0] = RED;
    image[1] = BLUE;
    image[2] = GREEN;
    image[3] = GRAY;

    Image test_img = {.data = image, .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, .height = 2, .width = 2, .mipmaps = 1};
    Texture test_text = LoadTextureFromImage(test_img);


    Rectangle player_img_rect = {.height=16, .width=8,.x=0,.y=0};


    float delta_t;

    BeginTextureMode(static_world);
    ClearBackground(WHITE);
    //RenderWorld(world, world.)
    EndTextureMode();
    Image img_slice;

    Map world2 = GenerateEmptyWorld((Vector2i){.x = 512, .y = 512}, (Vector2i){.x = 4096, .y =4096});

   
   

    while(!WindowShouldClose())
    {


        delta_t = GetFrameTime(); 


        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            delta_t /=2;
        }
        UpdateAnimationManager(&anim_m_test, &test_anim, GetFrameTime());

        ProcessInput(&player, IsKeyDown(KEY_LEFT), IsKeyDown(KEY_RIGHT), IsKeyDown(KEY_UP));

        Vector2 prev_player_pos = {.x = player.rect.x, .y =player.rect.y};

        //MoveAndUpdate(&player, GetFrameTime(), world);

        Vector2 scroll ={.x = player.rect.x-prev_player_pos.x, .y =player.rect.y-prev_player_pos.y};

        Scroll(&camera, scroll.x, scroll.y);


        Vector2i selected_tile = {.x= floor_to_muiltiple((GetMousePosition().x)/winWidth*screenWidth+camera.offset.x, TILE_SIZE), .y=floor_to_muiltiple((GetMousePosition().y)/winHeight*screenHeight+camera.offset.y, TILE_SIZE) };
       
       
        Vector2i selected_tile_index =  {.x= selected_tile.x/TILE_SIZE, .y=selected_tile.y /TILE_SIZE};

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            BeginTextureMode(static_world);
            ChangeTile(world2, selected_tile_index);
            EndTextureMode();
        }
        


        if (IsKeyPressed(KEY_SPACE))
        {
            ToggleFullscreen();
            if (IsWindowFullscreen())
            {
                winHeight = GetMonitorHeight(GetCurrentMonitor());
                winWidth = GetMonitorWidth(GetCurrentMonitor());
                printf("%i, %i", winHeight, winWidth);
            }
            else
            {
                winWidth = screenWidth;
                winHeight = screenHeight;
            }
        }

        // Rendering stuff starts from here

        //img_slice = ImageFromImage(static_world_img, (Rectangle){.x =(int)camera.offset.x, .y = (int)camera.offset.y, .width=screenWidth, .height= screenHeight});
        //ImageFlipVertical(&img_slice);


        //UpdateTexture(screen.texture, img_slice.data);   
        //UnloadImage(img_slice);
        

        BeginTextureMode(screen);
            ClearBackground(WHITE);
            // I store textures with (0,0) as top-left, 
            // This is why I have to flip the y




            DrawFPS(10, 10);
            //printf("%f\n", player.rect.width);
            //DrawRectangle(player.rect.x, player.rect.y, player.rect.width, player.rect.height , DARKGREEN);
            


            player_img_rect.width= fabs(player_img_rect.width)*player.facing;
            Vector2 offset = {.x = (int)(player.rect.x)-(int)camera.offset.x, .y= (int)(player.rect.y)-(int)camera.offset.y};
            


        EndTextureMode();

        BeginDrawing();

            DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=screenWidth, .height=-screenHeight}, (Rectangle){.x=0, .y=0, .width=winWidth, .height=winHeight}, (Vector2){.x = 0, .y = 0}, 0.0f, WHITE);

        EndDrawing();
    }

    SaveWorld(world, "worlds/world.saworld");

  

    CloseWindow();
    return 0;
}