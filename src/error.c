#include "../include/error.h"

/* print error message and exit */
void error(char* msg) {
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}

/* check if val is 1, if so, call error() */
void error_check(int val, char* msg) {
    if (val == 1) {
        error(msg);
    }
}