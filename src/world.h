#include <raylib.h>
#include "samath.h"


#ifndef WORLD_H
#define WORLD_H



#define TILE_SIZE 1
#define TILES 1
// I wasn't on planning on making these pre-set but my hand got forced
#define CHUNK_SIZE_X 512
#define CHUNK_SIZE_Y 512


// Macro to conver x and y coordinates into the
//#define COORD_TO_INDEX(X, Y, W_SIZE) ({typeof(x) x = x}) I don't care for now

extern char* texture_names[TILES];
extern Texture2D textures[TILES];

//PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
typedef enum {
    VOID =  0,
    GAS = 1,
    FLUID = 2,
    SOLID = 3
} TileState;




typedef struct Chunk{
    TileState *tiles;
    Image image;
    Color *colors;
    char *moved;
} Chunk;

typedef struct World{
    Vector2i size;
    TileState *tiles;
    Color *colors;

} World; 

typedef struct Map{
    Vector2i chunk_count;
    Vector2i chunk_size;

    Chunk **chunks;
    Chunk **beany_chunks;
} Map; 

// Should be noted that the top left of the world corresponds to arr[0][0]
// arr and color should be the same size
/*
typedef struct World{
    Vector2i chunk_count;
    Vector2i chunk_size;
    Chunk *chunks;
} World; */

// All tiles



//PIXELFORMAT_UNCOMPRESSED_R8G8B8A8



// World_size is accessed like this World_size[y][x]
void LoadTileTextures();
void RenderWorld(World world, Vector2i world_size);


// Allocates and creates an empty world
void EmptyWorld(World *world, Vector2i size);
void ChangeTile(Map world, Vector2i position, TileState tile_state, Color color);
TileState GetTileState(Map world, int x, int y);

void DrawWorld(Map world, Vector2i offset, Vector2i size, RenderTexture2D target);

Chunk *GenerateEmptyChunk(Vector2i chunk_size);
void DrawPartOfChunk(Chunk *c, Vector2i origin_in_chunk, Vector2i end_in_chunk, Vector2i offset, Vector2i chunk_size, RenderTexture2D target);
Map GenerateEmptyWorld(Vector2i chunk_size, Vector2i chunk_count);

void ScrollWorld(World world, Camera camera, RenderTexture2D static_world, Vector2 scoll);

void SimulateWorld(Map world);

void AddActiveChunk(Map world, Chunk* chunk_p);

// io
void SaveWorld(World world, char *path);




#endif // WORLD_H