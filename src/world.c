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

void SwapACBuffers(Map w)
{
    memcpy(w.active_chunks2, w.active_chunks, w.active_chunks_len*sizeof(Chunk*));
}

Map GenerateEmptyWorld(Vector2i chunk_size, Vector2i chunk_count, Vector2i subchunk_size) // REMEMBER: some more nullchecks should be added
{

    if (chunk_size.x%subchunk_size.x!=0||chunk_size.y%subchunk_size.y!=0)
    {
        printf("Not good: chunk_size isn't divisible by subchunk_size so yeah.");
    }
    Vector2i subchunk_count = (Vector2i){.x=chunk_size.x/subchunk_size.x, .y=chunk_size.y/subchunk_size.y}; // ngl only reason I  even declared this was cause I didn't want to allocate memory out of the fancy struct declaration

   

    Map m = {.chunk_size = chunk_size, .chunks = calloc(chunk_count.x*chunk_count.y, sizeof(Chunk*)), .chunk_count=chunk_count, 
    .active_chunks = calloc(DEFAULT_ACTIVE_LEN, sizeof(FullWorldIndex)), .subchunk_size = subchunk_size, 
    .active_chunks2 = calloc(DEFAULT_ACTIVE_LEN, sizeof(FullWorldIndex)),
    .subchunk_count = subchunk_count,
    // We reallocate the proper amount later
    .inner_subchunks_length = 0, .outer_subchunks_length = 0, 
    .inner_subchunks = calloc(subchunk_count.x*subchunk_count.y, sizeof(int)), .outer_subchunks = calloc(subchunk_count.x*subchunk_count.y, sizeof(int)), 
    .inner_subchunks_topleft = calloc(subchunk_count.x*subchunk_count.y, sizeof(Vector2i)), .outer_subchunks_topleft = calloc(subchunk_count.x*subchunk_count.y, sizeof(Vector2i)),
    .active_chunks_len = DEFAULT_ACTIVE_LEN
    };
    
   // printf("%i, %i", subchunk_count.x, subchunk_count.y);
    //printf("%i, %i\n", subchunk_size.x, subchunk_size.y);
    if (subchunk_count.x==0||subchunk_count.y==0)
    {

    }

    for (int x = 0; x<subchunk_count.x; x++)
    {
        for (int y = 0; y<subchunk_count.y; y++)
        {
            if (x == 0 || x == (subchunk_count.x-1) || y == 0 || y >= (subchunk_count.y-1))
            {
          
                m.outer_subchunks[m.outer_subchunks_length] = y*subchunk_count.x+x;
                m.outer_subchunks_topleft[m.outer_subchunks_length].x = x*subchunk_size.x;
                m.outer_subchunks_topleft[m.outer_subchunks_length].y = y*subchunk_size.y;
                m.outer_subchunks_length+=1;
            }
            else
            {

              
                m.inner_subchunks[m.inner_subchunks_length] = y*subchunk_count.x+x;
                m.inner_subchunks_topleft[m.inner_subchunks_length].x = x*subchunk_size.x;
                m.inner_subchunks_topleft[m.inner_subchunks_length].y = y*subchunk_size.y;
                if (m.inner_subchunks_length<100)
                {
                    printf("%i %i %i\n", m.inner_subchunks_length, m.inner_subchunks_topleft[m.inner_subchunks_length].x, m.inner_subchunks_topleft[m.inner_subchunks_length].y);
                }

           
                m.inner_subchunks_length+=1;

               
                //printf("%i, %i\n", x*subchunk_size.x, y*subchunk_size.y);

            }
        }
    }

    m.inner_subchunks = realloc(m.inner_subchunks, m.inner_subchunks_length*sizeof(int));
    printf("%f\n", m.inner_subchunks_length);
    m.inner_subchunks_topleft = realloc(m.inner_subchunks_topleft, m.inner_subchunks_length*sizeof(Vector2i));


    m.outer_subchunks = realloc(m.outer_subchunks, m.outer_subchunks_length*sizeof(int));
    m.outer_subchunks_topleft = realloc(m.outer_subchunks_topleft, m.outer_subchunks_length*sizeof(Vector2i));
    printf("...");
    if (m.active_chunks == NULL)
    {
            printf("\nfpp^lpl\n");
    }
    if (m.chunks == NULL)
    {
        printf("Can't even allocate 16 MB on this, you suck (probably more now though, you still suck though)");
    }
    printf("I am deeply sad.\n");

    for (int j = 0; j<m.inner_subchunks_length; j++)
    {
        printf("%i %i, %i\n", j, m.inner_subchunks_topleft[j].y, m.inner_subchunks_topleft[j].y);
        if (j == 100)
        {
            break;
        }
    }
    return m;
}

