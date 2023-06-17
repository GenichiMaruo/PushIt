/* include guard */
#ifndef GAME_RESULT_H
#define GAME_RESULT_H

#include "../object/_object_group.h"
#include "../socket/_socket_group.h"
#include "external_libs.h"

int is_game_end_box(Box box, int field_x, int color);
int is_game_end_time(int frame, int max_fps);

void show_result(int result);

#endif /* GAME_RESULT_H */