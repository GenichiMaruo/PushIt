/* include guard */
#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>

#include "object.h"
typedef struct Player {
    Object obj;
    int hp;
    int act;
    int act_cnt;
    int dir_x;
    int dir_y;
    int jump_level;
    int jump_start_frame;
    double jump_timestamp;
    int jump_cnt;
    short int jump_key_flg;
    int color;
} Player;

typedef struct PlayerList {
    Player *pl;
    struct PlayerList *next;
} PlayerList;

typedef struct KeyFlag {
    short int left;
    short int right;
    short int up;
    short int down;
    short int jump;
    short int squat;
    short int sprint;
    short int end;
} KeyFlag;

typedef struct FieldPos {
    int x;
    int y;
    int z;
    int old_x;
    int old_y;
    int old_z;
} FieldPos;

enum { PLAYER_LEFT, PLAYER_RIGHT, PLAYER_UP, PLAYER_DOWN };

void player_list_add(PlayerList **list, Player *pl);
void player_list_sort_y(PlayerList *list);
void player_list_free(PlayerList *list);
void player_list_to_obj_list(PlayerList *pl_list, ObjList **obj_list);

void set_player_color(Player *pl, int color);
void set_player_x(Player *pl, double x);
void set_player_y(Player *pl, double y);
void set_player_z(Player *pl, double z);

void get_key_flag(KeyFlag *key);
int get_player_color(Player pl);
int get_player_x(Player pl);
int get_player_y(Player pl);
int get_player_z(Player pl);
int get_player_pixel(Player pl, int px, int py);
char get_player_aa(Player pl, int px, int py);

void player_init(Player *pl, double x, double y, double z, double vx, double vy,
                 double vz, int hp);
void player_squat(Player *pl, int level);
void player_left(Player *pl, double speed);
void player_right(Player *pl, double speed);
void player_up(Player *pl, double speed);
void player_down(Player *pl, double speed);
void player_jump(Player *pl, double power);
void player_update(Player *pl, double d_sec);

void new_player_positon(Player *pl, KeyFlag key, double d_sec);
#endif /* PLAYER_H */