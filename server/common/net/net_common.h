#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SET_SOCKET_NONBLOCK(fd) \
int flags = fcntl(fd, F_GETFL, 0); \
fcntl(fd, F_SETFL, flags | O_NONBLOCK); \
