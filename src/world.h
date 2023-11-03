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
    SAND = 1,
    DIRT = 2,
} TileType;


typedef struct Tile{
    TileType type;
    Color color;
} Tile;

// Should be noted that the top left of the world corresponds to arr[0][0]
typedef struct World{
    Tile *arr;
    Vector2i size;
} World;





// World_size is accessed like this World_size[y][x]
void LoadTileTextures();
void RenderWorld(Tile *world, Vector2i world_size);
void ChangeTile(Tile *world, Vector2i world_size, Vector2i position);
void ScrollWorld(Tile *world, Vector2i scroll, RenderTexture2D static_world);


#endif // WORLD_H