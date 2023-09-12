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
    
    int bottom = player->rect.y + player->rect.height;



    // Describes tile rect of rect
    int top_tile = floor_to_muiltiple(player->rect.y, TILE_SIZE)/TILE_SIZE;
    int bottom_tile = floor_to_muiltiple(bottom, TILE_SIZE)/TILE_SIZE;

    int right_tile = floor_to_muiltiple(player->rect.x+player->rect.width, TILE_SIZE)/TILE_SIZE;
    int left_tile = floor_to_muiltiple(player->rect.x, TILE_SIZE)/TILE_SIZE;


    int top_tile_1  =floor_to_muiltiple(player->rect.y-1, TILE_SIZE)/TILE_SIZE;



    int left_tile_1 = floor_to_muiltiple(player->rect.x-1, TILE_SIZE)/TILE_SIZE;




    player->rect.y += player->velocity.y*delta;

    
    // Going down
    if (player->velocity.y > 0)
    {
        
        for(int i = left_tile; i<= right_tile; i++)
        {
            if (world.arr[bottom_tile* world.size.x +i].type == SAND)
            {
                player->rect.y = bottom_tile * TILE_SIZE- player->rect.height; 
                player->velocity.y = 0;
                
            }
        }
    }

    // Going up
    else if (player->velocity.y < 0)
    {

        for(int i = left_tile; i<= right_tile; i++)
        {
            if (world.arr[top_tile* world.size.x + i].type == SAND)
            {

                player->rect.y = (top_tile+1) * TILE_SIZE; 
            }
        }
     
    }


    player->rect.x += player->velocity.x*delta;
    /*
    // Going right
    if (player->velocity.x> 0)
    {
        printf("%i; %i\n", top_tile, bottom_tile);

        for(int i = top_tile; i<= bottom_tile; i++)
        { 
            if (world.arr[i *world.size.x+ right_tile].type ==SAND)
            {
                player->rect.x = (right_tile)*TILE_SIZE- player->rect.width;
                player->velocity.x = 0;
                break;
            }
        }
       
    }
    else if (player->velocity.x< 0)
    {
        for(int i = top_tile; i<= bottom_tile; i++)
        {
            if (world.arr[i *world.size.x+ left_tile_1].type ==SAND)
            {
                player->rect.x = (left_tile_1+1)*TILE_SIZE;

                player->velocity.x = 0;
                break;
            }
        }
    }
    */

    player->velocity.y += player->gravity*delta;
}

#endif // PLAYER_H
