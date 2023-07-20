#include "../../include/object/player.h"

extern int frame;
extern int max_fps;

int dot_art_n[5][5] = {{1, 1, 1, 1, 0},
                       {1, 1, 1, 1, 0},
                       {3, 3, 3, 3, 1},
                       {4, 4, 4, 4, 0},
                       {4, 0, 0, 4, 0}};
char dot_aa_n[5][6] = {" ^ ^ ", "  v  ", "     ", "     ", "     "};

int dot_art_1[4][5] = {
    {1, 1, 1, 1, 0}, {1, 1, 1, 1, 0}, {3, 3, 3, 3, 1}, {4, 4, 4, 4, 0}};
char dot_aa_1[4][6] = {" - - ", "  -  ", "     ", "     "};

int dot_art_2[3][5] = {{1, 1, 1, 1, 0}, {1, 1, 1, 1, 0}, {4, 3, 3, 4, 1}};
char dot_aa_2[3][6] = {" > < ", "  ^  ", "     "};

void player_list_add(PlayerList** list, Player* player) {
    /* Create a new node */
    PlayerList* newNode = (PlayerList*)malloc(sizeof(PlayerList));
    newNode->pl = player;
    newNode->next = NULL;
    /* If the list is empty, make the new node the head */
    if (*list == NULL) {
        *list = newNode;
    } else {
        /* Traverse the list to find the last node */
        PlayerList* temp = *list;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        /* Add the new node at the end */
        temp->next = newNode;
    }
}
void player_list_free(PlayerList* list) {
    PlayerList* current = list;
    PlayerList* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}
void player_list_to_obj_list(PlayerList* pl_list, ObjList** obj_list) {
    PlayerList* current = pl_list;
    while (current != NULL) {
        obj_list_add(obj_list, &current->pl->obj);
        current = current->next;
    }
}

void set_player_color(Player* pl, int color) {
    pl->color = color;
    pl->obj.id = color;
    pl->balloon.color = color;
}
void set_player_x(Player* pl, double x) {
    pl->obj.x = x;
    pl->obj.old_x = x;
}
void set_player_z(Player* pl, double z) {
    pl->obj.z = z;
    pl->obj.old_z = z;
}

/* get key flag */
void get_key_flag(KeyFlag* key) {
    if (GetKeyState('A') & 0x8000) {
        key->left = 1;
        key->right = 0;
    } else if (GetKeyState('D') & 0x8000) {
        key->right = 1;
        key->left = 0;
    } else {
        key->left = 0;
        key->right = 0;
    }
    if (GetKeyState('W') & 0x8000) {
        key->up = 1;
        key->down = 0;
    } else if (GetKeyState('S') & 0x8000) {
        key->down = 1;
        key->up = 0;
    } else {
        key->up = 0;
        key->down = 0;
    }
    if (GetKeyState(VK_SPACE) & 0x8000) {
        key->jump = 1;
        key->squat = 0;
    } else if (GetKeyState('Z') & 0x8000) {
        key->squat = 1;
        key->jump = 0;
    } else {
        key->jump = 0;
        key->squat = 0;
    }
    if (GetKeyState(VK_SHIFT) & 0x8000) {
        key->sprint = 1;
    } else {
        key->sprint = 0;
    }
    if (GetKeyState('Q') & 0x8000) {
        key->end = 1;
    } else {
        key->end = 0;
    }
    if (GetKeyState('E') & 0x8000) {
        key->balloon = 1;
    } else {
        key->balloon = 0;
    }
}
int get_player_color(Player pl) { return pl.color; }
int get_player_x(Player pl) { return (int)pl.obj.x; }
int get_player_z(Player pl) { return (int)pl.obj.z; }
int get_player_pixel(Player pl, int px, int pz) {
    if (pl.jump_level == 1) {
        if (pl.dir_x == PLAYER_RIGHT) {
            return dot_art_1[3 - pz][px];
        } else {
            return dot_art_1[3 - pz][4 - px];
        }
    } else if (pl.jump_level == 2) {
        if (pl.dir_x == PLAYER_RIGHT) {
            return dot_art_2[2 - pz][px];
        } else {
            return dot_art_2[2 - pz][4 - px];
        }
    } else {
        if (pl.dir_x == PLAYER_RIGHT) {
            return dot_art_n[4 - pz][px];
        } else {
            return dot_art_n[4 - pz][4 - px];
        }
    }
}
char get_player_aa(Player pl, int px, int pz) {
    if (pl.jump_level == 1) {
        return dot_aa_1[3 - pz][px];
    } else if (pl.jump_level == 2) {
        return dot_aa_2[2 - pz][px];
    } else {
        return dot_aa_n[4 - pz][px];
    }
}