Chunk *GenerateEmptyChunk(Vector2i chunk_size, Vector2i subchunk_size)
{
    Chunk *c = malloc(sizeof(Chunk));
    c->colors = calloc(chunk_size.y, chunk_size.x*sizeof(Color));
    c->tiles = calloc(chunk_size.y, chunk_size.x*sizeof(TileState));
    c->moved = calloc(chunk_size.y, chunk_size.x*sizeof(char));
    c->subchunks = calloc(subchunk_size.y, subchunk_size.x);
    c->image.data = (void*)c->colors;
    c->image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    c->image.width = chunk_size.x;
    c->image.height = chunk_size.y;
    c->image.mipmaps = 1;
    

    return c;
}
/*
inline FullWorldIndex GeneralAccess(Map w, int i)
{
    printf("temp?");
}*/

FullWorldIndex GeneralAccessXY(Map world, int x, int y)
{
    Vector2i tile_in_chunk = {.x=x%world.chunk_size.x, .y=y%world.chunk_size.y};
    Vector2i chunkxy = {.x=x/world.chunk_size.x, .y=y/world.chunk_size.y};

    //printf("%i,%i\n", x,y);
    //printv2i(chunkxy);
    return (FullWorldIndex){.chunk = chunkxy.x+chunkxy.y*world.chunk_count.x, .in_chunk=tile_in_chunk.x+tile_in_chunk.y*world.chunk_size.x, .in_chunk_pos=tile_in_chunk, .chunk_pos =chunkxy};

}


