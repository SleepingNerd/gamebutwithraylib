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
    player->rect.height = height;

    player->p_offset.x = width-1;
    player->p_offset.y = height-1;
}

void ProcessInput(Player *player, bool left, bool right, bool jump)
{
    player->velocity.x = (right- left)*player->speed;


    if (jump && player->grounded)
    {
        player->velocity.y = player->jump_force;
    } 
}



bool Collision(Player *player, World world, int left_tile, int right_tile, int top_tile, int bottom_tile)
{
    printf("%i; %i, %i; %i\n", left_tile,right_tile,top_tile,bottom_tile);
    for (int x = left_tile; x<=right_tile; x++)
    {
        
        for (int y = top_tile; y<=bottom_tile; y++)
        {
            printf("%i, %i\n", x, y);
            if (world.arr[(x)+(y *world.size.x)].type == SAND)
            {
                printf("A");
                return true;
            }
        }
    }
    printf("...\n");
    return false;

}

// Assumes player last moved down and applies collision accordingly, should be used as sparingly as possible,
// Always check if player actually crossed a tile before calling
bool DownCollision(Player *player, World world, int left_tile, int right_tile)
{
    int bottom_tile = (player->rect.y+player->p_offset.y)/TILE_SIZE;


       
        for(int i = left_tile; i<=right_tile; i++)
        {
            // Apply setback if moved into tile
            if (world.arr[(i)+(bottom_tile *world.size.x)].type == SAND)
            {
                player->rect.y = (bottom_tile)*TILE_SIZE-player->rect.height;
                return true;
                
            }

        }
    return false;
}

// Assumes player last moved up and applies collision accordingly, should be used as sparingly as possible,
// Always check if player actually crossed a tile before calling
bool UpCollision(Player *player, World world, int left_tile, int right_tile)
{
    int top_tile = (player->rect.y)/TILE_SIZE;

    for(int i = left_tile; i<=right_tile; i++)
    {
        // Apply setback if moved into tile
        if (world.arr[(i)+(top_tile *world.size.x)].type == SAND)
        {
            player->rect.y = (top_tile+1)*TILE_SIZE;
            return true;
        }
    }
    return false;
}

// Assumes player last moved right and applies collision accordingly, should be used as sparingly as possible,
// Always check if player actually crossed a tile before calling
bool RightCollision(Player *player, World world, int top_tile, int bottom_tile)
{

        int right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;


        for(int i = top_tile; i<= bottom_tile; i++)
        {
            if (world.arr[(right_tile)+(i *world.size.x)].type == SAND)
            {
                
                float original_y = player->rect.y;
                int left_tile = player->rect.x/TILE_SIZE;
                float original_x = player->rect.x;
                

                // This is extremely slow
                // From 1 tile above the bottom_tile to (incl.) slide_up above bottom_tile
                // The first slide_up times I should do this:


                // How many tiles the player would slide up 
                // (if he were to slide up the current tile)
                int slide_up_tile = (bottom_tile-i)+1;

                // If there is a tile blocking movement that is above player->slide_up
                if (slide_up_tile > player->slide_up)
                {
                    player->rect.x = right_tile*TILE_SIZE-player->rect.width;
                    return true;
                }

                // If there is a possibility to slide_up
                else 
                {
                    // Check the top slide_up_tile tiles, this is just done by looping those tiles
                    // 
                    int left_tile = player->rect.x/TILE_SIZE;
                    int new_top_tile = top_tile - slide_up_tile;

                    for (int x = left_tile; x<=right_tile; x++)
                    {
                        for(int y = top_tile-1; y>=new_top_tile; y--)
                        {

                            if (world.arr[(x)+(y *world.size.x)].type == SAND)
                            {
                                player->rect.x = right_tile*TILE_SIZE-player->rect.width;
                                return true;
                            }
                        }
                    }


                    player->rect.y = (new_top_tile)*TILE_SIZE;
                    return false;
                }

            }
        }
    return false;
}

