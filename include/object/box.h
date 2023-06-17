/* include guard */
#ifndef BOX_H
#define BOX_H

#include "object.h"

typedef struct Box {
    Object obj;
    int color;
} Box;

void box_init(Box* box, int id, double x, double y, int size_x, int size_y,
              int color);
void set_box_pos(Box* box, double x, double y);

double get_box_x(Box box);
double get_box_z(Box box);
int get_box_size_x(Box box);
int get_box_size_z(Box box);
int get_box_color(Box box);

int is_box_followed(Box box);

void box_update(Box* box, double d_sec);

#endif  // BOX_H