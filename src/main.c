#include "../include/main.h"

int field_x, field_z;
int screen_width, screen_height;
Player *main_pl = NULL;
PlayerList *pl_list = NULL;
ObjList *obj_list = NULL;
int frame = 0, timeout_cnt = 0;
struct sockaddr_in address;
char *ip_addr;
ArgmentFlag argment_flag = {0};

int main(int argc, char **argv) {
    int break_flg = 0, frame_delta = 0;
    double d_sec = 0, fps_timestamp = 0;
    pid_t pid;
    Player pl, pl2;
    PlayerList *tmp_pl_list = NULL;
    SharedData *post_shared_data;
    SharedData *return_shared_data;
    KeyFlag key_flag = {0}, p2_key_flag = {0};
    SharedData shd_players[2];
    FieldPos field_pos;

    /* ======================argment====================== */
    argment_parse(argc, argv, &argment_flag);

    /* ======================init====================== */
    /* init config */
    config_init();
    /* open socket */
    if (argment_flag.server == 1) {
        host_socket_init();
    } else if (argment_flag.client == 1) {
        guest_socket_init(ip_addr);
    }
    /* init player */
    player_init(&pl, 0, 0, 0, 0, 100);
    player_init(&pl2, 0, 0, 0, 0, 100);
    player_list_add(&pl_list, &pl);
    player_list_add(&pl_list, &pl2);
    player_list_to_obj_list(pl_list, &obj_list);
    main_pl = &pl;
    error_check((post_shared_data =
                     mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED,
                "mmap failed\n");
    error_check((return_shared_data =
                     mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED,
                "mmap failed\n");
    shared_data_init(post_shared_data);
    shared_data_init(return_shared_data);

    /* ======================fork====================== */
    pid = fork();
    if (pid < 0) {
        error("fork failed\n");
    } else if (pid == 0) {
        /* ~~~~~~~~~~~~~~~~~~~child process~~~~~~~~~~~~~~~~~~~ */
        while (post_shared_data->break_flag == 0) {
            if (post_shared_data->lock_flag == 0 &&
                post_shared_data->read_flag == 0) {
                /* lock shared data */
                post_shared_data->lock_flag = 1;
                shared_data_copy(&shd_players[0], post_shared_data);
                post_shared_data->read_flag = 1;
                /* unlock shared data */
                post_shared_data->lock_flag = 0;
            }
            if (return_shared_data->read_flag == 1 &&
                return_shared_data->lock_flag == 0) {
                /* lock shared data */
                return_shared_data->lock_flag = 1;
                shared_data_copy(return_shared_data, &shd_players[1]);
                return_shared_data->read_flag = 0;
                /* unlock shared data */
                return_shared_data->lock_flag = 0;
            }
            if (argment_flag.server == 1) {
                /* send new shared data to client */
                host_socket_send(&shd_players[0]);
                /* read the shared data from client */
                host_socket_recv(&shd_players[1]);
            } else if (argment_flag.client == 1) {
                /* send new shared data to server */
                guest_socket_send(&shd_players[0]);
                /* read the shared data from server */
                guest_socket_recv(&shd_players[1]);
            }
            Sleep((1000.0 / max_fps) / 2.0);
        }
        /* send break flag to opponent */
        if (argment_flag.server == 1) {
            shd_players[0].break_flag = 1;
            host_socket_send(&shd_players[0]);
            /* close socket */
            host_socket_close();
        } else if (argment_flag.client == 1) {
            shd_players[0].break_flag = 1;
            guest_socket_send(&shd_players[0]);
            /* close socket */
            guest_socket_close();
        }
        fprintf(stderr, "child process end\n");
        return 0;
    } else {
        /* ~~~~~~~~~~~~~~~~~~~parent process~~~~~~~~~~~~~~~~~~~ */
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
        init_pair(11, COLOR_BLACK, COLOR_YELLOW);
        init_pair(12, COLOR_BLACK, COLOR_GREEN);
        init_pair(13, COLOR_BLACK, COLOR_BLUE);
        init_pair(14, COLOR_BLACK, COLOR_CYAN);

        init_pair(21, COLOR_BLACK, COLOR_YELLOW);
        init_pair(22, COLOR_BLACK, COLOR_GREEN);
        init_pair(23, COLOR_BLACK, COLOR_RED);
        init_pair(24, COLOR_BLACK, COLOR_MAGENTA);
        /* get field */
        field_x = 200;
        field_z = 30;
        if (argment_flag.client == 1) {
            set_player_x(&pl, field_x - 5.5);
            set_player_x(&pl2, 5.5);
            pl.dir_x = PLAYER_LEFT;
            set_player_color(&pl, 2);
            set_player_color(&pl2, 1);
        } else {
            set_player_x(&pl, 5.5);
            set_player_x(&pl2, field_x - 5.5);
            pl2.dir_x = PLAYER_LEFT;
            set_player_color(&pl, 1);
            set_player_color(&pl2, 2);
        }

        import_from_txt();
        /* get start time */
        get_start_time();

        /* ======================main loop====================== */
        for (frame = 1; break_flg == 0; frame++) {
            /* get field */
            int tmp_screen_width, tmp_screen_height;
            getmaxyx(stdscr, tmp_screen_height, tmp_screen_width);
            tmp_screen_height -= 2;
            tmp_screen_width -= 2;
            if (tmp_screen_width != screen_width ||
                tmp_screen_height != screen_height) {
                clear();
                screen_width = tmp_screen_width;
                screen_height = tmp_screen_height;
            }
            max_offscreen_height = 5;
            /* ======================debug====================== */
            if (argment_flag.debug == 1) {
                fps_timestamp = get_now_time();
            }

            /* ======================Player input====================== */
            getch();
            /* keyboard input monitoring */
            get_key_flag(&key_flag);
            players_erase();
            new_player_positon(&pl, key_flag, d_sec);
            if (key_flag.end == 1) {
                break_flg = 1;
            }
            /* ======================CPU input====================== */
            /* if shared data is not locked and not read */
            if (return_shared_data->read_flag == 0 &&
                return_shared_data->lock_flag == 0) {
                /* lock shared data */
                return_shared_data->lock_flag = 1;
                return_shared_data->read_flag = 1;
                p2_key_flag = return_shared_data->key;
                pl2 = return_shared_data->pl;
                frame_delta = frame - return_shared_data->frame;
                /* unlock shared data */
                return_shared_data->lock_flag = 0;
                /* Predicting the motion of the current frame based on the
                 * information obtained from the received frame */
                for (int i = 1; i < frame_delta; i++) {
                    new_player_positon(&pl2, p2_key_flag, 1.0 / max_fps);
                    player_update(&pl2, 1.0 / max_fps);
                }
            } else {
                new_player_positon(&pl2, p2_key_flag, d_sec);
            }
            if (p2_key_flag.end == 1 || return_shared_data->break_flag == 1) {
                break_flg = 1;
            }

            /* ======================update====================== */
            /* update players */
            for (tmp_pl_list = pl_list; tmp_pl_list != NULL;
                 tmp_pl_list = tmp_pl_list->next) {
                player_update(tmp_pl_list->pl, 1.0 / max_fps);
            }
            /* if shared data is not locked */
            if (post_shared_data->lock_flag == 0) {
                /* lock shared data */
                post_shared_data->lock_flag = 1;
                post_shared_data->read_flag = 0;
                post_shared_data->frame = frame;
                post_shared_data->pl = pl;
                post_shared_data->key = key_flag;
                /* unlock shared data */
                post_shared_data->lock_flag = 0;
            }

            /* ======================termination check====================== */
            /* end after 1000s */
            if (frame >= max_fps * 1000) {
                break_flg = 1;
            }

            /* ======================fps control====================== */
            field_draw(&field_pos);
            players_draw();
            refresh();
            get_end_time();
            /* refresh */
            d_sec = calc_time();
            double sleep_time = (1000.0 / max_fps) * frame - d_sec * 1000.0;
            if (sleep_time < 0) {
                int jump = (int)(-sleep_time / (1000.0 / max_fps));
                timeout_cnt += jump;
                if (jump > 0) {
                    players_erase();
                    for (int i = 0; i < jump; i++) {
                        for (tmp_pl_list = pl_list; tmp_pl_list != NULL;
                             tmp_pl_list = tmp_pl_list->next) {
                            player_update(tmp_pl_list->pl, 1.0 / max_fps);
                        }
                    }
                    frame += jump;
                    field_draw(&field_pos);
                    players_draw();
                }
            } else {
                Sleep(sleep_time);
            }

            /* ======================debug====================== */
            if (argment_flag.debug == 1) {
                int fps = (int)(1.0 / (get_now_time() - fps_timestamp));
                debug_draw(fps, fps_timestamp, d_sec, sleep_time, frame_delta);
            }
        }
        /* ======================end loop====================== */
    }
    post_shared_data->break_flag = 1;
    getch();
    endwin();
    wait(NULL);
    fprintf(stderr, "parent process end\n");
    return EXIT_SUCCESS;
}
