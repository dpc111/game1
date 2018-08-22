#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include "rudp.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

class udp_network_t;

class udp_connection_t {
public:
	udp_connection_t(udp_network_t *network, struct sockaddr_in& addr, int sid);

	~udp_connection_t();

	int send(void *buff, int size);

	int process();

public:
	struct sockaddr_in address_;

	int64 addr_;

	int sid_;

private:
	udp_network_t *network_;

	udp_handle_t *udp_handle_;
};

#endif