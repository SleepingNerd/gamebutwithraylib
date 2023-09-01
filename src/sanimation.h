#include "raylib.h"

#ifndef SANIMATION_H
#define SANIMATION_H

#define COLOR_SIZE 4

// Stands for "frame index", can store as the frames as is supported
typedef unsigned char frame_index;
typedef frame_index frame_i;

// I doubt you'll ever need more than the [0, 255] range for this
// delta is the elapsed time since last frame 
// frames[frame] is the current_frame image
typedef struct Animation {
    frame_i frames;
    float time_between_frames;
    Image *frame_arr;
} Animation;

typedef struct AnimationManager {
    frame_i frame;
    float delta;        
} AnimationManager;

Animation LoadAnimation(const char *path, frame_i frames, float time_between_frames);


#endif