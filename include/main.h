/* include guard */
#ifndef MAIN_H
#define MAIN_H

#include "./common/_common_group.h"
#include "./object/_object_group.h"
#include "./socket/_socket_group.h"
#include "argment.h"
#include "game.h"
#include "manu.h"
#include "window_mng.h"

extern int port;
extern int max_clients;
extern int max_fps;
extern double gravity;
extern int max_offscreen_width;
extern int max_offscreen_height;

#endif /* MAIN_H */