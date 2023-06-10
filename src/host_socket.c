#include "../include/host_socket.h"

char version[10] = "0.0.2";

/* server */
int server_fd, new_socket;
struct sockaddr_in address;
int addrlen = sizeof(address);
char opponent_version[10];
char *ip_addr;
int frame_delta = 0;

void host_socket_init() {
    /* ======================server====================== */
    /* Creating socket file descriptor */
    error_check((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0,
                "socket failed\n");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    /* bind the socket to localhost port 50000 */
    error_check(
        bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0,
        "bind failed\n");
    fprintf(stderr, "waiting for connection...\n");
    /* try to specify maximum of 3 pending connections for the
     * master socket */
    error_check(listen(server_fd, 3) < 0, "listen\n");
    /* accept the incoming connection */
    error_check((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                     (socklen_t *)&addrlen)) < 0,
                "accept\n");
    /* read client version */
    error_check(
        read(new_socket, &opponent_version, sizeof(opponent_version)) < 0,
        "read\n");
    /* send server version to client */
    error_check(send(new_socket, &version, sizeof(version), 0) < 0, "send\n");
    /* check version */
    error_check(strcmp(opponent_version, version) != 0, "version mismatch\n");
}

void host_socket_close() {
    close(new_socket);
    close(server_fd);
}

void host_socket_send(SharedData *sd) {
    error_check(send(new_socket, sd, sizeof(SharedData), 0) < 0, "send\n");
}

void host_socket_recv(SharedData *sd) {
    error_check(read(new_socket, sd, sizeof(SharedData)) < 0, "read\n");
}