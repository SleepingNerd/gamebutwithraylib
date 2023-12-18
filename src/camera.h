#include "raylib.h"
#include "samath.h"


#ifndef CAMERA_H
#define CAMERA_H

//
typedef struct View {
    Vector2 offset;    // Left top
    Vector2i p_offset; // Left top index position
} View;

int Scroll(View *camera, float scroll_x,  float scroll_y);










#endif // CAMERA_H