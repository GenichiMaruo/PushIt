/* include guard */
#ifndef DATA_SOCKET_H
#define DATA_SOCKET_H

#include "player.h"

typedef struct SharedData {
    int frame;
    int player_id;
    Player pl;
    KeyFlag key;
    short int read_flag;
    short int break_flag;
    short int lock_flag;
} SharedData;

#endif /* DATA_SOCKET_H */