#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "samath.h"
#include "world.h"
//
char* texture_names[TILES] = {"SOLID"};
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

void ChangeTile(World world, Vector2i world_size, Vector2i position)
{
    if (position.x > world_size.x)
    {
        return;
    }

    for(int y= -5; y++; y<=5)
    {
        for (int x = -5; x++; x<=5)
        {

            world.tiles[(position.y+y)*(world_size.x) +position.x+x] = SOLID;
            world.colors[(position.y+y)*(world_size.x) +position.x+x] = BEIGE;

     
        }
    }

}

void RenderWorld(World world, Vector2i world_size)
{
    for (int x = 0; x<world_size.x; x++)
    {
        for (int y = 0; y<world_size.y; y ++)
        {
            if (world.tiles[y*world_size.x +x] == SOLID)
            {
                DrawPixel(x, y, BEIGE);
                //DrawTexture(textures[world[y*world_size.x +x]-1],x*TILE_SIZE,y*TILE_SIZE, WHITE);
            }
        }
    }
}

void SaveWorld(World world, char * file_name)
{
    printf("frick");
}

// Offset is the inworld offset, size is the size of the chunk we need
// It is drawn on static_world, with render_coords as top left
void RenderPartOfWorld(World world,  Vector2i offset, Vector2i size,  RenderTexture2D static_world, Vector2i render_coords)
{

    printf("%i, %i\n", offset.x, offset.y);
    printf("%i, %i\n", size.x, size.y);
    printf("%i, %i\n", offset.x, offset.y);
    printf("...\n");

    int i = render_coords.x;
    int j = render_coords.y;
    
    for (int x = offset.x; x<=(offset.x+size.x); x++)
    {
    
        for (int y = offset.y; x<=(offset.y+size.y); y++)
        {
            if (world.tiles[y*world.size.x +x] == SOLID)
            {
                DrawRectangle(i*TILE_SIZE, j*TILE_SIZE, TILE_SIZE, TILE_SIZE, BEIGE);
            }
            j++;

        }
        i++;
    }
}

void EmptyWorld(World *world)
{
    unsigned int total_elements = world->size.x*world->size.y;

    world->tiles = calloc(total_elements, sizeof(TileState));
    world->colors = calloc(total_elements, sizeof(Color));


    for (unsigned int i = 0; i<total_elements; i++)
    {
        world->tiles[i] = VOID;
        world->colors[i] = WHITE;
    }
}


