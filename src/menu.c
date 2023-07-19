#include "../include/manu.h"

// マウスイベントを有効にする関数
void enable_mouse() {
    mousemask(ALL_MOUSE_EVENTS, NULL);  // 全てのマウスイベントを受け取る
    keypad(stdscr, TRUE);  // 特殊キー（Fキーなど）の受け取りを有効化
    mouseinterval(0);  // マウスの速度制御を無効化
    nodelay(stdscr, TRUE);  // キーボードの非ブロッキング入力を有効化
}

int menu_main(void) {
    enable_mouse();  // マウスイベントを有効にする

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);  // 画面のサイズを取得

    // タイトルロゴを表示（中央の上部）
    mvprintw(maxY / 2 - 5, (maxX - 10) / 2, "Title Logo");

    // "host server"ボタンを作成（中央左下）
    int host_server_button_y = maxY / 2 + 3;
    int host_server_button_x = maxX / 4;
    mvprintw(host_server_button_y, host_server_button_x, "host server");

    // "find server"ボタンを作成（中央右下）
    int find_server_button_y = maxY / 2 + 3;
    int find_server_button_x = maxX * 3 / 4 - 11;  // 11は"find server"の文字数
    mvprintw(find_server_button_y, find_server_button_x, "find server");

    refresh();

    MEVENT event;
    int ch;
    bool mouse_clicked = false;
    bool key_pressed = false;

    while (1) {
        ch = getch();  // キーボードの入力を待機

        // マウスイベントを取得
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED) {
                    mouse_clicked = true;
                }
            }
        }

        // キーボード入力を検知
        if (ch != ERR) {
            key_pressed = true;
        }

        // "host server"ボタンがクリックされた場合
        if (mouse_clicked || (key_pressed && ch == KEY_F(1))) {
            return 1;  // 任意の値を返す（ここでは1を返す）
        }

        // "find server"ボタンがクリックされた場合
        if (mouse_clicked || (key_pressed && ch == KEY_F(2))) {
            return 2;  // 任意の値を返す（ここでは2を返す）
        }

        mouse_clicked = false;
        key_pressed = false;
    }

    // 通常、ここには到達しないが、念のため0を返す
    return 0;
}