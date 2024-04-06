#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "samath.h"
#include "world.h"

//
char* texture_names[TILES] = {"SOLID"};
Texture2D textures[TILES] = {0};

extern Texture chunk_texture;

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
    Map m = {.chunk_size = chunk_size, .chunks = calloc(chunk_count.x*chunk_count.y, sizeof(Chunk*)), .chunk_count=chunk_count, .beany_chunks = calloc(20, sizeof(Chunk*))};

 
    if (m.beany_chunks == NULL)
    {
            printf("\nWhy me????\n");
    }
    if (m.chunks == NULL)
    {
        printf("Can't even allocate 16 MB on this, you suck (probably more now, you still suck though)");
    }


    return m;
}

Chunk *GenerateEmptyChunk(Vector2i chunk_size)
{
    Chunk *c = malloc(sizeof(Chunk));
    c->colors = calloc(chunk_size.y, chunk_size.x*sizeof(Color));
    c->tiles = calloc(chunk_size.y, chunk_size.x*sizeof(TileState));
    c->moved = calloc(chunk_size.y, chunk_size.x*sizeof(char));
    c->image.data = (void*)c->colors;
    c->image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    c->image.width = chunk_size.x;
    c->image.height = chunk_size.y;
    c->image.mipmaps = 1;
    

    return c;
}

void AddActiveChunk(Map w, Chunk* chunk_p)
{


    int i;
    for(i = 0; i<20; i++)
    {
        if (w.beany_chunks[i]==chunk_p)
        {
            return;
        }
    }

    for(i = 0; i<20; i++)
    {
        if (w.beany_chunks[i]==NULL)
        {
            w.beany_chunks[i] = chunk_p;
            return;
        }
    }
}


void ChangeTile(Map world, Vector2i position, TileState tile_state, Color color)
{
    Vector2i tile_in_chunk = {.x=position.x%world.chunk_size.x, .y = position.y%world.chunk_size.y};
    Vector2i chunk = {.x = position.x / world.chunk_size.x, .y = position.y/world.chunk_size.y};

    

    for(int y = -1; y<=1; y++)
    {
        for (int x = -1; x<=1; x++)
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


            world.chunks[chunk.x+chunk.y*world.chunk_count.x]->tiles[tile_in_chunk.x+tile_in_chunk.y*world.chunk_size.x] = tile_state;
            world.chunks[chunk.x+chunk.y*world.chunk_count.x]->colors[tile_in_chunk.x+tile_in_chunk.y*world.chunk_size.x] = color;
          

            if (tile_state != SOLID)
            {
                AddActiveChunk(world, world.chunks[chunk.x+chunk.y*world.chunk_count.x]);
            }
         
        }
    }
}

TileState GetTileState(Map world, int x, int y)
{
    Vector2i tile_in_chunk = {.x=x%world.chunk_size.x, .y = y%world.chunk_size.y};
    Vector2i chunk = {.x = x / world.chunk_size.x, .y = y/world.chunk_size.y};

    return world.chunks[chunk.x+chunk.y*world.chunk_count.x]->tiles[tile_in_chunk.x+tile_in_chunk.y*world.chunk_size.x];

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
            }
        }
    }
}
bool try_to_move(Chunk* chunk, int origin_i, int new_i)
{
    if (chunk->tiles[new_i]<2)
    {
                         
        chunk->tiles[new_i] = FLUID;
        chunk->colors[new_i] =chunk->colors[origin_i];
        chunk->moved[new_i] = 1;

        chunk->tiles[origin_i] = VOID;
        chunk->colors[origin_i] = BLANK;
        chunk->moved[origin_i] = 0;
        return true;
    }
}

