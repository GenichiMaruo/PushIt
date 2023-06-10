/* include guard */
#ifndef MAIN_H
#define MAIN_H

#include <arpa/inet.h>
#include <ncurses.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <windows.h>

#include "./socket/guest_socket.h"
#include "./socket/host_socket.h"
#include "argment.h"
#include "config.h"
#include "error.h"
#include "object.h"
#include "player.h"
#include "version.h"
#include "window_mng.h"

extern int port;
extern int max_clients;
extern int max_fps;
extern double gravity;
extern int max_offscreen_width;
extern int max_offscreen_height;

#endif /* MAIN_H */