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
                

                printf("X %i ,%i ,%i\n", (bottom_tile-player->slide_up), bottom_tile, player->slide_up);
                for (int h = bottom_tile-1; h>=(bottom_tile-player->slide_up); h--)
                {
                    if (world.arr[(right_tile)+((h) *world.size.x)].type != SAND)
                    {   
                        player->rect.y = (h-player->p_offset.y)*TILE_SIZE;
                        if (!Collision(player, world, left_tile, right_tile, (bottom_tile*TILE_SIZE-player->p_offset.y)/TILE_SIZE, h))
                        {
                            return false;
                        }
                        printf("f");
                        
                    }  
                }
                player->rect.y = original_y;

                
                player->rect.x = right_tile*TILE_SIZE-player->rect.width;
                return true;
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
            float original_y = player->rect.y;
            int right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;;
                

            for (int h = bottom_tile-1; h>=(bottom_tile-player->slide_up); h--)
            {
                printf("%i\n", h);
                if (world.arr[(left_tile)+((h) *world.size.x)].type != SAND)
                {   
                    player->rect.y = (h-player->p_offset.y)*TILE_SIZE;
                    if (!Collision(player, world, left_tile, right_tile, bottom_tile-player->p_offset.y/TILE_SIZE, h))
                    {
                        return false;
                    }
                        
                }  
            }
            player->rect.y = original_y;
            player->rect.x = (left_tile+1)*TILE_SIZE;
            return true;
        }
    }
    return false;
}


// Applies and updates velocity and position, applies gravity
void MoveAndUpdate(Player *player, float delta, World world)
{
    int left_tile = (player->rect.x)/TILE_SIZE;
    int right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;

    int top_tile;
    int bottom_tile;
    
    
    Vector2 to_be_moved = {.y = player->velocity.y*delta, .x =player->velocity.x*delta};
    Vector2 offset_from_tile = {.x= fmodf(player->rect.x, TILE_SIZE), .y= fmodf(player->rect.y, TILE_SIZE)};
    Vector2 move_offset = {.x =  fmodf(player->velocity.x*delta, TILE_SIZE), .y= fmodf(player->velocity.y*delta, TILE_SIZE)};

   

   

    // Going down
    if (player->velocity.y> 0)
    {

        for (int i = 0; i<(int)to_be_moved.y; i+=TILE_SIZE)
        {
            player->rect.y += TILE_SIZE;
            DownCollision(player, world, left_tile, right_tile);

        }
        

        player->rect.y += move_offset.y;

        
        // Collide again
        if ((offset_from_tile.y+move_offset.y) >= 1)
        {
            
            if (DownCollision(player, world, left_tile, right_tile))
            {
                player->velocity.y = 0;
            }
        }



        // Recalculate top and bottom after movement
        top_tile = (player->rect.y)/TILE_SIZE;
        bottom_tile = (player->rect.y+player->p_offset.y)/TILE_SIZE;
    }

    else if (player->velocity.y< 0)
    {
        
        for (int i = 0; i>(int)to_be_moved.y; i-=TILE_SIZE)
        {
            player->rect.y -= TILE_SIZE;
            UpCollision(player, world, left_tile, right_tile);

        }
        
        player->rect.y += move_offset.y;

        // Collide again
        if ((offset_from_tile.y+move_offset.y) <= 0)
        {
            
            UpCollision(player, world, left_tile, right_tile);
        }






        // Recalculate top and bottom after movement
        top_tile = (player->rect.y)/TILE_SIZE;
        bottom_tile = (player->rect.y+player->p_offset.y)/TILE_SIZE;
    }


    
   

    
    

    left_tile = (player->rect.x)/TILE_SIZE;
    right_tile = (player->rect.x+player->p_offset.x)/TILE_SIZE;



    

    // Going right
    if (player->velocity.x > 0)
    {
        for (int i = 0; i<(int)to_be_moved.x; i+=TILE_SIZE)
        {
            player->rect.x += 1;
            RightCollision(player, world, top_tile, bottom_tile);
        }

        player->rect.x += move_offset.x;
        if ((offset_from_tile.x+move_offset.x)>=1)
        {
            RightCollision(player, world, top_tile, bottom_tile);
        }

        player->facing = RIGHT;

    }

    else if (player->velocity.x <0)
    {
        for (int i = 0; i>(int)to_be_moved.x; i-=TILE_SIZE)
        {
            player->rect.x -= 1;
            LeftCollision(player, world, top_tile, bottom_tile);
        }

        player->rect.x += move_offset.x;
        if ((offset_from_tile.x+move_offset.x)<=0)
        {
            LeftCollision(player, world, top_tile, bottom_tile);
        }
        
        player->facing = LEFT;
    }





    // Applying gravity    
    player->velocity.y += player->gravity*delta;
    if (player->velocity.y < 5)
    {
        player->velocity.y = 5;
    }
}



#endif // PLAYER_H
