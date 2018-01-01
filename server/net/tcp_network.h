#ifndef TCP_NETWORK_H
#define TCP_NETWORK_H

#include <event2/event.h>
#include <event2/listener.h>
#include "net_address.h"
#include "tcp_connection.h"

class msg_operate_t;
class msg_dispatch_t;

class tcp_network_t {
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

	bool new_connection(const char *ip, int port, void *context);

	tcp_connection_t *get_connection(int fd);

	void add_connection(tcp_connection_t *conn);

	void remove_connection(int fd);

	void send(tcp_connection_t *conn, google::protobuf::Message& msg) { msg_operate_->send(conn, msg); }

	void send(int fd, google::protobuf::Message& msg) { this->send(this->get_connection(fd), msg); }

	void ev_close();

	void ev_start();

	static void ev_listen_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *ud);

	static void ev_read_cb(evutil_socket_t fd, const short which, void *arg);

	static void ev_write_cb(evutil_socket_t fd, const short which, void *arg);

private:
	net_address_t addr_;

	conn_map_t conns_;

	msg_operate_t *msg_operate_;

	msg_dispatch_t *msg_dispatch_;

	event_base *ev_base_;

	evconnlistener *ev_listen_;
};

#endif