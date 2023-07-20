/* include guard */
#ifndef __GUEST_SOCKET_H
#define __GUEST_SOCKET_H

#include "../common/_common_group.h"
#include "../main.h"
#include "data_socket.h"

int guest_socket_init(char *ip_addr);
void guest_socket_close();
void guest_socket_send(SharedData *sd);
void guest_socket_recv(SharedData *sd);

#endif /* __GUEST_SOCKET_H */