void player_init(Player* pl, int id, double x, double z, double vx, double vz,
                 int hp) {
    object_init(&pl->obj, id, x, z, vx, vz, 5, 5);
    pl->hp = hp;
    pl->act = 0;
    pl->act_cnt = 0;
    pl->dir_x = PLAYER_RIGHT;
    pl->jump_level = 0;
    pl->jump_start_frame = 0;
    pl->jump_cnt = 0;
    pl->jump_key_flg = 0;
    pl->color = 0;
    balloon_init(&pl->balloon, x, z + 5, id);
}
void player_squat(Player* pl, int level) {
    if (pl->jump_level != level) {
        pl->obj.z -= pl->obj.hitbox.size_z / 2.0;
        if (level == 1) {
            object_set_size(&pl->obj, 5, 4);
        } else if (level == 2) {
            object_set_size(&pl->obj, 5, 3);
        } else {
            object_set_size(&pl->obj, 5, 5);
        }
        pl->obj.z += pl->obj.hitbox.size_z / 2.0;
    }
    pl->jump_level = level;
}
void player_left(Player* pl, double speed) {
    pl->obj.vx = -speed;
    pl->dir_x = PLAYER_LEFT;
}
void player_right(Player* pl, double speed) {
    pl->obj.vx = speed;
    pl->dir_x = PLAYER_RIGHT;
}
void player_jump(Player* pl, double power) {
    if (is_collided_z(pl->obj) == 1 || pl->obj.collision_above_flag == 1) {
        pl->jump_cnt = 0;
        pl->obj.vz = power;
    } else if (is_collided_z(pl->obj) != 1 && (is_collided_x(pl->obj) != 0) &&
               pl->jump_cnt < 1) {
        pl->obj.vz = 55;
        if (is_collided_x(pl->obj) == 1) {
            pl->obj.vx = -100;
            pl->dir_x = PLAYER_LEFT;
        } else if (is_collided_x(pl->obj) == -1) {
            pl->obj.vx = 100;
            pl->dir_x = PLAYER_RIGHT;
        }
        pl->jump_cnt++;
    }
    if (pl->jump_level != 0) {
        player_squat(pl, 0);
    }
}
void player_update(Player* pl, double d_sec) {
    object_update(&pl->obj, d_sec);
    if (pl->balloon_start_frame >= 0) {
        if (frame - pl->balloon_start_frame > 5 * max_fps) {
            pl->balloon_start_frame = -1;
        }
    }
    pl->balloon.obj.x = pl->obj.x;
    pl->balloon.obj.z = pl->obj.z + 5;
    if (is_collided_z(pl->obj) == 1) {
        pl->obj.vx *= 0.8;
    }
    if (pl->obj.vz != 0) {
        pl->act = 1;
    } else {
        pl->act = 0;
    }
    if (pl->act == 1) {
        pl->act_cnt++;
    } else {
        pl->act_cnt = 0;
    }
}

void new_player_positon(Player* pl, KeyFlag key, double d_sec) {
    /* --------------jump,squat key-------------- */
    if (key.jump == 1) {
        if (pl->jump_key_flg == 0) {
            pl->jump_key_flg = 1;
            pl->jump_timestamp = d_sec;
        } else {
            if (d_sec - pl->jump_timestamp > 0.2 &&
                d_sec - pl->jump_timestamp < 0.6) {
                player_squat(pl, 1);
            } else if (d_sec - pl->jump_timestamp > 0.6) {
                player_squat(pl, 2);
            }
        }
    } else if (pl->jump_key_flg == 1) {
        pl->jump_key_flg = 0;
        if (d_sec - pl->jump_timestamp < 0.1) {
            player_jump(pl, 40);
        } else if (d_sec - pl->jump_timestamp < 0.2) {
            player_jump(pl, 55);
        } else if (d_sec - pl->jump_timestamp < 0.4) {
            player_jump(pl, 70);
        } else if (d_sec - pl->jump_timestamp < 0.6) {
            player_jump(pl, 85);
        } else if (d_sec - pl->jump_timestamp < 0.8) {
            player_jump(pl, 100);
        } else {
            player_jump(pl, 115);
        }
    } else if (key.squat == 1) {
        player_squat(pl, 1);
    } else {
        player_squat(pl, 0);
    }
    /* --------------a,d key-------------- */
    if (key.left == 1) {
        if (key.jump == 1 && d_sec - pl->jump_timestamp > 0.2) {
            player_left(pl, 20);
        } else if (key.squat == 1) {
            player_left(pl, 20);
        } else {
            if (key.sprint == 1) {
                player_left(pl, 60);
            } else {
                player_left(pl, 40);
            }
        }
    } else if (key.right == 1) {
        if (key.jump == 1 && d_sec - pl->jump_timestamp > 0.2) {
            player_right(pl, 20);
        } else if (key.squat == 1) {
            player_right(pl, 20);
        } else {
            if (key.sprint == 1) {
                player_right(pl, 60);
            } else {
                player_right(pl, 40);
            }
        }
    }
}