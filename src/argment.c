#include "../include/argment.h"

void argment_parse(int argc, char *argv[], ArgmentFlag *argment_flag) {
    /* ======================argment====================== */
    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                argment_print_help();
            } else if (strcmp(argv[i], "-v") == 0 ||
                       strcmp(argv[i], "--version") == 0) {
                argment_print_version();
            } else if (strcmp(argv[i], "-d") == 0 ||
                       strcmp(argv[i], "--debug") == 0) {
                argment_flag->debug = 1;
            } else if (strcmp(argv[i], "-f") == 0 ||
                       strcmp(argv[i], "--fps") == 0) {
                error_check(i + 1 >= argc, "fps is not specified\n");
                max_fps = atoi(argv[i + 1]);
                i++;
            } else if (strcmp(argv[i], "--server") == 0 &&
                       argment_flag->server == 0 && argment_flag->client == 0) {
                argment_flag->server = 1;
            } else if (strcmp(argv[i], "--client") == 0 &&
                       argment_flag->server == 0 && argment_flag->client == 0) {
                argment_flag->client = 1;
                error_check(i + 1 >= argc, "ip address is not specified\n");
                ip_addr = argv[i + 1];
                i++;
            } else if (strcmp(argv[i], "--aa") == 0) {
                argment_flag->aa = 1;
            } else {
                error("invalid argment: %s\n", argv[i]);
            }
        }
    }
}

void argment_print_help(void) {
    fprintf(stderr, "usage: %s [options]\n", PROGRAM_NAME);
    fprintf(stderr, "options:\n");
    fprintf(stderr, "  -h, --help     show this help message and exit\n");
    fprintf(stderr,
            "  -v, --version  show program's version number and "
            "exit\n");
    fprintf(stderr, "  -d, --debug    debug mode\n");
    fprintf(stderr, "  -f, --fps      set max fps\n");
    fprintf(stderr, "  --server       server mode\n");
    fprintf(stderr, "  --client       client mode\n");
    fprintf(stderr, "  --aa           enable ascii art\n");
    exit(0);
}
void argment_print_version(void) {
    fprintf(stderr, "version: %s\n", PROGRAM_VERSION);
    exit(0);
}