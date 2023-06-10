#include "../include/data_socket.h"

#include "data_socket.h"

void shared_data_init(SharedData* sd) {
    sd->frame = 0;
    sd->player_id = 0;
    sd->read_flag = 0;
    sd->break_flag = 0;
    sd->lock_flag = 0;
}

void shared_data_copy(SharedData* sd, SharedData* sd2) {
    sd->frame = sd2->frame;
    sd->player_id = sd2->player_id;
    sd->pl = sd2->pl;
    sd->key = sd2->key;
    sd->break_flag = sd2->break_flag;
}