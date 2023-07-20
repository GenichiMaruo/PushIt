#include "../include/manu.h"

int menu_main(void) {
    int ch, choice = 0;
    MEVENT event;

    // Initialize ncurses
    initscr();
    start_color();  // Enable color mode
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    curs_set(0);

    // Set colors
    init_pair(100, COLOR_BLACK,
              COLOR_YELLOW);  // Title: black text on yellow background
    init_pair(200, COLOR_WHITE,
              COLOR_BLUE);  // Host Server and Find Server button: white text on
                            // blue background

    // Get the size of the terminal window
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Set the title text
    char title[5][24] = {"### # # ### # # ### ###", "# # # # #   # #  #   # ",
                         "### # # ### ###  #   # ", "#   # #   # # #  #   # ",
                         "#   ### ### # # ###  # "};

    // Set the button labels
    char host_label[] = "[Host Server]";
    char find_label[] = "[Find Server]";

    // Calculate the positions for buttons and title
    int title_y = max_y / 2 - 5;
    int title_x = (max_x - 23) / 2;
    int host_y = max_y / 2 + 2;
    int host_x = (max_x - 13) / 4;
    int find_y = max_y / 2 + 2;
    int find_x = (max_x - 13) / 2 + (max_x - 13) / 4;

    // Draw the title and buttons
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 23; j++) {
            if (title[i][j] == '#') {
                attron(COLOR_PAIR(100));
                mvprintw(title_y + i, title_x + j, " ");
                attroff(COLOR_PAIR(100));
            } else {
                mvprintw(title_y + i, title_x + j, " ");
            }
        }
    }
    attron(COLOR_PAIR(200));
    mvprintw(host_y, host_x, "%s", host_label);
    mvprintw(find_y, find_x, "%s", find_label);
    attroff(COLOR_PAIR(200));
    refresh();

    // Wait for mouse input and handle the selected button
    while (1) {
        ch = getch();
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_PRESSED) {
                    // Check if the mouse click is within the "Host Server"
                    // button
                    if (event.y == host_y && event.x >= host_x &&
                        event.x < host_x + 13) {
                        choice = 1;
                        break;
                    }

                    // Check if the mouse click is within the "Find Server"
                    // button
                    if (event.y == find_y && event.x >= find_x &&
                        event.x < find_x + 13) {
                        choice = 2;
                        break;
                    }
                }
            }
        }
    }
    clear();
    // Cleanup ncurses and return the selected choice
    endwin();
    return choice;
}

int menu_input_ip(void) {
    int ch, choice = 0;
    MEVENT event;

    // Initialize ncurses
    initscr();
    raw();  // Use raw mode for immediate input without line buffering
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    curs_set(0);

    // Enable non-blocking input
    nodelay(stdscr, TRUE);

    // Get the size of the terminal window
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Set the button labels
    char confirm_label[] = "[ Confirm ]";
    char cancel_label[] = "[ Cancel  ]";

    // Calculate the positions for buttons and input field
    int input_y = max_y / 2 - 2;
    int input_x = max_x / 2 - 24;  // Adjust based on the input field width
    int confirm_y = max_y / 2 + 2;
    int confirm_x = (max_x - 13) / 4;
    int cancel_y = max_y / 2 + 2;
    int cancel_x = (max_x - 13) / 2 + (max_x - 13) / 4;

    // Draw the input field and buttons
    char* tmp_ip_addr =
        (char*)malloc(21 * sizeof(char));  // Allocate memory for IP address
                                           // input (including null terminator)
    memset(tmp_ip_addr, '\0',
           21);  // Initialize IP address with null characters
    bool valid_ip = false;
    while (!valid_ip) {
        mvprintw(input_y, input_x, "IP Address: %s", tmp_ip_addr);
        mvprintw(confirm_y, confirm_x, "%s", confirm_label);
        mvprintw(cancel_y, cancel_x, "%s", cancel_label);
        refresh();

        ch = getch();
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_PRESSED) {
                    // Check if the mouse click is within the Confirm button
                    if (event.y == confirm_y && event.x >= confirm_x &&
                        event.x < confirm_x + 12) {
                        choice = 1;
                        break;
                    }

                    // Check if the mouse click is within the Cancel button
                    if (event.y == cancel_y && event.x >= cancel_x &&
                        event.x < cancel_x + 12) {
                        choice = 2;
                        break;
                    }
                }
            }
        } else if (ch != ERR) {
            // Regular character input, add it to the IP address
            int len = strlen(tmp_ip_addr);
            if (ch == 127 && len > 0) {
                // Handle backspace (ASCII code 127)
                tmp_ip_addr[len - 1] = '\0';

                // Erase the character visually
                mvprintw(input_y, input_x, "IP Address: %*s", 20, " ");
            } else if (len < 20) {
                // Maximum length of the IP address is 20 characters
                tmp_ip_addr[len] = ch;

                // Display the new character
                mvaddch(input_y, input_x + len, ch);
            }
            refresh();  // Refresh the screen to apply changes
        }
    }
    clear();
    // Cleanup ncurses and return the selected choice
    endwin();
    ip_addr = (char*)malloc(21 * sizeof(char));
    if (choice == 1) {
        strcpy(ip_addr, tmp_ip_addr);
    }
    return choice;
}
