#include "../include/window_mng.h"

struct timespec start_time, end_time, end_time2;
extern int field_x, field_z;
extern int screen_width, screen_height;
extern int max_offscreen_width, max_offscreen_height;
extern ArgmentFlag argment_flag;
extern Player *main_pl;
extern PlayerList *pl_list;
extern int max_fps, frame, timeout_cnt;

char *aa[121] = {0};

void init_ncurses() {
    /* init ncurses */
    initscr();
    curs_set(0);
    noecho();
    timeout(0);
    /* init color */
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(5, COLOR_WHITE, COLOR_CYAN);
    init_pair(11, COLOR_BLACK, COLOR_YELLOW);
    init_pair(12, COLOR_BLACK, COLOR_GREEN);
    init_pair(13, COLOR_BLACK, COLOR_BLUE);
    init_pair(14, COLOR_BLACK, COLOR_CYAN);

    init_pair(21, COLOR_BLACK, COLOR_YELLOW);
    init_pair(22, COLOR_BLACK, COLOR_GREEN);
    init_pair(23, COLOR_BLACK, COLOR_RED);
    init_pair(24, COLOR_BLACK, COLOR_MAGENTA);
}

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

void get_screen_size() {
    int tmp_screen_width, tmp_screen_height;
    getmaxyx(stdscr, tmp_screen_height, tmp_screen_width);
    screen_width = tmp_screen_width - 2;
    screen_height = tmp_screen_height - 2;
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
                    if (argment_flag.aa == 1) {
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
        if (argment_flag.debug == 1) {
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
        /* balloon clear */
        if (pl->balloon_start_frame >= 0) {
            balloon_erase(pl->balloon);
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
        /* balloon */
        if (pl->balloon_start_frame >= 0) {
            balloon_draw(pl->balloon);
        }
        pl->obj.draw_old_x = pl->obj.draw_x;
        pl->obj.draw_old_z = pl->obj.draw_z;
        pl->obj.hitbox.old_size_x = pl->obj.hitbox.size_x;
        pl->obj.hitbox.old_size_z = pl->obj.hitbox.size_z;
    }
    attrset(COLOR_PAIR(2));
    /*debug*/
    if (argment_flag.debug == 1) {
        for (current = pl_list; current != NULL; current = current->next) {
            pl = current->pl;
            /* hp */
            mvprintw(
                screen_height - pl->obj.draw_z - pl->obj.hitbox.size_z / 2 - 2,
                pl->obj.draw_x - pl->obj.hitbox.size_x / 2, "HP: %3d", pl->hp);
        }
    }
}

void box_erase(Box box) {
    int x, z;
    attrset(COLOR_PAIR(2));
    for (z = 0; z < box.obj.hitbox.old_size_z; z++) {
        for (x = 0; x < box.obj.hitbox.old_size_x; x++) {
            mvprintw(screen_height - box.obj.draw_old_z - z +
                         box.obj.hitbox.old_size_z / 2,
                     box.obj.draw_old_x + x - box.obj.hitbox.old_size_x / 2 + 1,
                     " ");
        }
    }
}

void box_draw(Box *box) {
    int x, z;
    box->obj.draw_x = get_main_player_screen_x(*main_pl, screen_width, field_x,
                                               max_offscreen_width) -
                      get_player_x(*main_pl) + get_box_x(*box);
    box->obj.draw_z = get_main_player_screen_z(*main_pl, screen_height, field_z,
                                               max_offscreen_height) -
                      get_player_z(*main_pl) + get_box_z(*box);
    /* shadow */
    attrset(COLOR_PAIR(2));
    for (x = (int)(box->obj.z) / 5;
         x < (box->obj.hitbox.size_x - (int)(box->obj.z) / 5); x++) {
        mvprintw(screen_height - box->obj.draw_z + box->obj.z + 1,
                 box->obj.draw_x + x - box->obj.hitbox.size_x / 2 + 1, " ");
    }
    /* box */
    attrset(COLOR_PAIR(3));
    for (z = 0; z < box->obj.hitbox.size_z; z++) {
        for (x = 0; x < box->obj.hitbox.size_x; x++) {
            mvprintw(screen_height - box->obj.draw_z - z +
                         box->obj.hitbox.size_z / 2,
                     box->obj.draw_x + x - box->obj.hitbox.size_x / 2 + 1, "x");
        }
    }
    box->obj.draw_old_x = box->obj.draw_x;
    box->obj.draw_old_z = box->obj.draw_z;
    box->obj.hitbox.old_size_x = box->obj.hitbox.size_x;
    box->obj.hitbox.old_size_z = box->obj.hitbox.size_z;
}

void balloon_erase(Balloon balloon) {
    int x, z;
    attrset(COLOR_PAIR(2));
    for (z = 0; z < balloon.obj.hitbox.old_size_z; z++) {
        for (x = 0; x < balloon.obj.hitbox.old_size_x; x++) {
            if (get_balloon_pixel(x, z) == 0 && get_balloon_aa(x, z) == ' ')
                continue;
            mvprintw(screen_height - balloon.obj.draw_old_z - z +
                         balloon.obj.hitbox.old_size_z / 2,
                     balloon.obj.draw_old_x + x -
                         balloon.obj.hitbox.old_size_x / 2 + 1,
                     " ");
        }
    }
}

void balloon_draw(Balloon balloon) {
    int x, z, color;
    balloon.obj.draw_x =
        get_main_player_screen_x(*main_pl, screen_width, field_x,
                                 max_offscreen_width) -
        get_player_x(*main_pl) + get_balloon_x(balloon);
    balloon.obj.draw_z =
        get_main_player_screen_z(*main_pl, screen_height, field_z,
                                 max_offscreen_height) -
        get_player_z(*main_pl) + get_balloon_z(balloon);
    /* shadow */
    attrset(COLOR_PAIR(2));
    for (x = (int)(balloon.obj.z) / 5;
         x < (balloon.obj.hitbox.size_x - (int)(balloon.obj.z) / 5); x++) {
        mvprintw(screen_height - balloon.obj.draw_z + balloon.obj.z + 1,
                 balloon.obj.draw_x + x - balloon.obj.hitbox.size_x / 2 + 1,
                 " ");
    }
    /* balloon */
    attrset(COLOR_PAIR(4));
    for (z = 0; z < balloon.obj.hitbox.size_z; z++) {
        for (x = 0; x < balloon.obj.hitbox.size_x; x++) {
            color = get_balloon_pixel(x, z);
            if (color == 0 && get_balloon_aa(x, z) == ' ') continue;
            attrset(COLOR_PAIR(color + 10 * get_balloon_color(balloon)));
            mvprintw(screen_height - balloon.obj.draw_z - z +
                         balloon.obj.hitbox.size_z / 2,
                     balloon.obj.draw_x + x - balloon.obj.hitbox.size_x / 2 + 1,
                     "%c", get_balloon_aa(x, z));
        }
    }
    balloon.obj.draw_old_x = balloon.obj.draw_x;
    balloon.obj.draw_old_z = balloon.obj.draw_z;
    balloon.obj.hitbox.old_size_x = balloon.obj.hitbox.size_x;
    balloon.obj.hitbox.old_size_z = balloon.obj.hitbox.size_z;
}

/* debug */
void debug_draw(int fps, double fps_timestamp, double d_sec, double sleep_time,
                int frame_delta, Box box) {
    int i = 0;
    PlayerList *current;
    attrset(COLOR_PAIR(2));
    /* draw players x,z,vx,vz */
    mvprintw(1, 3, "count: %d, %f[s], timeout: %d   ", frame, d_sec,
             timeout_cnt);
    mvprintw(2, 3, "field_x: %d, field_z: %d, max_fps: %d, fps: %d   ", field_x,
             field_z, max_fps, fps);
    for (current = pl_list; current != NULL; current = current->next) {
        mvprintw(
            3 + i, 3,
            "pl_col: %d, x: %.4f, z: %.4f, vx: %.4f, vz: %.4f, follow: %d   ",
            get_player_color(*current->pl), current->pl->obj.x,
            current->pl->obj.z, current->pl->obj.vx, current->pl->obj.vz,
            current->pl->obj.followed_obj != NULL ? 1 : 0);
        i++;
    }
    mvprintw(3 + i, 3, "frame_time:%.4f, d_sec:%.4f, sleep_time:%.4f   ",
             get_now_time() - fps_timestamp, d_sec, sleep_time);
    i++;
    mvprintw(3 + i, 3, "frame_delta:%d   ", frame_delta);
    mvprintw(4 + i, 3, "box: x:%.4f, z:%.4f, vx:%.4f, vz:%.4f   ", box.obj.x,
             box.obj.z, box.obj.vx, box.obj.vz);
    mvprintw(5 + i, 3, "box followed:%d, follow_obj_id:%d, flag:%d   ",
             box.obj.followed_obj != NULL ? 1 : 0,
             box.obj.followed_obj != NULL ? box.obj.followed_obj->id : -1,
             box.obj.collision_side_flag);
}