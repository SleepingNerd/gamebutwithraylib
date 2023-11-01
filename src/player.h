#include "raylib.h"
#include "world.h"
#include "samath.h"



typedef enum {
    IDLE = 0,
    RUN = 1
} PlayerState;

typedef enum{
    LEFT = -1,
    RIGHT = 1
} HorizontalFacing;

// p
typedef struct Player{
    Rectangle rect;
    Vector2i p_offset; // Offset from (0, 0) pixel in the top-left corner
    Vector2 velocity;
    PlayerState state;

    HorizontalFacing facing;
    int speed;
    int jump_force;
    int gravity;
    int slide_up // How much tiles it's allowed to slide up


} Player;

void CalculateSize(Player *player , int width, int height);
void CalculateJump(Player *player, int height, float peak_time);
void ProcessInput(Player *player, bool left, bool right, bool jump);
void MoveAndUpdate(Player *player, float delta, World world);

