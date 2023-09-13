#include "samath.h"

int floor_to_muiltiple(int x, int m)
{
    return x-(x%m);
}

int ceiling_to_muiltiple(int x, int m)
{
    return x+(x%m);
}
