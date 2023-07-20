/* include guard */
#ifndef BALLOON_H
#define BALLOON_H

#include "object.h"

typedef struct Balloon {
    Object obj;
    int color;
} Balloon;

int balloon_init(Balloon* balloon, double x, double z, int color);

void set_balloon_pos(Balloon* balloon, double x, double y);

int get_balloon_x(Balloon balloon);
int get_balloon_z(Balloon balloon);

int get_balloon_pixel(int px, int pz);

char get_balloon_aa(int px, int pz);

int get_balloon_color(Balloon balloon);

#endif  // BALLOON_H