#include "../../include/socket/data_socket.h"

struct sockaddr_in address;
char* ip_addr;

void shared_data_init(SharedData* sd) {
    sd->frame = 0;
    sd->player_id = 0;
    sd->break_flag = 0;
    sd->lock_flag = 0;
    sd->pl = (Player){0};
    sd->box = (Box){0};
    sd->key = (KeyFlag){0};
}

void shared_data_copy(SharedData* sd, SharedData* sd2) {
    if (sd2->lock_flag != 1) {
        sd2->lock_flag = 1;
        sd->frame = sd2->frame;
        sd->player_id = sd2->player_id;
        sd->pl = sd2->pl;
        sd->box = sd2->box;
        sd->key = sd2->key;
        sd->break_flag = sd2->break_flag;
        sd2->lock_flag = 0;
    }
}

void make_shared_data(SharedData* sd, Player pl, Box box, int frame,
                      KeyFlag key) {
    if (sd->lock_flag != 1) {
        sd->lock_flag = 1;
        sd->frame = frame;
        sd->pl = pl;
        sd->box = box;
        sd->key = key;
        sd->lock_flag = 0;
    }
}

int take_shared_data(SharedData* sd, Player* pl, Box* box, int* frame,
                     KeyFlag* key) {
    if (sd->lock_flag != 1) {
        sd->lock_flag = 1;
        *frame = sd->frame;
        *pl = sd->pl;
        *box = sd->box;
        *key = sd->key;
        sd->lock_flag = 0;
        return 1;
    }
    return 0;
}

void set_box_in_shared_data(SharedData* sd, Box box) {
    if (sd->lock_flag != 1) {
        sd->lock_flag = 1;
        sd->box = box;
        sd->lock_flag = 0;
    }
}

void get_box_from_shared_data(SharedData* sd, Box* box) {
    if (sd->lock_flag != 1) {
        sd->lock_flag = 1;
        *box = sd->box;
        sd->lock_flag = 0;
    }
}