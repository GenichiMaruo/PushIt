/* include guard */
#ifndef DATA_SOCKET_H
#define DATA_SOCKET_H

#include "../object/_object_group.h"

typedef struct SharedData {
    int frame;
    int player_id;
    Player pl;
    int pl_followed_obj_id;
    Box box;
    int box_followed_obj_id;
    KeyFlag key;
    short int break_flag;
    short int lock_flag;
} SharedData;

void shared_data_init(SharedData *sd);
void shared_data_copy(SharedData *sd, SharedData *sd2);

void make_shared_data(SharedData *sd, Player pl, Box box, int frame,
                      KeyFlag key);
/* return 1 if data is taken */
int take_shared_data(SharedData *sd, Player *pl, Box *box, int *frame,
                     KeyFlag *key);

void set_box_in_shared_data(SharedData *sd, Box box);

void get_box_from_shared_data(SharedData *sd, Box *box);

#endif /* DATA_SOCKET_H */