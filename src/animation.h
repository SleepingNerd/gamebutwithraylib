#include "raylib.h"

// I doubt you'll ever need more than the [0, 255] range for this
// delta is the elapsed time since last frame 
// frames[frame] is the current_frame image
typedef struct Animation {
    unsigned char frames;
    float time_between_frames;
    Image *frames;
} Animation;

typedef struct AnimationManager {
    unsigned char frame;

        
} AnimationManager;

