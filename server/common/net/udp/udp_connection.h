#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include "rudp.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

class udp_network_t;

class udp_connection_t {
public:
	udp_connection_t(udp_network_t *network, struct sockaddr_in addr, int sid);

	~udp_connection_t();

	int64 get_addr() { return addr_; }

	int get_sid() { return sid_; }

	udp_handle_t* get_udp_handle() { return udp_handle_; }

	void on_recv_udp_chunk(udp_chunk_t *c);

	int send(void *buff, int size);

	int send_chunk_force(udp_chunk_t *c);

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