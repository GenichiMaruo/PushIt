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
    /* init socket */
    if (argment_flag.server == 1) {
        host_socket_init();
    } else if (argment_flag.client == 1) {
        guest_socket_init(ip_addr);
    }
    init_ncurses();
    /* ======================menu====================== */
    int selected_option = menu_main();

    // 選択されたオプションに応じて何かの処理を行う
    switch (selected_option) {
        case 1:
            printf("host serverが選択されました。\n");
            break;
        case 2:
            printf("find serverが選択されました。\n");
            break;
        default:
            printf("無効な選択です。\n");
            break;
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
