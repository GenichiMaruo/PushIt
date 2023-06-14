#include "../../include/object/box.h"

/* initialize box */
void box_init(Box* box, double x, double z, int size_x, int size_z, int color) {
    object_init(&box->obj, x, z, 0, 0, size_x, size_z);
    box->color = color;
}

void set_box_pos(Box* box, double x, double y) {
    object_set_pos(&box->obj, x, y);
}

double get_box_x(Box box) { return box.obj.x; }
double get_box_z(Box box) { return box.obj.z; }
int get_box_size_x(Box box) { return box.obj.hitbox.size_x; }
int get_box_size_z(Box box) { return box.obj.hitbox.size_z; }
int get_box_color(Box box) { return box.color; }

int is_box_followed(Box box) { return box.obj.followed_obj != NULL; }
/* update box */
void box_update(Box* box, double d_sec) { object_update(&box->obj, d_sec); }