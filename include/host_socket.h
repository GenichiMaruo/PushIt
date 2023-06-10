/* include guard */
#ifndef __HOST_SOCKET_H
#define __HOST_SOCKET_H

#define PORT 50000

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "data_socket.h"
#include "error.h"
#include "main.h"

void host_socket_init();
void host_socket_close();
void host_socket_send(SharedData *sd);
void host_socket_recv(SharedData *sd);

#endif /* __HOST_SOCKET_H */