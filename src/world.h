#include <raylib.h>
#include "samath.h"


#ifndef WORLD_H
#define WORLD_H



#define TILE_SIZE 1
#define TILES 1

// Macro to conver x and y coordinates into the
//#define COORD_TO_INDEX(X, Y, W_SIZE) ({typeof(x) x = x}) I don't care for now

extern char* texture_names[TILES];
extern Texture2D textures[TILES];

//PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
typedef enum {
    VOID =  0,
    SOLID = 1,
    FLUID = 2,
    GAS = 3

} TileState;


typedef struct Tile{
    TileState type;
    Color color;
} Tile;



// Should be noted that the top left of the world corresponds to arr[0][0]
// arr and color should be the same size
typedef struct World{
    TileState *tiles;
    Color *colors;
    Vector2i size;
} World;

// All tiles



//PIXELFORMAT_UNCOMPRESSED_R8G8B8A8



// World_size is accessed like this World_size[y][x]
void LoadTileTextures();
void RenderWorld(World world, Vector2i world_size);
void ChangeTile(World world, Vector2i world_size, Vector2i position);
void ScrollWorld(World world, Camera camera, RenderTexture2D static_world, Vector2 scoll);

// Allocates and creates an empty world
void EmptyWorld(World *world);



#endif // WORLD_H