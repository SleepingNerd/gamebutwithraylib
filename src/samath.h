#ifndef SAMATH_H
#define SAMATH_H

typedef struct Vector2i {
    int x;
    int y;
} Vector2i;

typedef Vector2i Vect2i;



typedef struct Recti {
    int x;
    int y;
    int width;
    int height;
} Recti;

int floor_to_muiltiple(int x, int m);
int ceiling_to_muiltiple(int x, int m);

#endif // SAMATH_H
