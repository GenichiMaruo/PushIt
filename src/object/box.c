#include "../../include/object/box.h"

/* initialize box */
void box_init(Box* box, double x, double z, int size_x, int size_z, int color) {
    object_init(&box->obj, x, z, 0, 0, size_x, size_z);
    box->color = color;
}

double box_get_x(Box box) { return box.obj.x; }
double box_get_z(Box box) { return box.obj.z; }
int box_get_size_x(Box box) { return box.obj.hitbox.size_x; }
int box_get_size_z(Box box) { return box.obj.hitbox.size_z; }
int box_get_color(Box box) { return box.color; }

/* update box */
void box_update(Box* box, double d_sec) { object_update(&box->obj, d_sec); }