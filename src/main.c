#include "../include/main.h"

int field_x = 200, field_z = 30, screen_width, screen_height;
int frame = 0, timeout_cnt = 0;
Player *main_pl = NULL;
PlayerList *pl_list = NULL;
ObjList *obj_list = NULL;
ArgmentFlag argment_flag = {0};

int main(int argc, char **argv) {
    int result = 0;
    /* ======================init====================== */
    argment_parse(argc, argv, &argment_flag);
    /* init config */
    config_init();
    init_ncurses();
    /* ======================menu====================== */
    while (1) {
        int selected_option = menu_main();
        switch (selected_option) {
            case 1:
                argment_flag.server = 1;
                break;
            case 2: {
                int selected_input_ip = menu_input_ip();
                if (selected_input_ip == 1) {
                    argment_flag.client = 1;
                }
            } break;
            default:
                printf("無効な選択です。\n");
                break;
        }
        if (argment_flag.server == 1 || argment_flag.client == 1) {
            break;
        }
    }

    /* init socket */
    if (argment_flag.server == 1) {
        host_socket_init();
    } else if (argment_flag.client == 1) {
        guest_socket_init(ip_addr);
    }
    result = pushit_game_main();

    /* ======================result====================== */
    show_result(result);
    Sleep(3000);

    getch();
    endwin();
    wait(NULL);
    return EXIT_SUCCESS;
}
