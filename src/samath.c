#include "samath.h"

int floor_to_muiltiple(long x, long m)
{
    return x-(x%m);
}

int ceiling_to_muiltiple(long x, long m)
{
    return x+(x%m);
}
