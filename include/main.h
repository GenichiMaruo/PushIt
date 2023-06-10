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
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <windows.h>

#include "error.h"
#include "guest_socket.h"
#include "host_socket.h"
#include "object.h"
#include "player.h"
#include "window_mng.h"

#define MAX_CLIENTS 2

#endif /* MAIN_H */