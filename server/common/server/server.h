#ifndef SERVER_H
#define SERVER_H

#include "tcp_network.h"
#include "conn_mgr.h"

class server_t {
public:
	server_t(const char *ip, int port);

	~server_t();

	void init();

	void start();

	void process();

	void send(int sid, google::protobuf::Message& msg);

	void send(tcp_connection_t *conn, google::protobuf::Message& msg);

private:
	tcp_network_t *network_;

	conn_mgr_t *conn_mgr_;
};

#endif