#ifndef _SOCKET_INIT_H
#define _SOCKET_INIT_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

int socket_init(int port);



#endif
