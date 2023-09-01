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
        printf("I don't give a shit right now");
    }

    Animation animation = {.frames = frames, .time_between_frames = time_between_frames, .frame_arr=malloc(sizeof(Image)*frames)};
    int frame_width = sprite_sheet.width/frames;
    for(int i = 0; i<frames; i++)
    {
        Rectangle rec = {.x = frame_width*i, .y = 0, .width=frame_width, .height = sprite_sheet.height};
        animation.frame_arr[i] = ImageFromImage(sprite_sheet, rec);
    }
    return animation;
}
bool UpdateAnimationManager(AnimationManager*, an
{
    anim_m_test.delta += GetFrameTime();
        if (anim_m_test.delta >= test_anim.time_between_frames)
        {
            anim_m_test.delta = 0;
            anim_m_test.frame += 1;
            if (anim_m_test.frame == test_anim.frames)
            {
                anim_m_test.frame = 0;
            }
        }
}