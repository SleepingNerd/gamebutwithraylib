#include <raylib.h>

#ifndef WORLD_H
#define WORLD_H



#define TILE_SIZE 16
#define TILES 1

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





// World_size is accessed like this World_size[y][x]
void LoadTileTextures();
void RenderWorld(Tile *world, Vector2i world_size);
void ChangeTile(Tile *world, Vector2i world_size, Vector2i position);

#endif // WORLD_H