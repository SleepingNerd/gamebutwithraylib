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
    world[position.y*world_size.x +position.x].type = SAND;
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
                DrawTexture(textures[world[y*world_size.x +x].type-1],x*TILE_SIZE,y*TILE_SIZE, WHITE);
            }

        }
    }
}