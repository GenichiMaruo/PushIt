/* include guard */
#ifndef ARGMENT_H
#define ARGMENT_H

#include "main.h"

extern char *ip_addr;

typedef struct ArgmentFlag {
    short int debug;
    short int server;
    short int client;
    short int aa;
} ArgmentFlag;

void argment_parse(int argc, char *argv[], ArgmentFlag *argment_flag);
void argment_print_help(void);
void argment_print_version(void);

#endif /* ARGMENT_H */