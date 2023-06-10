/* include guard */
#ifndef MAIN_H
#define MAIN_H

#include "./object/_object_group.h"
#include "./socket/_socket_group.h"
#include "argment.h"
#include "config.h"
#include "error.h"
#include "external_libs.h"
#include "version.h"
#include "window_mng.h"

extern int port;
extern int max_clients;
extern int max_fps;
extern double gravity;
extern int max_offscreen_width;
extern int max_offscreen_height;

#endif /* MAIN_H */