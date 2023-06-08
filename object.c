#include "object.h"

extern int field_x, field_y, field_z;
extern double gravity;
extern ObjList* obj_list;

void move_referencing_obj(Object* obj, double x, double y, double z, double vx,
                          double vy, double vz) {
    ObjList* tmp_objs = obj_list;
    for (tmp_objs = obj_list; tmp_objs != NULL; tmp_objs = tmp_objs->next) {
        if (tmp_objs->obj->followed_obj == obj) {
            tmp_objs->obj->x += x;
            tmp_objs->obj->y += y;
            tmp_objs->obj->z += z;
            tmp_objs->obj->vx += vx;
            tmp_objs->obj->vy += vy;
            tmp_objs->obj->vz += vz;
        }
    }
}

void obj_list_add(ObjList** list, Object* obj) {
    ObjList *new_node, *current;
    /* create new node */
    new_node = (ObjList*)malloc(sizeof(ObjList));
    new_node->obj = obj;
    new_node->next = NULL;
    /* if the list is empty, make the new node the head */
    if (*list == NULL) {
        *list = new_node;
        return;
    }
    /* traverse the list to find the last node */
    current = *list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

void free_obj_list(ObjList* list) {
    ObjList* current = list;
    ObjList* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

void object_init(Object* obj, double x, double y, double z, double vx,
                 double vy, double vz, int size_x, int size_y, int size_z) {
    obj->x = x;
    obj->y = y;
    obj->z = z;
    obj->vx = vx;
    obj->vy = vy;
    obj->vz = vz;
    obj->old_x = (int)x;
    obj->old_y = (int)y;
    obj->old_z = (int)z;
    obj->old_vx = vx;
    obj->old_vy = vy;
    obj->old_vz = vz;
    obj->collision_above_flag = 0;
    obj->collision_enable = 1;
    obj->hitbox.size_x = size_x;
    obj->hitbox.size_y = size_y;
    obj->hitbox.size_z = size_z;
    obj->followed_obj = NULL;
}

void object_set_size(Object* obj, int size_x, int size_y, int size_z) {
    obj->hitbox.size_x = size_x;
    obj->hitbox.size_y = size_y;
    obj->hitbox.size_z = size_z;
}

void object_update(Object* main_obj, double d_sec) {
    ObjList* tmp_objs;
    main_obj->old_vx = main_obj->vx;
    main_obj->old_vy = main_obj->vy;
    main_obj->old_vz = main_obj->vz;
    if (main_obj->followed_obj == NULL) {
        main_obj->x += main_obj->vx * d_sec;
        main_obj->y += main_obj->vy * d_sec;
        main_obj->z += main_obj->vz * d_sec;
    } else {
        main_obj->x += (main_obj->vx + main_obj->followed_obj->old_vx) * d_sec;
        main_obj->y += (main_obj->vy + main_obj->followed_obj->old_vy) * d_sec;
        main_obj->z += (main_obj->vz + main_obj->followed_obj->old_vz) * d_sec;
    }
    if (is_collided_z(*main_obj) != 1 && main_obj->collision_above_flag == 0) {
        if ((is_collided_x(*main_obj) != 0 || is_collided_y(*main_obj) != 0) &&
            main_obj->vz < 0) {
            main_obj->vz -= gravity * 0.1 * d_sec;
        } else {
            main_obj->vz -= gravity * d_sec;
        }
    }
    main_obj->vx *= 0.99;
    main_obj->vy *= 0.99;
    main_obj->vz *= 0.99;
    if (main_obj->collision_above_flag == 1) {
        main_obj->vx *= 0.5;
        main_obj->vz *= 0.5;
    }
    if (main_obj->vx < 0.1 && main_obj->vx > -0.1) main_obj->vx = 0;
    if (main_obj->vy < 0.1 && main_obj->vy > -0.1) main_obj->vy = 0;
    if (main_obj->vz < 0.1 && main_obj->vz > -0.1) main_obj->vz = 0;
    for (tmp_objs = obj_list; tmp_objs != NULL; tmp_objs = tmp_objs->next) {
        if (tmp_objs->obj != main_obj && tmp_objs->obj->collision_enable == 1) {
            collision(main_obj, tmp_objs->obj);
        }
    }
}

void collision(Object* obj1, Object* obj2) {
    double ax, ay, az;
    /* wall colide */
    if (obj1->x - obj1->hitbox.size_x / 2.0 < 0) {
        obj1->x = obj1->hitbox.size_x / 2.0;
        obj1->vx = 0;
    } else if (obj1->x + obj1->hitbox.size_x / 2.0 > field_x) {
        obj1->x = field_x - obj1->hitbox.size_x / 2.0;
        obj1->vx = 0;
    }
    if (obj1->y - obj1->hitbox.size_y / 2.0 < 0) {
        obj1->y = obj1->hitbox.size_y / 2.0;
        obj1->vy = 0;
    } else if (obj1->y + obj1->hitbox.size_y / 2.0 > field_y) {
        obj1->y = field_y - obj1->hitbox.size_y / 2.0;
        obj1->vy = 0;
    }
    /* floor colide */
    if (obj1->z - obj1->hitbox.size_z / 2.0 < 0) {
        obj1->z = obj1->hitbox.size_z / 2.0;
        obj1->vz = 0;
    } else if (obj1->z + obj1->hitbox.size_z / 2.0 > field_z) {
        obj1->z = field_z - obj1->hitbox.size_z / 2.0;
        obj1->vz = 0;
    }
    /* object colide */
    if (obj2 == NULL) return;
    obj1->collision_above_flag = check_if_object_above(*obj1, *obj2);

    ax = obj1->x - obj2->x;
    ay = obj1->y - obj2->y;
    az = obj1->z - obj2->z;
    if (ax < 0) ax = -ax;
    if (ay < 0) ay = -ay;
    if (az < 0) az = -az;

    if (ax < obj1->hitbox.size_x / 2.0 + obj2->hitbox.size_x / 2.0 &&
        ay < obj1->hitbox.size_y / 2.0 + obj2->hitbox.size_y / 2.0 &&
        az < obj1->hitbox.size_z / 2.0 + obj2->hitbox.size_z / 2.0) {
        /* x colide */
        /* if not colide wall */
        if (ax < obj1->hitbox.size_x / 2.0 + obj2->hitbox.size_x / 2.0 &&
            obj1->x - obj1->hitbox.size_x / 2.0 > 0 &&
            obj1->x + obj1->hitbox.size_x / 2.0 < field_x) {
            if (obj1->x > obj2->x) {
                obj1->x +=
                    obj1->hitbox.size_x / 2.0 + obj2->hitbox.size_x / 2.0 - ax;
            } else {
                obj1->x -=
                    obj1->hitbox.size_x / 2.0 + obj2->hitbox.size_x / 2.0 - ax;
            }
            obj1->vx = 0;
        } else if (obj1->x - obj1->hitbox.size_x / 2.0 <= 0) {
            obj1->x = obj1->hitbox.size_x / 2.0;
            obj1->vx = 0;
        } else if (obj1->x + obj1->hitbox.size_x / 2.0 >= field_x) {
            obj1->x = field_x - obj1->hitbox.size_x / 2.0;
            obj1->vx = 0;
        }
        /* y colide */
        /* if not colide wall */
        if (ay < obj1->hitbox.size_y / 2.0 + obj2->hitbox.size_y / 2.0 &&
            obj1->y - obj1->hitbox.size_y / 2.0 > 0 &&
            obj1->y + obj1->hitbox.size_y / 2.0 < field_y) {
            if (obj1->y > obj2->y) {
                obj1->y +=
                    obj1->hitbox.size_y / 2.0 + obj2->hitbox.size_y / 2.0 - ay;
            } else {
                obj1->y -=
                    obj1->hitbox.size_y / 2.0 + obj2->hitbox.size_y / 2.0 - ay;
            }
            obj1->vy = 0;
        } else if (obj1->y - obj1->hitbox.size_y / 2.0 <= 0) {
            obj1->y = obj1->hitbox.size_y / 2.0;
            obj1->vy = 0;
        } else if (obj1->y + obj1->hitbox.size_y / 2.0 >= field_y) {
            obj1->y = field_y - obj1->hitbox.size_y / 2.0;
            obj1->vy = 0;
        }
        /* z colide */
        /* if not colide floor */
        if (az < obj1->hitbox.size_z / 2.0 + obj2->hitbox.size_z / 2.0 &&
            obj1->z - obj1->hitbox.size_z / 2.0 > 0 &&
            obj1->z + obj1->hitbox.size_z / 2.0 < field_z) {
            if (obj1->z > obj2->z) {
                obj1->z +=
                    obj1->hitbox.size_z / 2.0 + obj2->hitbox.size_z / 2.0 - az;
            } else {
                obj1->z -=
                    obj1->hitbox.size_z / 2.0 + obj2->hitbox.size_z / 2.0 - az;
            }
            obj1->vz = 0;
        } else if (obj1->z - obj1->hitbox.size_z / 2.0 <= 0) {
            obj1->z = obj1->hitbox.size_z / 2.0;
            obj1->vz = 0;
        } else if (obj1->z + obj1->hitbox.size_z / 2.0 >= field_z) {
            obj1->z = field_z - obj1->hitbox.size_z / 2.0;
            obj1->vz = 0;
        }
    }

    if (obj1->collision_above_flag == 1) {
        obj1->followed_obj = obj2;
    } else {
        if (obj1->followed_obj != NULL) obj1->vx += obj1->followed_obj->old_vx;
        obj1->followed_obj = NULL;
    }
}

int is_collided_x(Object obj1) {
    /* wall colide */
    if (obj1.x - obj1.hitbox.size_x / 2.0 <= 0) {
        return -1;
    } else if (obj1.x + obj1.hitbox.size_x / 2.0 >= field_x) {
        return 1;
    }
    return 0;
}

int is_collided_y(Object obj1) {
    /* wall colide */
    if (obj1.y - obj1.hitbox.size_y / 2.0 <= 0) {
        return -1;
    } else if (obj1.y + obj1.hitbox.size_y / 2.0 >= field_y) {
        return 1;
    }
    return 0;
}

int is_collided_z(Object obj1) {
    /* floor colide */
    if (obj1.z - (double)obj1.hitbox.size_z / 2.0 <= 0.5) {
        return 1;
    } else if (obj1.z + (double)obj1.hitbox.size_z / 2.0 >= field_z - 0.5) {
        return -1;
    }
    return 0;
}

int check_if_object_above(Object obj1, Object obj2) {
    double ax, ay, az;
    /* object colide */
    ax = obj1.x - obj2.x;
    ay = obj1.y - obj2.y;
    az = obj1.z - obj2.z;
    if (ax < 0) ax = -ax;
    if (ay < 0) ay = -ay;
    if (az < 0) az = -az;
    az -= 0.2;
    if (ax < obj1.hitbox.size_x / 2.0 + obj2.hitbox.size_x / 2.0 &&
        ay < obj1.hitbox.size_y / 2.0 + obj2.hitbox.size_y / 2.0 &&
        az < obj1.hitbox.size_z / 2.0 + obj2.hitbox.size_z / 2.0) {
        if (obj1.z > obj2.z) {
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}