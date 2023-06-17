/* include guard */
#ifndef OBJECT_H
#define OBJECT_H

#include "../common/external_libs.h"
typedef struct Hitbox {
    int size_x;
    int size_z;
    int old_size_x;
    int old_size_z;
} Hitbox;
typedef struct Object {
    int id;
    double x;
    double z;
    double vx;
    double vz;
    int draw_x;
    int draw_z;
    int draw_old_x;
    int draw_old_z;
    int old_x;
    int old_z;
    double old_vx;
    double old_vz;
    int collision_above_flag;
    int collision_side_flag;
    int collision_enable;
    Hitbox hitbox;
    struct Object* followed_obj;
} Object;

typedef struct ObjList {
    Object* obj;
    struct ObjList* next;
} ObjList;

void obj_list_add(ObjList** list, Object* obj);
void free_obj_list(ObjList* list);

void object_init(Object* obj, int id, double x, double z, double vx, double vz,
                 int size_x, int size_z);
void object_set_size(Object* obj, int size_x, int size_z);
void object_set_pos(Object* obj, double x, double z);

void object_update(Object* main_obj, double d_sec);

void collision(Object* obj1, Object* obj2);

int is_collided_x(Object obj1);
int is_collided_z(Object obj1);

int check_if_object_above(Object obj1, Object obj2);
int check_if_object_side(Object obj1, Object obj2);

#endif /* OBJECT_H */