#include "../include/windowmng.h"

struct timespec start_time, end_time, end_time2;
extern int field_x, field_z;
extern int screen_width, screen_height;
extern int max_offscreen_width, max_offscreen_height;
extern short int aa_flg, debug_flg;
extern Player *main_pl;
extern PlayerList *pl_list;
extern int max_fps, frame, timeout_cnt;

char *aa[121] = {0};

void import_from_txt() {
    FILE *fp;
    char *fname = "aa.txt";
    char buf[256];
    int i = 0;

    /* add file path to fname */
    char *path = (char *)malloc(sizeof(char) * 256);
    strcpy(path, ART_FOLDER_PATH);
    strcat(path, fname);
    fname = path;

    error_check((fp = fopen(fname, "r")) == NULL, "fopen");
    while (fgets(buf, 256, fp) != NULL) {
        aa[i] = (char *)malloc(sizeof(char) * 256);
        strcpy(aa[i], buf);
        i++;
    }
    fclose(fp);
}

/* get start time */
void get_start_time() { clock_gettime(CLOCK_REALTIME, &start_time); }

/* get end time */
void get_end_time() { clock_gettime(CLOCK_REALTIME, &end_time); }

/* get now time */
double get_now_time() {
    struct timespec now_time;
    clock_gettime(CLOCK_REALTIME, &now_time);
    return now_time.tv_sec + (double)now_time.tv_nsec / 1000000000;
}

/* calc time */
double calc_time() {
    unsigned int sec;
    int nsec;
    double d_sec;
    sec = end_time.tv_sec - start_time.tv_sec;
    nsec = end_time.tv_nsec - start_time.tv_nsec;
    d_sec = sec + (double)nsec / 1000000000;
    return d_sec;
}

/* get main player screen position x */
int get_main_player_screen_x(Player pl, int screen_width, int field_width,
                             int max_offscreen_width) {
    int x = get_player_x(pl);
    if (field_x > screen_width) {
        if (screen_width / 2.0 > max_offscreen_width + x) {
            return max_offscreen_width + x;
        } else if (screen_width / 2.0 > max_offscreen_width + field_width - x) {
            return screen_width - max_offscreen_width - field_width + x;
        } else {
            return screen_width / 2.0;
        }
    } else {
        return x + screen_width / 2.0 - field_width / 2.0;
    }
}

/* get main player screen position z */
int get_main_player_screen_z(Player pl, int screen_height, int field_height,
                             int max_offscreen_height) {
    int z = get_player_z(pl);
    if (field_z > screen_height) {
        if (screen_height / 2.0 > max_offscreen_height + z) {
            return max_offscreen_height + z;
        } else if (screen_height / 2.0 >
                   max_offscreen_height + field_height - z) {
            return screen_height - max_offscreen_height - field_height + z;
        } else {
            return screen_height / 2.0;
        }
    } else {
        return z + screen_height / 2.0 - field_height / 2.0;
    }
}

/* get field screen position x */
int get_field_screen_x(Player pl, int screen_width, int field_width,
                       int max_offscreen_width) {
    return get_main_player_screen_x(pl, screen_width, field_width,
                                    max_offscreen_width) -
           get_player_x(pl) + field_width / 2.0;
}

/* get field screen position z */
int get_field_screen_z(Player pl, int screen_height, int field_height,
                       int max_offscreen_height) {
    return get_main_player_screen_z(pl, screen_height, field_height,
                                    max_offscreen_height) -
           get_player_z(pl) + field_height / 2.0;
}

char get_aa(int x, int z) {
    if (x < 0 || x >= 120 || z < 0 || z >= 35) {
        return ' ';
    }
    return aa[35 - z][x];
}

char field_aa(int x, int z) {
    int i, a, b;
    srand(z * x + z);
    a = (rand() % 5) + 10;
    b = (rand() % 5) + 5;
    for (i = 0; i < b; i++) {
        rand();
    }
    if (rand() % a == 0) {
        return 'w';
    }
    return ' ';
}

void field_draw(FieldPos *field_pos) {
    int point_x, point_z, cnt = 0;
    field_pos->x = get_field_screen_x(*main_pl, screen_width, field_x,
                                      max_offscreen_width);
    field_pos->z = get_field_screen_z(*main_pl, screen_height, field_z,
                                      max_offscreen_height);
    attrset(COLOR_PAIR(2));
    for (cnt = 0; cnt < 2; cnt++) {
        for (point_z = 0; point_z < field_z + 2; point_z++) {
            for (point_x = 0; point_x < field_x + 2; point_x++) {
                if (point_z > 0 && point_z < field_z + 1 && point_x > 0 &&
                    point_x < field_x + 1) {
                    if (aa_flg == 1) {
                        mvprintw(
                            screen_height - field_pos->old_z - point_z +
                                (field_z + 2) / 2,
                            field_pos->old_x + point_x - (field_x + 2) / 2 + 1,
                            "%c", get_aa(point_x, point_z - 1));
                    }
                    continue;
                }
                mvprintw(screen_height - field_pos->old_z - point_z +
                             (field_z + 2) / 2,
                         field_pos->old_x + point_x - (field_x + 2) / 2 + 1,
                         " ");
            }
        }
        field_pos->old_x = field_pos->x;
        field_pos->old_z = field_pos->z;
        attrset(COLOR_PAIR(1));
    }
    attrset(COLOR_PAIR(2));
}

