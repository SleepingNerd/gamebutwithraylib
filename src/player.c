#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "player.h"

// Calculates your jump, and applies the parameters
void CalculateJump(Player *player, int height, float peak_time)
{
    player->gravity=height/peak_time;
    player->jump_force=-height;
}

void ProcessInput(Player *player, bool left, bool right, bool jump)
{
    player->velocity.x = (right- left)*player->speed;


    if (jump)
    {
        player->velocity.y = player->jump_force;
    } 
}

// Applies and updates velocity and position
void MoveAndUpdate(Player *player, float delta)
{
    player->rect.x += player->velocity.x*delta;

    player->rect.y += player->velocity.y*delta;
    player->velocity.y += player->gravity*delta;

}

#endif // PLAYER_H
