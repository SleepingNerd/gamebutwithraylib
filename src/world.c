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

Map GenerateEmptyWorld(Vector2i chunk_size, Vector2i chunk_count)
{
    Map m = {.chunk_size = chunk_size, .chunks = calloc(chunk_count.x*chunk_count.y, sizeof(Chunk*)), .chunk_count=chunk_count};
    if (m.chunks == NULL)
    {
        printf("Can't even allocate 16 MB on this, you suck");
    }
    return m;
}
Chunk *GenerateEmptyChunk(Vector2i chunk_size)
{
    Chunk *c = malloc(sizeof(Chunk));
    c->colors = calloc(chunk_size.y, chunk_size.x*sizeof(Color));
    c->tiles = calloc(chunk_size.y, chunk_size.x*sizeof(TileState));
    return c;
}



void ChangeTile(Map world, Vector2i position)
{
    Vector2i tile_in_chunk = {.x=position.x%world.chunk_size.x, .y = position.y%world.chunk_size.y};
    Vector2i chunk = {.x = position.x / world.chunk_size.x, .y = position.y/world.chunk_size.y};

    

    for(int y= -5; y++; y<=5)
    {
        for (int x = -5; x++; x<=5)
        {
            tile_in_chunk.x = (position.x+x)%world.chunk_size.x; 

            tile_in_chunk.y = (position.y+y)%world.chunk_size.y;
            chunk.x = (position.x +x)/ world.chunk_size.x;

            chunk.y = (position.y+y)/world.chunk_size.y;

         


            if (world.chunks[chunk.x+chunk.y*world.chunk_count.x] == NULL)
            {
                world.chunks[chunk.x+chunk.y*world.chunk_count.x] = GenerateEmptyChunk(world.chunk_size);
            }

            // tile_in_chunk.x+x>world.chunk_size.x || (tile_in_chunk.y + y)<world.chunk_size.y
            if (true)
            {
                    world.chunks[chunk.x+chunk.y*world.chunk_count.x]->tiles[tile_in_chunk.x+tile_in_chunk.y*world.chunk_size.x] = SOLID;
                    world.chunks[chunk.x+chunk.y*world.chunk_count.x]->colors[tile_in_chunk.x+tile_in_chunk.y*world.chunk_size.x] = ORANGE;
            }
            /*
            else
            {
                //...
                world.chunks[chunk.y*world.chunk_size.x+chunk.x].tiles[tile_in_chunk.y*world.chunk_size.x+tile_in_chunk.x] = SOLID;
                world.chunks[chunk.y*world.chunk_size.x+chunk.x].colors[tile_in_chunk.y*world.chunk_size.x+tile_in_chunk.x] = ORANGE;
            }*/
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

void SaveWorld(World world, char *path)
{
    FILE *fptr = fopen(path, "w");
    fwrite(&world.size, sizeof(typeof(world.size)), 1, fptr);
    fclose(fptr);
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

void EmptyWorld(World *world, Vector2i size)
{
    world->size = size;
    unsigned int total_elements = world->size.x*world->size.y;

    world->tiles = calloc(total_elements, sizeof(TileState));
    world->colors = calloc(total_elements, sizeof(Color));
    if (world->tiles == NULL || world->colors == NULL)
    {
        printf("Fucking hate tis schoollaptop");
    }


    for (unsigned int i = 0; i<total_elements; i++)
    {
        world->tiles[i] = VOID;
        world->colors[i] = WHITE;
    }
}


