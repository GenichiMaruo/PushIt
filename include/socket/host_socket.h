/* include guard */
#ifndef __HOST_SOCKET_H
#define __HOST_SOCKET_H

#include "../common/_common_group.h"
#include "../main.h"
#include "data_socket.h"

void host_socket_init();
void host_socket_close();
void host_socket_send(SharedData *sd);
void host_socket_recv(SharedData *sd);

#endif /* __HOST_SOCKET_H */