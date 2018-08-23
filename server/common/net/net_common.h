#define SET_SOCKET_NONBLOCK(fd) \
int flags = fcntl(fd, F_GETFL, 0); \
fcntl(fd, F_SETFL, flags | O_NONBLOCK); \
