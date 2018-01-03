#ifndef SERVER_H
#define SERVER_H

#include "tcp_network.h"
#include ""

class server_t : public tcp_network_t {
public:
	server_t(const char *ip, int port);

	~server_t();

private:
	
};

#endif