void ActivateChunk(Map w, FullWorldIndex chunk_i)
{

    if (w.chunks[chunk_i.chunk]->is_active)
    {
        return;
    }

    int i;
    for(i = 0; i<20; i++)
    {
        if (w.active_chunks[i].in_chunk!=0 && w.active_chunks[i].chunk==chunk_i.chunk)
        {
            return;
        }
    }

    for(i = 0; i<20; i++)
    {
        if (w.active_chunks[i].in_chunk==0)
        {
            w.active_chunks[i] = chunk_i;
            return;
        }
    }
    w.chunks[chunk_i.chunk]->is_active = true;
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
                world.chunks[chunk.x+chunk.y*world.chunk_count.x] = GenerateEmptyChunk(world.chunk_size, world.subchunk_size);
            }


            // tile_in_chunk.x+x>world.chunk_size.x || (tile_in_chunk.y + y)<world.chunk_size.y


            world.chunks[chunk.x+chunk.y*world.chunk_count.x]->tiles[tile_in_chunk.x+tile_in_chunk.y*world.chunk_size.x] = tile_state;
            world.chunks[chunk.x+chunk.y*world.chunk_count.x]->colors[tile_in_chunk.x+tile_in_chunk.y*world.chunk_size.x] = color;
          

            if (tile_state != SOLID)
            {
                world.chunks[chunk.x+chunk.y*world.chunk_count.x]->subchunks[tile_in_chunk.x-(tile_in_chunk.x%world.subchunk_size.x) + (tile_in_chunk.y-(tile_in_chunk.y%world.subchunk_size.y))*world.subchunk_size.x] = 1;
                //AddActiveChunk(world, world.chunks[chunk.x+chunk.y*world.chunk_count.x]);
                ActivateChunk(world, (FullWorldIndex){.chunk=chunk.x+chunk.y*world.chunk_count.x,.chunk_pos.x = chunk.x, .chunk_pos.y=chunk.y, .in_chunk=1, .in_chunk_pos={0}});

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
        Chunk *chunk_pointer = world.chunks[world.active_chunks2[i].chunk];

        if (world.active_chunks2[i].in_chunk != 0)
        {

            memset(chunk_pointer->moved, 0, world.chunk_size.y*world.chunk_size.x);
            
            for (int j = 0; j<world.inner_subchunks_length; j++)
            {

              
                // Technically both 1 over the border
                int right = world.inner_subchunks_topleft[j].x+world.subchunk_size.x;
                int bottom = world.inner_subchunks_topleft[j].y+world.subchunk_size.y;

                //for(int y = bottom-1; y>=world.inner_subchunks_topleft[j].y; y--)

                
                for(int y = bottom-1; y>=world.inner_subchunks_topleft[j].y; y--)
                {
                    for(int x = right-1; x>=world.inner_subchunks_topleft[j].x; x--)
                    {
                        int index = y*world.chunk_size.x+x;
                        if (chunk_pointer->moved[index])
                        {
                            continue;
                        }
                  


                        if (chunk_pointer->tiles[index]==FLUID)
                        {

                            if ((chunk_pointer->tiles[(y+1)*world.chunk_size.x+x]<2))
                            {
                                //printf("I am deeply saad once agin");
                                //printf("%i\n", SOLID);
                                //printf("%i the under\n", chunk_pointer->tiles[(y+1)*world.chunk_size.x+x]);
                                
                                chunk_pointer->tiles[(y+1)*world.chunk_size.x+x] = FLUID;
                                chunk_pointer->colors[(y+1)*world.chunk_size.x+x] = chunk_pointer->colors[y*world.chunk_size.x+x];
                                chunk_pointer->moved[(y+1)*world.chunk_size.x+x] = 1;

                                chunk_pointer->tiles[y*world.chunk_size.x+x] = VOID;
                                chunk_pointer->colors[y*world.chunk_size.x+x] = BLANK;
                                chunk_pointer->moved[y*world.chunk_size.x+x] = 0;


                            }
                             else if (chunk_pointer->tiles[(y+1)*world.chunk_size.x+x+1]<2)
                            {

                                //printf("%i\n", SOLID);
                                //printf("%i the under\n", chunk_pointer->tiles[(y+1)*world.chunk_size.x+x]);
                                chunk_pointer->tiles[(y+1)*world.chunk_size.x+x+1] = FLUID;
                                chunk_pointer->colors[(y+1)*world.chunk_size.x+x+1] = chunk_pointer->colors[y*world.chunk_size.x+x];
                                chunk_pointer->moved[(y+1)*world.chunk_size.x+x+1] = 1;

                                chunk_pointer->tiles[y*world.chunk_size.x+x] = VOID;
                                chunk_pointer->colors[y*world.chunk_size.x+x] = BLANK;
                                chunk_pointer->moved[y*world.chunk_size.x+x] = 0;


                            }
                            else if (chunk_pointer->tiles[(y+1)*world.chunk_size.x+x-1]<2)
                            {

                                //printf("%i\n", SOLID);
                                //printf("%i the under\n", chunk_pointer->tiles[(y+1)*world.chunk_size.x+x]);
                                chunk_pointer->tiles[(y+1)*world.chunk_size.x+x-1] = FLUID;
                                chunk_pointer->colors[(y+1)*world.chunk_size.x+x-1] = chunk_pointer->colors[y*world.chunk_size.x+x];
                                chunk_pointer->moved[(y+1)*world.chunk_size.x+x-1] = 1;

                                chunk_pointer->tiles[y*world.chunk_size.x+x] = VOID;
                                chunk_pointer->colors[y*world.chunk_size.x+x] = BLANK;
                                chunk_pointer->moved[y*world.chunk_size.x+x] = 0;

                            }

                            else if (chunk_pointer->tiles[y*world.chunk_size.x+x+1]<2)
                            {
                                chunk_pointer->tiles[y*world.chunk_size.x+x+1] = FLUID;
                                chunk_pointer->colors[y*world.chunk_size.x+x+1] = chunk_pointer->colors[y*world.chunk_size.x+x];
                                chunk_pointer->moved[y*world.chunk_size.x+x+1] = 1;

                                chunk_pointer->tiles[y*world.chunk_size.x+x] = VOID;
                                chunk_pointer->colors[y*world.chunk_size.x+x] = BLANK;
                                chunk_pointer->moved[y*world.chunk_size.x+x] = 0;
                            }

                            else if (chunk_pointer->tiles[y*world.chunk_size.x+x-1]<2)
                            {
                                chunk_pointer->tiles[y*world.chunk_size.x+x-1] = FLUID;
                                chunk_pointer->colors[y*world.chunk_size.x+x-1] = chunk_pointer->colors[y*world.chunk_size.x+x];
                                chunk_pointer->moved[y*world.chunk_size.x+x-1] = 1;

                                chunk_pointer->tiles[y*world.chunk_size.x+x] = VOID;
                                chunk_pointer->colors[y*world.chunk_size.x+x] = BLANK;
                                chunk_pointer->moved[y*world.chunk_size.x+x] = 0;
                            }

                           
                        }
                    }
                }
            }
            int a = 0;

            for (int j = 0; j<world.outer_subchunks_length; j++)
            {
                // Technically both 1 over the border
                //printf("...");
                int left =  world.outer_subchunks_topleft[j].x + world.active_chunks2[i].chunk_pos.x*world.chunk_size.x;
                int top =   world.outer_subchunks_topleft[j].y +world.active_chunks2[i].chunk_pos.y*world.chunk_size.y;

                int right = left+world.subchunk_size.x;
                int bottom = top+world.subchunk_size.y;
                //printf("...ees");

                FullWorldIndex wi_bottom_left_sc = GeneralAccessXY(world, right, bottom);
                if (world.chunks[wi_bottom_left_sc.chunk_pos.y*world.chunk_count.x+world.active_chunks2[i].chunk_pos.x]==NULL)
                {
                    world.chunks[wi_bottom_left_sc.chunk_pos.y*world.chunk_count.x+world.active_chunks2[i].chunk_pos.x]=GenerateEmptyChunk(world.chunk_size,world.subchunk_size);
                }
                

                //for(int y = bottom-1; y>=world.outer_subchunks_topleft[j].y; y--)
                //printf("...");
                for(int y = bottom-1; y>=top; y--)
                {
                    
                    for(int x = right-1; x>=left; x--)
                    {
                        FullWorldIndex wi = GeneralAccessXY(world, x, y);
                        int index = y*world.chunk_size.x+x;


                      
                        if (world.chunks[wi.chunk]->moved[wi.in_chunk])
                        {
                            //printf("contin");
                            continue;
                        }

                        FullWorldIndex wi_under = GeneralAccessXY(world, x, y+1);
                        FullWorldIndex wi_under_right=  GeneralAccessXY(world, x+1, y+1);
                        FullWorldIndex wi_under_left=  GeneralAccessXY(world, x-1, y+1);

                        FullWorldIndex wi_up = GeneralAccessXY(world, x, y+1);
                        
                        FullWorldIndex wi_right = GeneralAccessXY(world, x+1, y);
                        FullWorldIndex wi_left = GeneralAccessXY(world, x-1, y);


                        if (world.chunks[wi_under.chunk] &&  world.chunks[wi_under_left.chunk] && world.chunks[wi_left.chunk] && world.chunks[wi_right.chunk], world.chunks[wi_under_right.chunk])
                            break;
 
                        //printf("%i, %i", wi.chunk, wi.in_chunk);
                        if (world.chunks[wi.chunk]->tiles[wi.in_chunk]==FLUID)
                        {
                            if ((world.chunks[wi_under.chunk]->tiles[wi_under.in_chunk]<2))
                            {
                                world.chunks[wi_under.chunk]->tiles[wi_under.in_chunk] = FLUID;
                                world.chunks[wi_under.chunk]->colors[wi_under.in_chunk] = world.chunks[wi.chunk]->colors[wi.in_chunk];
                                world.chunks[wi_under.chunk]->moved[wi_under.in_chunk] = 1;

                                world.chunks[wi.chunk]->tiles[wi.in_chunk] = VOID;
                                world.chunks[wi.chunk]->colors[wi.in_chunk] = BLANK;
                                world.chunks[wi.chunk]->moved[wi.in_chunk] = 0;
                                ActivateChunk(world, wi_under);



                            }
                            else if (world.chunks[wi_under_right.chunk]->tiles[wi_under_right.in_chunk]<2)
                            {

                                world.chunks[wi_under_right.chunk]->tiles[wi_under_right.in_chunk] = FLUID;
                                world.chunks[wi_under_right.chunk]->colors[wi_under_right.in_chunk] = world.chunks[wi.chunk]->colors[wi.in_chunk];
                                world.chunks[wi_under_right.chunk]->moved[wi_under_right.in_chunk] = 1;

                                world.chunks[wi.chunk]->tiles[wi.in_chunk] = VOID;
                                world.chunks[wi.chunk]->colors[wi.in_chunk] = BLANK;
                                world.chunks[wi.chunk]->moved[wi.in_chunk] = 0;
                                ActivateChunk(world, wi_under_right);



                            }                            

                            else if (world.chunks[wi_under_left.chunk]->tiles[wi_under_left.in_chunk]<2)
                            {

                                world.chunks[wi_under_left.chunk]->tiles[wi_under_left.in_chunk] = FLUID;
                                world.chunks[wi_under_left.chunk]->colors[wi_under_left.in_chunk] = world.chunks[wi.chunk]->colors[wi.in_chunk];
                                world.chunks[wi_under_left.chunk]->moved[wi_under_left.in_chunk] = 1;

                                world.chunks[wi.chunk]->tiles[wi.in_chunk] = VOID;
                                world.chunks[wi.chunk]->colors[wi.in_chunk] = BLANK;
                                world.chunks[wi.chunk]->moved[wi.in_chunk] = 0;
                                ActivateChunk(world, wi_under_left);

                            }

                            else if (world.chunks[wi_right.chunk]->tiles[wi_right.in_chunk]<2)
                            {
                                world.chunks[wi_right.chunk]->tiles[wi_right.in_chunk] = FLUID;
                                world.chunks[wi_right.chunk]->colors[wi_right.in_chunk] = world.chunks[wi.chunk]->colors[wi.in_chunk];
                                world.chunks[wi_right.chunk]->moved[wi_right.in_chunk] = 1;


                                world.chunks[wi.chunk]->tiles[wi.in_chunk] = VOID;
                                world.chunks[wi.chunk]->colors[wi.in_chunk] = BLANK;
                                world.chunks[wi.chunk]->moved[wi.in_chunk] = 0;
                                ActivateChunk(world, wi_right);

                            }

                            else if (world.chunks[wi_left.chunk]->tiles[wi_left.in_chunk]<2)
                            {
                                world.chunks[wi_left.chunk]->tiles[wi_left.in_chunk] = FLUID;
                                world.chunks[wi_left.chunk]->colors[wi_left.in_chunk] = world.chunks[wi.chunk]->colors[wi.in_chunk];
                                world.chunks[wi_left.chunk]->moved[wi_left.in_chunk] = 1;

                                world.chunks[wi.chunk]->tiles[wi.in_chunk] = VOID;
                                world.chunks[wi.chunk]->colors[wi.in_chunk] = BLANK;
                                world.chunks[wi.chunk]->moved[wi.in_chunk] = 0;
                                ActivateChunk(world, wi_left);

                            }

                           
                        }
                    }
                }
                //printf("godver wa nu weer\n");
                //if (a){a=21/0;}

            }
            

        }
    }
}

