/* include guard */
#ifndef MAIN_H
#define MAIN_H

#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <windows.h>

#include "error.h"
#include "object.h"
#include "player.h"
#include "windowmng.h"

#define PORT 50000
#define MAX_CLIENTS 2

typedef struct SharedData {
    int frame;
    int player_id;
    Player pl;
    KeyFlag key;
    short int read_flag;
    short int break_flag;
    short int lock_flag;
} SharedData;

#endif /* MAIN_H */