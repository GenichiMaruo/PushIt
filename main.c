#include "main.h"

int field_x, field_y, field_z;
int screen_width, screen_height;
int max_offscreen_width = 2, max_offscreen_height = 4;
double gravity = 300;
int y_angle = 2;
static char version[10] = "0.0.2";
Player *main_pl = NULL;
PlayerList *pl_list = NULL;
ObjList *obj_list = NULL;
short int debug_flg = 0, aa_flg = 0;
int max_fps = 60, frame = 0, timeout_cnt = 0;

int main(int argc, char **argv) {
    int i, jump, break_flg = 0;
    double d_sec = 0, sleep_time;
    double fps_timestamp = 0;
    int fps;
    int tmp_screen_width, tmp_screen_height;
    Player pl, pl2;
    PlayerList *tmp_pl_list = NULL;
    pid_t pid;
    SharedData *post_shared_data;
    SharedData *return_shared_data;
    KeyFlag key_flag = {0}, p2_key_flag = {0};
    SharedData shd_players[2];
    FieldPos field_pos;

    /* server */
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    short int server_open_flag = 0;
    short int client_open_flag = 0;
    char opponent_version[10];
    char *ip_addr;
    int frame_delta = 0;

    /* ======================argment====================== */
    if (argc >= 2) {
        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                fprintf(stderr, "usage: %s [options]\n", argv[0]);
                fprintf(stderr, "options:\n");
                fprintf(stderr,
                        "  -h, --help     show this help message and exit\n");
                fprintf(stderr,
                        "  -v, --version  show program's version number and "
                        "exit\n");
                fprintf(stderr, "  -d, --debug    debug mode\n");
                fprintf(stderr, "  -f, --fps      set max fps\n");
                fprintf(stderr, "  --server       server mode\n");
                fprintf(stderr, "  --client       client mode\n");
                fprintf(stderr, "  --aa           enable ascii art\n");
                return 0;
            } else if (strcmp(argv[i], "-v") == 0 ||
                       strcmp(argv[i], "--version") == 0) {
                fprintf(stderr, "version: %s\n", version);
                return 0;
            } else if (strcmp(argv[i], "-d") == 0 ||
                       strcmp(argv[i], "--debug") == 0) {
                debug_flg = 1;
            } else if (strcmp(argv[i], "-f") == 0 ||
                       strcmp(argv[i], "--fps") == 0) {
                if (i + 1 < argc) {
                    max_fps = atoi(argv[i + 1]);
                    if (max_fps <= 0) {
                        error("fps must be positive integer\n");
                    }
                    i++;
                } else {
                    error("fps is not specified\n");
                }
            } else if (strcmp(argv[i], "--server") == 0 &&
                       client_open_flag == 0 && server_open_flag == 0) {
                server_open_flag = 1;
                /* ======================server====================== */
                /* Creating socket file descriptor */
                error_check((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0,
                            "socket failed\n");
                address.sin_family = AF_INET;
                address.sin_addr.s_addr = INADDR_ANY;
                address.sin_port = htons(PORT);
                /* bind the socket to localhost port 50000 */
                error_check(bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address)) < 0,
                            "bind failed\n");
                fprintf(stderr, "waiting for connection...\n");
                /* try to specify maximum of 3 pending connections for the
                 * master socket */
                error_check(listen(server_fd, 3) < 0, "listen\n");
                /* accept the incoming connection */
                error_check(
                    (new_socket = accept(server_fd, (struct sockaddr *)&address,
                                         (socklen_t *)&addrlen)) < 0,
                    "accept\n");
                /* read client version */
                error_check(read(new_socket, &opponent_version,
                                 sizeof(opponent_version)) < 0,
                            "read\n");
                /* send server version to client */
                error_check(send(new_socket, &version, sizeof(version), 0) < 0,
                            "send\n");
                /* check version */
                error_check(strcmp(opponent_version, version) != 0,
                            "version mismatch\n");

            } else if (strcmp(argv[i], "--client") == 0 &&
                       client_open_flag == 0 && server_open_flag == 0) {
                client_open_flag = 1;
                /* ======================client====================== */
                /* get ip address from argment */
                ip_addr = argv[i + 1];
                i++;

                /* Check if the input is "localhost" and replace it with the
                 * corresponding IP address */
                if (strcmp(ip_addr, "localhost") == 0) {
                    ip_addr = "127.0.0.1";
                }
                printf("ip_addr: %s\n", ip_addr);
                /* Creating socket file descriptor */
                error_check((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0,
                            "socket failed\n");
                memset(&address, '0', sizeof(address));
                address.sin_family = AF_INET;
                address.sin_port = htons(PORT);
                /* Convert IPv4 and IPv6 addresses from text to binary form */
                error_check(inet_pton(AF_INET, ip_addr, &address.sin_addr) <= 0,
                            "Invalid address/ Address not supported \n");
                /* connect the socket to the server address */
                error_check(connect(server_fd, (struct sockaddr *)&address,
                                    sizeof(address)) < 0,
                            "Connection Failed \n");
                /* send client version to server */
                error_check(send(server_fd, &version, sizeof(version), 0) < 0,
                            "send\n");
                /* read server version */
                error_check(read(server_fd, &opponent_version,
                                 sizeof(opponent_version)) < 0,
                            "read\n");
                /* check version */
                error_check(strcmp(opponent_version, version) != 0,
                            "version mismatch\n");
            } else if (strcmp(argv[i], "--aa") == 0) {
                aa_flg = 1;
            } else {
                fprintf(stderr, "invalid argment: %s\n", argv[i]);
                return 0;
            }
        }
    }

    /* ======================init====================== */
    /* init player */
    player_init(&pl, 0, 0, 0, 0, 0, 0, 100);
    player_init(&pl2, 0, 0, 0, 0, 0, 0, 100);
    player_list_add(&pl_list, &pl);
    player_list_add(&pl_list, &pl2);
    player_list_to_obj_list(pl_list, &obj_list);
    main_pl = &pl;
    fps = max_fps;
    error_check((post_shared_data =
                     mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED,
                "mmap failed\n");
    error_check((return_shared_data =
                     mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED,
                "mmap failed\n");
    post_shared_data->player_id = 0;
    post_shared_data->read_flag = 0;
    post_shared_data->break_flag = 0;
    post_shared_data->frame = 0;
    return_shared_data->player_id = 0;
    return_shared_data->read_flag = 0;
    return_shared_data->break_flag = 0;
    return_shared_data->frame = 0;

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
                shd_players[0].pl = post_shared_data->pl;
                shd_players[0].key = post_shared_data->key;
                shd_players[0].frame = post_shared_data->frame;
                shd_players[0].break_flag = post_shared_data->break_flag;
                post_shared_data->read_flag = 1;
                /* unlock shared data */
                post_shared_data->lock_flag = 0;
            }
            if (return_shared_data->read_flag == 1 &&
                return_shared_data->lock_flag == 0) {
                /* lock shared data */
                return_shared_data->lock_flag = 1;
                return_shared_data->pl = shd_players[1].pl;
                return_shared_data->key = shd_players[1].key;
                return_shared_data->frame = shd_players[1].frame;
                return_shared_data->read_flag = 0;
                /* unlock shared data */
                return_shared_data->lock_flag = 0;
            }
            if (server_open_flag == 1) {
                /* send new shared data to client */
                error_check(send(new_socket, &shd_players[0],
                                 sizeof(SharedData), 0) < 0,
                            "send\n");
                /* read the shared data from client */
                error_check(
                    read(new_socket, &shd_players[1], sizeof(SharedData)) < 0,
                    "read\n");
            } else if (client_open_flag == 1) {
                /* send new shared data to server */
                error_check(
                    send(server_fd, &shd_players[0], sizeof(SharedData), 0) < 0,
                    "send\n");
                /* read the shared data from server */
                error_check(
                    read(server_fd, &shd_players[1], sizeof(SharedData)) < 0,
                    "read\n");
            }
            Sleep((1000.0 / max_fps) / 2.0);
        }
        /* send break flag to opponent */
        if (server_open_flag == 1) {
            shd_players[0].break_flag = 1;
            send(new_socket, &shd_players[0], sizeof(SharedData), 0);
            /* close socket */
            close(new_socket);
        } else if (client_open_flag == 1) {
            shd_players[0].break_flag = 1;
            send(server_fd, &shd_players[0], sizeof(SharedData), 0);
            /* close socket */
            close(server_fd);
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
        field_y = 200;
        field_z = 7 + field_y / y_angle;
        set_player_y(&pl, 5.5);
        set_player_y(&pl2, 5.5);
        if (client_open_flag == 1) {
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
            getmaxyx(stdscr, tmp_screen_height, tmp_screen_width);
            tmp_screen_height -= 2;
            tmp_screen_width -= 2;
            if (tmp_screen_width != screen_width ||
                tmp_screen_height != screen_height) {
                clear();
                screen_width = tmp_screen_width;
                screen_height = tmp_screen_height;
            }
            max_offscreen_height = screen_height / 2 - 5;
            /* ======================debug====================== */
            if (debug_flg == 1) {
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
                for (i = 1; i < frame_delta; i++) {
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
            sleep_time = (1000.0 / max_fps) * frame - d_sec * 1000.0;
            if (sleep_time < 0) {
                jump = (int)(-sleep_time / (1000.0 / max_fps));
                timeout_cnt += jump;
                if (jump > 0) {
                    players_erase();
                    for (i = 0; i < jump; i++) {
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
            if (debug_flg == 1) {
                fps = (int)(1.0 / (get_now_time() - fps_timestamp));
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
