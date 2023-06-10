/* include guard */
#ifndef WINDOWMNG_H
#define WINDOWMNG_H

#define ART_FOLDER_PATH "art/"

#include "./common/_common_group.h"
#include "./object/_object_group.h"
#include "argment.h"

void get_start_time();
void get_end_time();
double calc_time();
double get_now_time();

int get_main_player_screen_x(Player pl, int screen_width, int field_width,
                             int max_offscreen_width);
int get_main_player_screen_z(Player pl, int screen_height, int field_height,
                             int max_offscreen_height);

int get_field_screen_x(Player pl, int screen_width, int field_width,
                       int max_offscreen_width);
int get_field_screen_z(Player pl, int screen_height, int field_height,
                       int max_offscreen_height);

char get_aa(int x, int z);
void import_from_txt();

void field_draw(FieldPos *field_pos);
void players_erase();
void players_draw();

void debug_draw(int fps, double fps_timestamp, double d_sec, double sleep_time,
                int frame_delta);

#endif /* WINDOWMNG_H */