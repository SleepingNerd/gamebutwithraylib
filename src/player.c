#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "player.h"
#include "world.h"
#include "samath.h"
 
// Calculates your jump, and applies the parameters
void CalculateJump(Player *player, int height, float peak_time)
{
    player->gravity=height/peak_time;
    player->jump_force=-height;
}

void CalculateTileSize(Player *player)
{
    player->t_size.x = ceil(player->rect.width/TILE_SIZE);
    player->t_size.y = ceil(player->rect.height/TILE_SIZE);
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
void MoveAndUpdate(Player *player, float delta, World world)
{

    
    
    player->rect.x += player->velocity.x*delta;

    // Going right
    if (player->velocity.x > 0)
    {
        int right = player->rect.x + player->rect.width;
        int bottom = player->rect.y + player->rect.height;
        
        for (int tracker = player->rect.y; tracker <= )
        
        if (world.arr[floor_to_muiltiple(player->rect.y, TILE_SIZE)*world.size.y + floor_to_muiltiple((long)right, TILE_SIZE)].type == SAND)
        {
            player->rect.x = (floor_to_muiltiple((long) player->rect.x, TILE_SIZE) -1)* TILE_SIZE;
            printf("?????????????????????????????????\n");
        }
      

    }

    int top_left = floor_to_muiltiple(player->rect.x, TILE_SIZE);



    

    player->rect.y += player->velocity.y*delta;
    player->velocity.y += player->gravity*delta;

}

#endif // PLAYER_H
