/* include guard */
#ifndef BOX_H
#define BOX_H

#include "object.h"

typedef struct Box {
    Object obj;
    int color;
} Box;

void box_init(Box* box, double x, double y, int size_x, int size_y, int color);

void box_update(Box* box, double d_sec);

#endif  // BOX_H