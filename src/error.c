#include "../include/error.h"

/* print error message and exit */
void error(char* msg, ...) {
    va_list args;
    fprintf(stderr, "error: ");
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    exit(1);
}

/* check if val is 1, if so, call error() */
void error_check(int val, char* msg) {
    if (val == 1) {
        error(msg);
    }
}