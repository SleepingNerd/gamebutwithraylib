#include "raylib.h"
#include "world.h"
#include "samath.h"




typedef enum {
    IDLE = 0,
    RUN = 1
} PlayerState;

typedef struct Player{
    Rectangle rect;
    Vector2i t_size;
    Vector2 velocity;
    PlayerState state;
    int speed;
    int jump_force;
    int gravity;
} Player;

void CalculateJump(Player *player, int height, float peak_time);
void CalculateTileSize(Player *player); // Calculates  the "tile size" of the player (t_size)
void ProcessInput(Player *player, bool left, bool right, bool jump);
void MoveAndUpdate(Player *player, float delta, World world);

