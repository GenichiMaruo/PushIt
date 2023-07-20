#include "../../include/object/balloon.h"

char dot_art[5][5] = {{0, 1, 1, 1, 0},
                      {0, 1, 1, 1, 0},
                      {0, 1, 1, 1, 0},
                      {0, 0, 1, 0, 0},
                      {0, 0, 0, 0, 0}};
char dot_aa[5][6] = {"     ", "     ", "     ", "     ", "  |  "};

int balloon_init(Balloon* balloon, double x, double z, int color) {
    object_init(&balloon->obj, 0, x, z, 0, 0, 5, 5);
    balloon->color = color;
    return 0;
}

void set_balloon_pos(Balloon* balloon, double x, double y) {
    object_set_pos(&balloon->obj, x, y);
}
int get_balloon_x(Balloon balloon) { return (int)balloon.obj.x; }
int get_balloon_z(Balloon balloon) { return (int)balloon.obj.z; }
int get_balloon_pixel(int px, int pz) { return dot_art[4 - pz][px]; }
char get_balloon_aa(int px, int pz) { return dot_aa[4 - pz][px]; }

int get_balloon_color(Balloon balloon) { return balloon.color; }