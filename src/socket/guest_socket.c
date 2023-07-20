#include "../../include/socket/guest_socket.h"

/* client */
int guest_server_fd;
extern struct sockaddr_in address;

void guest_socket_init(char *ip_addr) {
    char version[10] = PROGRAM_VERSION;
    char opponent_version[10];
    /* ======================client====================== */
    /* Check if the input is "localhost" and replace it with the
     * corresponding IP address */
    if (strcmp(ip_addr, "localhost") == 0) {
        ip_addr = "127.0.0.1";
    }
    /* get ip address from hostname */
    if (strcmp(ip_addr, "localhost") != 0) {
        struct hostent *host;
        struct in_addr **addr_list;
        int i;
        if ((host = gethostbyname(ip_addr)) == NULL) {
            error("gethostbyname\n");
            return;
        }
        addr_list = (struct in_addr **)host->h_addr_list;
        for (i = 0; addr_list[i] != NULL; i++) {
            ip_addr = inet_ntoa(*addr_list[i]);
        }
    }
    /* Creating socket file descriptor */
    error_check((guest_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0,
                "socket failed\n");
    memset(&address, '0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    /* Convert IPv4 and IPv6 addresses from text to binary form */
    error_check(inet_pton(AF_INET, ip_addr, &address.sin_addr) <= 0,
                "Invalid address/ Address not supported \n");
    /* connect the socket to the server address */
    menu_waiting_message("Connecting...");
    error_check(connect(guest_server_fd, (struct sockaddr *)&address,
                        sizeof(address)) < 0,
                "Connection Failed \n");
    /* send client version to server */
    error_check(send(guest_server_fd, &version, sizeof(version), 0) < 0,
                "send\n");
    /* read server version */
    error_check(
        read(guest_server_fd, &opponent_version, sizeof(opponent_version)) < 0,
        "read\n");
    /* check version */
    error_check(strcmp(opponent_version, version) != 0, "version mismatch\n");
}

void guest_socket_close() { close(guest_server_fd); }

void guest_socket_send(SharedData *sd) {
    error_check(send(guest_server_fd, sd, sizeof(SharedData), 0) < 0, "send\n");
}

void guest_socket_recv(SharedData *sd) {
    error_check(read(guest_server_fd, sd, sizeof(SharedData)) < 0, "read\n");
}