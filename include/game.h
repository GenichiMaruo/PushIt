/* include guard */
#ifndef GAME_H
#define GAME_H

#include "main.h"

extern int field_x, field_z, screen_width, screen_height;
extern int frame, timeout_cnt;
extern Player *main_pl;
extern PlayerList *pl_list;
extern ObjList *obj_list;
extern struct ArgmentFlag argment_flag;

int pushit_game_main(void);

int menu_input_ip(void);

#endif  // GAME_H