#ifndef SERVER_H
#define SERVER_H

#include "tcp_network.h"
#include "conn_mgr.h"
#include "timer.h"

class server_t {
public:
	server_t(const char *ip, int port);

	~server_t();

	void init();

	void start();

	void process();

	tcp_network_t *get_network() { return network_; }

	void send(int sid, google::protobuf::Message& msg);

	void send(tcp_connection_t *conn, google::protobuf::Message& msg);

	void register_timer(timer_handler_t *handler, void *user, timestamp start, timestamp interval);

private:
	tcp_network_t *network_;

	conn_mgr_t *conn_mgr_;

	timers_t *times_;
};

#endif