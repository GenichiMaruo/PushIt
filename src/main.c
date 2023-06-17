#include "../include/main.h"

int field_x = 200, field_z = 30, screen_width, screen_height;
int frame = 0, timeout_cnt = 0;
Player *main_pl = NULL;
PlayerList *pl_list = NULL;
ObjList *obj_list = NULL;
ArgmentFlag argment_flag = {0};

int main(int argc, char **argv) {
    int break_flg = 0;
    double d_sec = 0, fps_timestamp = 0;
    Player pl, pl2;
    Box box;
    SharedData *post_shared_data, *return_shared_data;
    SharedData shd_players[2];
    KeyFlag key_flag = {0}, p2_key_flag = {0};
    FieldPos field_pos;
    /* ======================init====================== */
    argment_parse(argc, argv, &argment_flag);
    /* init config */
    config_init();
    /* init socket */
    if (argment_flag.server == 1) {
        host_socket_init();
    } else if (argment_flag.client == 1) {
        guest_socket_init(ip_addr);
    }
    /* init objects */
    player_init(&pl, 0, 0, 0, 0, 0, 100);
    player_init(&pl2, 0, 0, 0, 0, 0, 100);
    player_list_add(&pl_list, &pl);
    player_list_add(&pl_list, &pl2);
    player_list_to_obj_list(pl_list, &obj_list);
    main_pl = &pl;
    box_init(&box, 3, field_x / 2, field_z / 2, 5 * 2 + 1, 5, 3);
    obj_list_add(&obj_list, &box.obj);
    /* init shared data */
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
    shared_data_init(&shd_players[0]);
    shared_data_init(&shd_players[1]);
    /* ======================fork====================== */
    pid_t pid = fork();
    if (pid < 0) {
        error("fork failed\n");
    } else if (pid == 0) { /* ~~~~~~~~~child process~~~~~~~~~ */
        set_box_pos(&box, field_x / 2, field_z / 2);
        set_box_in_shared_data(&shd_players[0], box);
        set_box_in_shared_data(&shd_players[1], box);
        while (break_flg == 0) { /* =====main loop===== */
            /* ======================termination check====================== */
            if (post_shared_data->break_flag == 1) {
                break_flg = 1;
            }
            /* ======================data exchange====================== */
            shared_data_copy(&shd_players[0], post_shared_data);
            shared_data_copy(return_shared_data, &shd_players[1]);
            if (argment_flag.server == 1) {
                host_socket_send(&shd_players[0]);
                host_socket_recv(&shd_players[1]);
            } else if (argment_flag.client == 1) {
                guest_socket_send(&shd_players[0]);
                guest_socket_recv(&shd_players[1]);
            }
            Sleep((1000.0 / max_fps) / 2.0);
        } /* =====end loop===== */
        /* close socket */
        if (argment_flag.server == 1) {
            host_socket_close();
        } else if (argment_flag.client == 1) {
            guest_socket_close();
        }
        fprintf(stderr, "child process end\n");
        return 0;
    } else { /* ~~~~~~~~~parent process~~~~~~~~~ */
        /* close socket */
        if (argment_flag.server == 1) {
            host_socket_close();
        } else if (argment_flag.client == 1) {
            guest_socket_close();
        }
        init_ncurses();
        set_player_x(&pl, argment_flag.client == 1 ? field_x - 5.5 : 5.5);
        set_player_x(&pl2, argment_flag.client == 1 ? 5.5 : field_x - 5.5);
        pl.dir_x = argment_flag.client == 1 ? PLAYER_LEFT : PLAYER_RIGHT;
        pl2.dir_x = argment_flag.client == 1 ? PLAYER_RIGHT : PLAYER_LEFT;
        set_player_color(&pl, argment_flag.client == 1 ? 2 : 1);
        set_player_color(&pl2, argment_flag.client == 1 ? 1 : 2);
        import_from_txt();
        get_start_time();
        for (frame = 1; break_flg == 0; frame++) { /* =====main loop===== */
            get_screen_size();
            /* ======================termination check====================== */
            /* end after 1000s or end key pressed */
            if (frame >= max_fps * 1000 || key_flag.end == 1 ||
                p2_key_flag.end == 1) {
                break_flg = 1;
                post_shared_data->break_flag = 1;
            }
            /* ======================debug====================== */
            if (argment_flag.debug == 1) {
                fps_timestamp = get_now_time();
            }
            /* ======================Player input====================== */
            getch();
            get_key_flag(&key_flag);
            players_erase();
            box_erase(box);
            new_player_positon(&pl, key_flag, d_sec);
            /* ======================Player2 input====================== */
            int frame_delta = 0;
            Box tmp_box;
            if (take_shared_data(return_shared_data, &pl2, &tmp_box,
                                 &frame_delta, &p2_key_flag) == 1) {
                /* Predicting the motion of the current frame based on the
                 * information obtained from the received frame */
                frame_delta = frame - frame_delta;
                for (int i = frame_delta; i > 1; i--) {
                    new_player_positon(&pl2, p2_key_flag, 1.0 / max_fps);
                    player_update(&pl2, 1.0 / max_fps);
                    box_update(&tmp_box, 1.0 / max_fps);
                }
                if (is_box_followed(tmp_box) == 1 &&
                    tmp_box.obj.followed_obj->id != main_pl->obj.id) {
                    box = tmp_box;
                }
            } else {
                new_player_positon(&pl2, p2_key_flag, d_sec);
            }
            /* ======================update====================== */
            for (PlayerList *tmp_pl_list = pl_list; tmp_pl_list != NULL;
                 tmp_pl_list = tmp_pl_list->next) {
                player_update(tmp_pl_list->pl, 1.0 / max_fps);
            }
            box_update(&box, 1.0 / max_fps);
            make_shared_data(post_shared_data, pl, box, frame, key_flag);
            /* ======================fps control====================== */
            field_draw(&field_pos);
            players_draw();
            box_draw(&box);
            refresh();
            get_end_time();
            d_sec = calc_time();
            double sleep_time = (1000.0 / max_fps) * frame - d_sec * 1000.0;
            if (sleep_time < 0) {
                int jump = (int)(-sleep_time / (1000.0 / max_fps));
                timeout_cnt += jump;
                if (jump > 0) {
                    players_erase();
                    box_erase(box);
                    for (int i = 0; i < jump; i++) {
                        for (PlayerList *tmp_pl_list = pl_list;
                             tmp_pl_list != NULL;
                             tmp_pl_list = tmp_pl_list->next) {
                            player_update(tmp_pl_list->pl, 1.0 / max_fps);
                            box_update(&box, 1.0 / max_fps);
                        }
                    }
                    frame += jump;
                    field_draw(&field_pos);
                    players_draw();
                    box_draw(&box);
                }
            } else {
                Sleep(sleep_time);
            }
            /* ======================debug====================== */
            if (argment_flag.debug == 1) {
                int fps = (int)(1.0 / (get_now_time() - fps_timestamp));
                debug_draw(fps, fps_timestamp, d_sec, sleep_time, frame_delta,
                           box);
            }
        } /* =====end loop===== */
    }
    getch();
    endwin();
    wait(NULL);
    fprintf(stderr, "parent process end\n");
    return EXIT_SUCCESS;
}
