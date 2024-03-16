#include "samath.h"
#include <stdio.h>


void printv2i(Vector2i i)
{
    printf("%i, %i\n", i.x, i.y);
}

int floor_to_muiltiple(int x, int m)
{
    return x-(x%m);
}

int ceiling_to_muiltiple(int x, int m)
{
    return x+(x%m);
}