bool LeftCollision(Player *player, World world, int top_tile, int bottom_tile)
{
    int left_tile = (player->rect.x)/TILE_SIZE;
    for(int i = top_tile; i<= bottom_tile; i++)
    {
        if (world.arr[(left_tile)+(i *world.size.x)].type == SAND)
        {
            
             // How many tiles the player would slide up 
            // (if he were to slide up the current tile)
            int slide_up_tile = (bottom_tile-i)+1;

            // If there is a tile blocking movement that is above player->slide_up
            if (slide_up_tile > player->slide_up)
            {
                player->rect.x = (left_tile+1)*TILE_SIZE;
                return true;
            }

            // If there is a possibility to slide_up
            else 
            {
                // Check the top slide_up_tile tiles, this is just done by looping those tiles
                // 
                int right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;;
                int new_top_tile = top_tile - slide_up_tile;

                for (int x = left_tile; x<=right_tile; x++)
                {
                    for(int y = top_tile-1; y>=new_top_tile; y--)
                    {

                        if (world.arr[(x)+(y *world.size.x)].type == SAND)
                        {
                            player->rect.x = (left_tile+1)*TILE_SIZE;
                            return true;
                        }
                    }
                }


                player->rect.y = (new_top_tile)*TILE_SIZE;
                return false;
                }
       
            player->rect.x = (left_tile+1)*TILE_SIZE;
            return true;
        }
    }
    return false;
}

bool HorizontalMoveAndUpdate(Player *player, World world, float delta)
{

    int top_tile = (player->rect.y)/TILE_SIZE;
    int bottom_tile = (player->rect.y+player->p_offset.y)/TILE_SIZE;

    // All relate to movement on x axis:
    float to_be_moved =  player->velocity.x*delta;
    float offset_from_tile =  fmodf(player->rect.x, TILE_SIZE);
    float move_offset = fmodf(player->velocity.x*delta, TILE_SIZE);
    
    // Going right
    if (player->velocity.x > 0)
    {
        for (int i = 0; i<(int)to_be_moved; i+=TILE_SIZE)
        {
            player->rect.x += 1;

            if (RightCollision(player, world, top_tile, bottom_tile))
            {
                return true;
            }
        }

        player->rect.x += move_offset;
        if ((offset_from_tile+move_offset)>=1)
        {
            RightCollision(player, world, top_tile, bottom_tile);
        }

        player->facing = RIGHT;

    }

    else if (player->velocity.x <0)
    {
        for (int i = 0; i>(int)to_be_moved; i-=TILE_SIZE)
        {
            player->rect.x -= 1;
            if (LeftCollision(player, world, top_tile, bottom_tile))
            {
                return true;
            }
        }

        player->rect.x += move_offset;
        if ((offset_from_tile+move_offset)<=0)
        {
            LeftCollision(player, world, top_tile, bottom_tile);
        }
        
        player->facing = LEFT;
    }
}

bool VerticalMoveAndUpdate(Player *player, World world, float delta)
{
    // Calculate left and right tile, this might still be optimised
    int left_tile = (player->rect.x)/TILE_SIZE;
    int right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;

    // All relate to movement on y axis:
    float to_be_moved =  player->velocity.y*delta;
    float offset_from_tile =  fmodf(player->rect.y, TILE_SIZE);
    float move_offset = fmodf(player->velocity.y*delta, TILE_SIZE);

     // Going down
    if (player->velocity.y> 0)
    {

        for (int i = 0; i<(int)to_be_moved; i+=TILE_SIZE)
        {
            player->rect.y += TILE_SIZE;
            if (DownCollision(player, world, left_tile, right_tile))
            {
                player->grounded=true;
                return true;
            }
            

        }
        player->rect.y += move_offset;

        
        // Collide again
        if ((offset_from_tile+move_offset) >= 1)
        {
            
            if (DownCollision(player, world, left_tile, right_tile))
            {
                player->grounded=true;
                player->velocity.y = 0;
            }
        }
    }

    else if (player->velocity.y< 0)
    {
        
        for (int i = 0; i>(int)to_be_moved; i-=TILE_SIZE)
        {
            player->rect.y -= TILE_SIZE;
            if (UpCollision(player, world, left_tile, right_tile))
            {
                return true;
            }

        }
        
        player->rect.y += move_offset;

        // Collide again
        if ((offset_from_tile+move_offset) <= 0)
        {
            
            UpCollision(player, world, left_tile, right_tile);
        }
    }

}






// Applies and updates velocity and position, applies gravity
void MoveAndUpdate(Player *player, float delta, World world)
{

    player->grounded = false;


    VerticalMoveAndUpdate(player, world, delta);

    
    HorizontalMoveAndUpdate(player, world, delta);


    // Applying gravity    
    player->velocity.y += player->gravity*delta;
    if (player->velocity.y > 100)
    {
        player->velocity.y = 100;
    }
}



#endif // PLAYER_H
