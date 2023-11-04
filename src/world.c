#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "samath.h"
#include "world.h"

char* texture_names[TILES] = {"sand"};
Texture2D textures[TILES] = {0};

void LoadTileTextures()
{
    char tiles_folder[] = "assets/tiles/";
    int tile_folder_length = strlen(tiles_folder);
    char file_extension[] = ".png";
    int file_ext_length = strlen(file_extension);

    int temp_size = tile_folder_length+file_ext_length+1;

    for (int i = 0; i < TILES; i++)
    {
        int name_length = strlen(texture_names[i]);
        char *concatted = malloc(name_length+temp_size);

        strcpy(concatted, tiles_folder);
        strcat(concatted, texture_names[i]);
        strcat(concatted,  file_extension);

        // I'm just going to assume all textures are TILE_SIZE cause I really don't care for now
        textures[i] = LoadTexture(concatted);
        free(concatted);
    }
}

void ChangeTile(Tile *world, Vector2i world_size, Vector2i position)
{

    for(int y= -5; y++; y<=5)
    {
        for (int x = -5; x++; x<=5)
        {

            world[(position.y+y)*(world_size.x) +position.x+x].type = SAND;
            DrawRectangle((position.x+x)*TILE_SIZE,(position.y+y)*TILE_SIZE, TILE_SIZE, TILE_SIZE, BEIGE);
        }
    }

}

void RenderWorld(Tile *world, Vector2i world_size)
{
    for (int x = 0; x<world_size.x; x++)
    {
        for (int y = 0; y<world_size.y; y ++)
        {
            if (world[y*world_size.x +x].type == SAND)
            {
                DrawRectangle(x*TILE_SIZE,y*TILE_SIZE, TILE_SIZE, TILE_SIZE, BEIGE);
                //DrawTexture(textures[world[y*world_size.x +x].type-1],x*TILE_SIZE,y*TILE_SIZE, WHITE);
            }

        }
    }
}

// Offset is the inworld offset, size is the size of the chunk we need
// It is drawn on static_world, with render_coords as top left
void RenderPartOfWorld(World world,  Vector2i offset, Vector2i size,  RenderTexture2D static_world, Vector2i render_coords)
{
    int i = 0;
    int j = 0;
    for (int x = offset.x; x<=(offset.x+size.x); x++)
    {
        for (int y = offset.y; x<=(offset.y+size.y); y++)
        {
            if (world.arr[y*world.size.x +x].type == SAND)
            {
                DrawRectangle(i*TILE_SIZE+render_coords.x, j*TILE_SIZE+render_coords.y, TILE_SIZE, TILE_SIZE, BEIGE);
            }
            j++;

        }
        i++;
    }
}


void ScrollWorld(World world, Vector2i scroll, RenderTexture2D static_world, Vector2i camera_offset)
{

    BeginTextureMode(static_world);
    
 

    // Going left
    if (scroll.x < 0)
    {

        
        // Copy left part of the screen (x=0, width = static_world.texture.width -abs(scroll.x)) to scroll.x
        DrawTextureRec(static_world.texture, (Rectangle){.x=0, .y=0, .width = static_world.texture.width -abs(scroll.x), .height= -static_world.texture.height}, (Vector2){.x=abs(scroll.x), .y=0}, WHITE);

        RenderPartOfWorld(world, camera_offset, (Vector2i){.x = scroll.x, .y = world.size.y}, static_world, (Vector2i){.x = 0, .y=0});
    

    }
 
    else if (scroll.x > 0)
    {

        DrawTextureRec(static_world.texture, (Rectangle){.x= abs(scroll.x), .y=0, .width = static_world.texture.width -abs(scroll.x), .height= -static_world.texture.height}, (Vector2){.x=0, .y=0}, WHITE);

    }
  

    EndTextureMode();
    
    

}

