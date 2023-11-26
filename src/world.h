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


typedef enum {
    VOID = 0,
    SOLID = 1,
    FLUID = 2,
    GAS = 3

} TileState;


typedef struct Tile{
    TileState type;
    Color color;
} Tile;

// Should be noted that the top left of the world corresponds to arr[0][0]
typedef struct World{
    Tile *arr;
    Vector2i size;
} World;

// All tiles
Tile SAND = {.type = SOLID, .color = {.r = 100, .b = 100, .g = 100, .a = 255}};
Tile EMPTY = {.type = VOID, .color = {.r = 100, .b = 100, .g = 100, .a = 0}};






// World_size is accessed like this World_size[y][x]
void LoadTileTextures();
void RenderWorld(Tile *world, Vector2i world_size);
void ChangeTile(Tile *world, Vector2i world_size, Vector2i position);
void ScrollWorld(World world, Camera camera, RenderTexture2D static_world, Vector2 scoll)


#endif // WORLD_H