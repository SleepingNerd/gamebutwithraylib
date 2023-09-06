#include <stdlib.h>
#include "sanimation.h"
#include "raylib.h"

// Only supports animations stored from left to right
// Probally not perfectly optimised, but that doesn't really matter here
Animation LoadAnimation(const char *path, frame_i frames, float time_between_frames)
{
    Image sprite_sheet = LoadImage(path);

    if (sprite_sheet.width%frames!=0)
    {
        printf("Frame_count doesn't match spritesheet! ");
    }

    Animation animation = {.frames = frames, .time_between_frames = time_between_frames, .frame_arr=malloc(sizeof(Texture2D)*frames)};
    int frame_width = sprite_sheet.width/frames;
    for(int i = 0; i<frames; i++)
    {
        Rectangle rec = {.x = frame_width*i, .y = 0, .width=frame_width, .height = sprite_sheet.height};
        animation.frame_arr[i] = LoadTextureFromImage(ImageFromImage(sprite_sheet, rec));

    }
    return animation;
}

// Returns 
bool UpdateAnimationManager(AnimationManager *anim_manager, Animation *anim, float delta)
{
    anim_manager->delta += delta;
        if (anim_manager->delta >= anim->time_between_frames)
        {
            anim_manager->delta = 0;
            anim_manager->frame += 1;
            if (anim_manager->frame == anim->frames)
            {
                anim_manager->frame = 0;
            }
        }
    return true;
}