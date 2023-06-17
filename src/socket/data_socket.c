#include "../../include/socket/data_socket.h"

struct sockaddr_in address;
char* ip_addr;

void shared_data_init(SharedData* sd) {
    sd->frame = 0;
    sd->player_id = 0;
    sd->winner = 0;
    sd->break_flag = 0;
    sd->lock_flag = 0;
    sd->pl = (Player){0};
    sd->pl_followed_obj_id = -1;
    sd->box = (Box){0};
    sd->box_followed_obj_id = -1;
    sd->key = (KeyFlag){0};
}

void shared_data_copy(SharedData* sd, SharedData* sd2) {
    if (sd2->lock_flag != 1) {
        sd2->lock_flag = 1;
        sd->frame = sd2->frame;
        sd->player_id = sd2->player_id;
        sd->pl = sd2->pl;
        sd->pl_followed_obj_id = sd2->pl_followed_obj_id;
        sd->box = sd2->box;
        sd->box_followed_obj_id = sd2->box_followed_obj_id;
        sd->key = sd2->key;
        sd->winner = sd2->winner;
        sd->break_flag = sd2->break_flag;
        sd2->lock_flag = 0;
    }
}

void make_shared_data(SharedData* sd, Player pl, Box box, int frame,
                      KeyFlag key, int winner, int break_flag) {
    if (sd->lock_flag != 1) {
        sd->lock_flag = 1;
        sd->frame = frame;
        sd->pl = pl;
        sd->pl_followed_obj_id =
            pl.obj.followed_obj == NULL ? -1 : pl.obj.followed_obj->id;
        sd->box = box;
        sd->box_followed_obj_id =
            box.obj.followed_obj == NULL ? -1 : box.obj.followed_obj->id;
        sd->key = key;
        sd->winner = winner;
        sd->break_flag = break_flag;
        sd->lock_flag = 0;
    }
}

int take_shared_data(SharedData* sd, Player* pl, Box* box, int* frame,
                     KeyFlag* key, int* winner, int* break_flag) {
    if (sd->lock_flag != 1) {
        sd->lock_flag = 1;
        *frame = sd->frame;
        *pl = sd->pl;
        pl->obj.followed_obj = sd->pl_followed_obj_id == -1
                                   ? NULL
                                   : get_obj_by_id(sd->pl_followed_obj_id);
        *box = sd->box;
        box->obj.followed_obj = sd->box_followed_obj_id == -1
                                    ? NULL
                                    : get_obj_by_id(sd->box_followed_obj_id);
        *key = sd->key;
        *winner = sd->winner;
        *break_flag = sd->break_flag;
        sd->lock_flag = 0;
        return 1;
    }
    return 0;
}

void set_box_in_shared_data(SharedData* sd, Box box) {
    if (sd->lock_flag != 1) {
        sd->lock_flag = 1;
        sd->box = box;
        sd->box_followed_obj_id =
            box.obj.followed_obj == NULL ? -1 : box.obj.followed_obj->id;
        sd->lock_flag = 0;
    }
}

void get_box_from_shared_data(SharedData* sd, Box* box) {
    if (sd->lock_flag != 1) {
        sd->lock_flag = 1;
        *box = sd->box;
        box->obj.followed_obj = sd->box_followed_obj_id == -1
                                    ? NULL
                                    : get_obj_by_id(sd->box_followed_obj_id);
        sd->lock_flag = 0;
    }
}