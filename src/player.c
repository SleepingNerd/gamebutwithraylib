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

void CalculateSize(Player *player, int width, int height)
{
    player->rect.width = width;
    player->rect.width = height;

    player->p_offset.x = width-1;
    player->p_offset.y = height-1;
}

void ProcessInput(Player *player, bool left, bool right, bool jump)
{
    player->velocity.x = (right- left)*player->speed;


    if (jump)
    {
        printf("?????????????????,,");
        player->velocity.y = player->jump_force;
    } 
}

// Applies and updates velocity and position, applies gravity
void MoveAndUpdate(Player *player, float delta, World world)
{

    int left_tile = (player->rect.x)/TILE_SIZE;
    int right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;

    int top_tile;
    int bottom_tile;
    
    player->rect.y += player->velocity.y*delta;

    

    // Going down
    if (player->velocity.y> 0)
    {
        bottom_tile = (player->rect.y+player->p_offset.y)/TILE_SIZE;


        printf("%f\n", (player->rect.y+player->p_offset.y));
        printf("%i, %i, %f ..\n", top_tile, bottom_tile, player->rect.y);
        for(int i = left_tile; i<=right_tile; i++)
        {
            // Apply setback if moved into tile
            if (world.arr[(i)+(bottom_tile *world.size.x)].type == SAND)
            {
                player->rect.y = (bottom_tile)*TILE_SIZE-player->rect.height;
                break;
            }

        }

        // Recalculate top and bottom after movement
        top_tile = (player->rect.y+player->p_offset.y)/TILE_SIZE;
        bottom_tile = (player->rect.y+player->p_offset.y)/TILE_SIZE;
    }

    else if (player->velocity.y< 0)
    {
        top_tile = (player->rect.y+player->p_offset.y)/TILE_SIZE;
        for(int i = left_tile; i<=right_tile; i++)
        {
            // Apply setback if moved into tile
            if (world.arr[(i)+(bottom_tile *world.size.x)].type == SAND)
            {
                player->rect.y = (bottom_tile)*TILE_SIZE-player->rect.height;
                break;
            }

        }
        

    }


    
    left_tile = (player->rect.x)/TILE_SIZE;
    right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;
    


    

    player->rect.x += player->velocity.x*delta;
    // Going right
    if (player->velocity.x > 0)
    {
        right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;
        for(int i = top_tile; i<= bottom_tile; i++)
        {
            if (world.arr[(right_tile)+(i *world.size.x)].type == SAND)
            {
                player->rect.x = right_tile*TILE_SIZE-player->rect.width;
            }
        }
    }



    // Applying gravity    
    player->velocity.y += player->gravity*delta;
    if (player->velocity.y < 10)
    {
        player->velocity.y = 10;
    }

    printf("%f\n", (player->rect.y+player->p_offset.y));
    printf("%i, %i, %f ...\n", top_tile, bottom_tile, player->rect.y);
}

#endif // PLAYER_H
