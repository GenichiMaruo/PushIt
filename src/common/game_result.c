#include "../../include/common/game_result.h"

int is_game_end_box(Box box, int field_x, int color) {
    if (box.obj.x - box.obj.hitbox.size_x / 2.0 <= 5) {
        return (color == 1) ? 2 : 1;
    } else if (box.obj.x + box.obj.hitbox.size_x / 2.0 >= field_x - 5) {
        return (color == 1) ? 1 : 2;
    } else {
        return 0;
    }
}

int is_game_end_time(int frame, int max_fps) {
    if (frame >= max_fps * 120) {
        return 1;
    } else {
        return 0;
    }
}

int get_result_letter(int result, int x, int y) {
    int win[5][15] = {{1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1},
                      {1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1},
                      {1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1},
                      {1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1},
                      {0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1}};
    int lose[5][15] = {{1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
                       {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0},
                       {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
                       {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0},
                       {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1}};
    return result == 1 ? win[y][x] : lose[y][x];
}

void show_result(int result) {
    int x, y;
    getmaxyx(stdscr, y, x);
    int color = result == 1 ? 4 : 5;
    attron(COLOR_PAIR(color));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 15; j++) {
            if (get_result_letter(result, j, i) == 1) {
                mvaddch(y / 2 - 2 + i, x / 2 - 7 + j, ' ');
            }
        }
    }
    attroff(COLOR_PAIR(color));
    refresh();
}