// Size should be larger than 0 (in both axes), anything else is undefined behaviour
void DrawWorldDebug(Map world, Vector2i offset, Vector2i size, RenderTexture2D target)
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
                world.chunks[row_offset+left_chunk+x]= GenerateEmptyChunk(world.chunk_size, world.subchunk_size);
            }

            
            DrawPartOfChunk(world.chunks[row_offset+left_chunk+x], 

            origin_in_chunk, end_in_chunk, 
            offset_c, world.chunk_size, target);

            offset_c.x += end_in_chunk.x - origin_in_chunk.x;
        }
        offset_c.y += end_in_chunk.y - origin_in_chunk.y;

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
                world.chunks[row_offset+left_chunk+x]= GenerateEmptyChunk(world.chunk_size, world.subchunk_size);
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
    int subchunk_x;
    int subchunk_y;

    /*
    // sc for subchunk
    for (int x = 0; x<512; x++)
    {
        subchunk_x = x/8;
        for (int y = origin_in_chunk.y/8; y<512; y++)
        {
            subchunk_y = y/8;
            if ((x+y)%2)
            {
                DrawRectangle(x*8+offset.x, y*8+offset.y, 8, 8, (Color){.r=240, .g= 240, .b=240, .a=255});
            }
            else
            {
                DrawRectangle(x*8+offset.x, y*8+offset.y, 8, 8, (Color){.r=220, .g= 220, .b=220, .a=255});

            }
        }
            
    }*/

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


