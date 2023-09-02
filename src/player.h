#include "raylib.h"


typedef enum {
    IDLE = 0,
    RUN = 1
} PlayerState;

typedef struct Player{
    Rectangle rect;
    Vector2 velocity;
    PlayerState state;
    int speed;
    int jump_force;
    int gravity;
    
    
} Player;

void CalculateJump(Player *player, int height, float peak_time);

void ProcessInput(Player *player, bool left, bool right, bool jump);
void MoveAndUpdate(Player *player, float delta);

