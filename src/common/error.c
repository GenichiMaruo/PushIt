#include "../../include/common/error.h"

int error_flag = 0;

/* print error message and exit */
void error(char* msg, ...) {
    va_list args;
    va_start(args, msg);
    char* buf = (char*)malloc(sizeof(char) * 1024);
    vsprintf(buf, msg, args);
    menu_waiting_message(buf);
    free(buf);
    va_end(args);
    error_flag = 1;
}

/* check if val is 1, if so, call error() */
void error_check(int val, char* msg) {
    if (val == 1) {
        error(msg);
    }
}