/* erase the player */
void players_erase() {
    int x, z;
    Player *pl;
    PlayerList *current;
    attrset(COLOR_PAIR(2));
    for (current = pl_list; current != NULL; current = current->next) {
        pl = current->pl;
        /*debug*/
        if (debug_flg == 1) {
            /* hp clear */
            mvprintw(screen_height - pl->obj.draw_old_z -
                         pl->obj.hitbox.old_size_z / 2 - 2,
                     pl->obj.draw_old_x - pl->obj.hitbox.old_size_x / 2,
                     "       ");
        }
        /* player clear */
        for (z = 0; z < pl->obj.hitbox.old_size_z; z++) {
            for (x = 0; x < pl->obj.hitbox.old_size_x; x++) {
                if (get_player_pixel(*pl, x, z) == 0 &&
                    get_player_aa(*pl, x, z) == ' ')
                    continue;
                mvprintw(
                    screen_height - pl->obj.draw_old_z - z +
                        pl->obj.hitbox.old_size_z / 2,
                    pl->obj.draw_old_x + x - pl->obj.hitbox.old_size_x / 2 + 1,
                    " ");
            }
        }
    }
}

/* draw the player */
void players_draw() {
    int x, z, color;
    Player *pl;
    PlayerList *current;
    /* draw player */
    for (current = pl_list; current != NULL; current = current->next) {
        pl = current->pl;
        pl->obj.draw_x =
            get_main_player_screen_x(*main_pl, screen_width, field_x,
                                     max_offscreen_width) -
            get_player_x(*main_pl) + get_player_x(*pl);
        pl->obj.draw_z =
            get_main_player_screen_z(*main_pl, screen_height, field_z,
                                     max_offscreen_height) -
            get_player_z(*main_pl) + get_player_z(*pl);
        /* shadow */
        attrset(COLOR_PAIR(2));
        for (x = (int)(pl->obj.z) / 5;
             x < (pl->obj.hitbox.size_x - (int)(pl->obj.z) / 5); x++) {
            mvprintw(screen_height - pl->obj.draw_z + pl->obj.z + 1,
                     pl->obj.draw_x + x - pl->obj.hitbox.size_x / 2 + 1, " ");
        }
        /* player */
        for (z = 0; z < pl->obj.hitbox.size_z; z++) {
            for (x = 0; x < pl->obj.hitbox.size_x; x++) {
                color = get_player_pixel(*pl, x, z);
                if (color == 0 && get_player_aa(*pl, x, z) == ' ') continue;
                attrset(COLOR_PAIR(color + 10 * get_player_color(*pl)));
                mvprintw(screen_height - pl->obj.draw_z - z +
                             pl->obj.hitbox.size_z / 2,
                         pl->obj.draw_x + x - pl->obj.hitbox.size_x / 2 + 1,
                         "%c", get_player_aa(*pl, x, z));
            }
        }
        pl->obj.draw_old_x = pl->obj.draw_x;
        pl->obj.draw_old_z = pl->obj.draw_z;
        pl->obj.hitbox.old_size_x = pl->obj.hitbox.size_x;
        pl->obj.hitbox.old_size_z = pl->obj.hitbox.size_z;
    }
    attrset(COLOR_PAIR(2));
    /*debug*/
    if (debug_flg == 1) {
        for (current = pl_list; current != NULL; current = current->next) {
            pl = current->pl;
            /* hp */
            mvprintw(
                screen_height - pl->obj.draw_z - pl->obj.hitbox.size_z / 2 - 2,
                pl->obj.draw_x - pl->obj.hitbox.size_x / 2, "HP: %3d", pl->hp);
        }
    }
}

/* debug */
void debug_draw(int fps, double fps_timestamp, double d_sec, double sleep_time,
                int frame_delta) {
    int i = 0;
    PlayerList *current;
    attrset(COLOR_PAIR(2));
    /* draw players x,z,vx,vz */
    mvprintw(1, 3, "count: %d, %f[s], timeout: %d   ", frame, d_sec,
             timeout_cnt);
    mvprintw(2, 3, "field_x: %d, field_z: %d, max_fps: %d, fps: %d   ", field_x,
             field_z, max_fps, fps);
    for (current = pl_list; current != NULL; current = current->next) {
        mvprintw(3 + i, 3, "pl_col: %d, x: %f, z: %f, vx: %f, vz: %f",
                 current->pl->color, current->pl->obj.x, current->pl->obj.z,
                 current->pl->obj.vx, current->pl->obj.vz);
        i++;
    }
    mvprintw(3 + i, 3, "frame_time:%f, d_sec:%f, sleep_time:%f   ",
             get_now_time() - fps_timestamp, d_sec, sleep_time);
    i++;
    mvprintw(3 + i, 3, "frame_delta:%d   ", frame_delta);
}