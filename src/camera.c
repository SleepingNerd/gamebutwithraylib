#include "camera.h"

int Scroll(View *camera, float scroll_x,  float scroll_y)
{
    camera->offset.x += scroll_x;
    camera->offset.y +=scroll_y;
    camera->p_offset.x = (int)camera->offset.x;
    camera->p_offset.y = (int)camera->offset.y;
}