void SimulateWorld(Map world)
{

    for (int i = 0; i<20; i++)
    {

        if (world.beany_chunks[i] != NULL)
        {
            memset(world.beany_chunks[i]->moved, 0, world.chunk_size.y*world.chunk_size.x);
            for (int x = 0; x<world.chunk_size.x; x++)
            {
                for (int y = world.chunk_size.y; y>=0; y--)
                {
                    int index = y*world.chunk_size.x+x;
                    if (world.beany_chunks[i]->moved[index])
                    {
                        continue;
                    }
                    
                    if (world.beany_chunks[i]->tiles[y*world.chunk_size.x+x]==FLUID)
                    {

                        if ((world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x]<2))
                        {
                            //printf("%i\n", SOLID);
                            //printf("%i the under\n", world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x]);
                            world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x] = FLUID;
                            world.beany_chunks[i]->colors[(y+1)*world.chunk_size.x+x] = world.beany_chunks[i]->colors[y*world.chunk_size.x+x];
                            world.beany_chunks[i]->moved[(y+1)*world.chunk_size.x+x] = 1;

                            world.beany_chunks[i]->tiles[y*world.chunk_size.x+x] = VOID;
                            world.beany_chunks[i]->colors[y*world.chunk_size.x+x] = BLANK;

                        }
                        else if (world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x+1]<2)
                        {
                            //printf("%i\n", SOLID);
                            //printf("%i the under\n", world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x]);
                            world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x+1] = FLUID;
                            world.beany_chunks[i]->colors[(y+1)*world.chunk_size.x+x+1] = world.beany_chunks[i]->colors[y*world.chunk_size.x+x];
                            world.beany_chunks[i]->moved[(y+1)*world.chunk_size.x+x+1] = 1;

                            world.beany_chunks[i]->tiles[y*world.chunk_size.x+x] = VOID;
                            world.beany_chunks[i]->colors[y*world.chunk_size.x+x] = BLANK;

                        }
                        else if (world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x-1]<2)
                        {
                            //printf("%i\n", SOLID);
                            //printf("%i the under\n", world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x]);
                            world.beany_chunks[i]->tiles[(y+1)*world.chunk_size.x+x-1] = FLUID;
                            world.beany_chunks[i]->colors[(y+1)*world.chunk_size.x+x-1] = world.beany_chunks[i]->colors[y*world.chunk_size.x+x];
                            world.beany_chunks[i]->moved[(y+1)*world.chunk_size.x+x-1] = 1;

                            world.beany_chunks[i]->tiles[y*world.chunk_size.x+x] = VOID;
                            world.beany_chunks[i]->colors[y*world.chunk_size.x+x] = BLANK;
                        }

                    
                        else if (world.beany_chunks[i]->tiles[y*world.chunk_size.x+x+1]<2)
                        {
                                            
                            world.beany_chunks[i]->tiles[y*world.chunk_size.x+x+1] = FLUID;
                            world.beany_chunks[i]->colors[y*world.chunk_size.x+x+1] = world.beany_chunks[i]->colors[y*world.chunk_size.x+x];
                            world.beany_chunks[i]->moved[y*world.chunk_size.x+x+1] = 1;

                            world.beany_chunks[i]->tiles[y*world.chunk_size.x+x] = VOID;
                            world.beany_chunks[i]->colors[y*world.chunk_size.x+x] = BLANK;
                            world.beany_chunks[i]->moved[y*world.chunk_size.x+x] = 0;
                        }

                        else if (world.beany_chunks[i]->tiles[y*world.chunk_size.x+x-1]<2)
                        {
                                            
                            world.beany_chunks[i]->tiles[y*world.chunk_size.x+x-1] = FLUID;
                            world.beany_chunks[i]->colors[y*world.chunk_size.x+x-1] = world.beany_chunks[i]->colors[y*world.chunk_size.x+x];
                            world.beany_chunks[i]->moved[y*world.chunk_size.x+x-1] = 1;

                            world.beany_chunks[i]->tiles[y*world.chunk_size.x+x] = VOID;
                            world.beany_chunks[i]->colors[y*world.chunk_size.x+x] = BLANK;
                            world.beany_chunks[i]->moved[y*world.chunk_size.x+x] = 0;
                        } 

                        

                        
                    
                        /*
                        else if (try_to_move(world.beany_chunks[i], y*world.chunk_size.x+x, y*world.chunk_size.x+x+1))
                        {
                                try_to_move(world.beany_chunks[i], y*world.chunk_size.x+x, y*world.chunk_size.x+x-1);                        
                        }*/
                        

                    }

                }
            }

        }

    }
}
   


