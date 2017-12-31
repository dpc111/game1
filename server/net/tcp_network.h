#ifndef TCP_NETWORK_H
#define TCP_NETWORK_H

#include "net_event.h"

class tcp_network_t : public net_event_t {
public:
	typedef std::map<int, tcp_connection_t *> conn_map_t;

public:
	tcp_network_t(const net_address_t& addr);

	~tcp_network_t();

	void start();

	void shutdown();
 
	void process();

	const tcp_network_t& get_local_addr() { return addr_; }

	msg_operate_t *get_msg_operate() { return msg_operate_; }

	msg_dispatch_t *get_msg_dispatch() { return msg_dispatch_; }

	bool new_connection(const char *ip, int ip, void *context);

	tcp_connection_t *get_connection(int fd);

	void add_connection(tcp_connection_t *conn);

	void remove_connection(fd);

	void send(tcp_connection_t *conn, google::protobuf::Message& msg) { msg_operate_->send(conn, msg); }

	void send(int fd, google::protobuf::Message& msg) { this->send(this->get_connection(fd), msg); }

private:
	net_address_t addr_;

	conn_map_t conns_;

	msg_operate_t *msg_operate_;

	msg_dispatch_t *msg_dispatch_;
};

#endif