// Size should be larger than 0 (in both axes), anything else is undefined behaviour
void DrawWorld(Map world, Vector2i offset, Vector2i size, RenderTexture2D target)
{
    int offset_from_left_chunk_border = offset.x % world.chunk_size.x;
    int left_chunk = offset.x / world.chunk_size.x;


    int width_from_left_chunk_border =  offset_from_left_chunk_border + size.x;
    int chunk_width = width_from_left_chunk_border/world.chunk_size.x;

    int offset_from_top_chunk_border = offset.y % world.chunk_size.y;
    int top_chunk = offset.y / world.chunk_size.y;


    int height_from_top_chunk_border = offset_from_top_chunk_border + size.y;
    int chunk_height = height_from_top_chunk_border/world.chunk_size.y;

    int row_offset;

    Vect2i origin_in_chunk;
    Vect2i end_in_chunk;

    Vect2i offset_c = (Vect2i){.x=0, .y=0}; // Screen offset

    // I could implement this all in different for loops for a slight performance gain but I don't wanna debug that all
    for(int y = 0; y<=chunk_height; y++)
    {
        origin_in_chunk.y = 0;
        end_in_chunk.y = world.chunk_size.y; 


        if (y==0)
        {
            origin_in_chunk.y = offset_from_top_chunk_border;
        }
        if (y==chunk_height)
        {
            end_in_chunk.y = (offset.y+size.y)%world.chunk_size.y;
        }
      

        row_offset = (top_chunk+y)*world.chunk_count.x;
        offset_c.x = 0;
        for(int x = 0; x<=chunk_width; x++)
        {
            origin_in_chunk.x = 0;
            end_in_chunk.x = world.chunk_size.x; 



            if (x==0)
            {
                origin_in_chunk.x = offset_from_left_chunk_border;


            }
            if (x==chunk_width)
            {
                end_in_chunk.x = (offset.x+size.x)%world.chunk_size.x;
            }
            
            if (world.chunks[row_offset+left_chunk+x] == NULL)
            { 
                world.chunks[row_offset+left_chunk+x]= GenerateEmptyChunk(world.chunk_size);
            }

            
            DrawPartOfChunk(world.chunks[row_offset+left_chunk+x], 
            origin_in_chunk, end_in_chunk, 
            offset_c, world.chunk_size, target);

            offset_c.x += end_in_chunk.x - origin_in_chunk.x;
        }
        offset_c.y += end_in_chunk.y - origin_in_chunk.y;

    }

}
// Off
void DrawPartOfChunk(Chunk *c, Vector2i origin_in_chunk, Vector2i end_in_chunk, Vector2i offset, Vector2i chunk_size, RenderTexture2D target)
{

    



    // 35-50 fps
    //Image sliced = ImageFromImage(c->image, (Rectangle){.x=origin_in_chunk.x, .y = origin_in_chunk.y, .width = end_in_chunk.x-origin_in_chunk.x, .height= end_in_chunk.y-origin_in_chunk.y});                                                  // Create an image from another image piece
    //Texture sliced_texture = LoadTextureFromImage(sliced);
   // DrawTexture(sliced_texture, offset.x+origin_in_chunk.x*chunk_size.x, offset.y+origin_in_chunk.y*chunk_size.y, WHITE);

   
    UpdateTexture(chunk_texture, c->image.data);  
    DrawTextureRec(chunk_texture, (Rectangle){.x=origin_in_chunk.x, .y = origin_in_chunk.y, .width = end_in_chunk.x-origin_in_chunk.x, .height= end_in_chunk.y-origin_in_chunk.y},  (Vector2){.x=offset.x, .y=offset.y}, WHITE);
    rlDrawRenderBatchActive();

    //DrawTexture(chunk_texture, offset.x+origin_in_chunk.x*chunk_size.x, offset.y+origin_in_chunk.y*chunk_size.y, WHITE);

    //UpdateTextureRec(target.texture, (Rectangle){.x = offset.x+origin_in_chunk.x*chunk_size.x, .y = offset.y+origin_in_chunk.y*chunk_size.y, .width = sliced.width, .height = sliced.height}, sliced.data);
    /*int row_offset;
    for (int y = 0; y<end_in_chunk.y; y++)
    {
        row_offset = y*chunk_size.x;
        for (int x = 0; x<end_in_chunk.x; x++)
        {
            DrawPixel(x+offset.x, y+offset.y, c->colors[row_offset+x]);

        }
    }*/
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
        printf("Fucking hate tis schoollaptop sometimes");
    }


    for (unsigned int i = 0; i<total_elements; i++)
    {
        world->tiles[i] = VOID;
        world->colors[i] = WHITE;
    }
}


