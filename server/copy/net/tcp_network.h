#ifndef TCP_NETWORK_H
#define TCP_NETWORK_H

#include <stdio.h>
#include <map>
//#include "net_thread.h"
#include "thread_cmd.h"

class net_address_t;
class net_thread_t;
class poller_t;

class tcp_network_t
{
public:
	typedef std::map<int, tcp_connection_t *> conn_map_t;
	typedef std::map<int, net_thread_t *> thread_map_t;

public: 
	tcp_network_t(const char *ip, int port);

	~tcp_network_t();

	bool is_quit() { return is_quit_; }

	net_thread_t* get_idle_thread();

	conn_map_t& get_conn_map() { return conns_; } 

	void set_conn_add_cb(const conn_add_cb_t& cb) { conn_add_cb_ = cb; }

	void set_conn_remove_cb(const conn_remove_cb_t& cb) { conn_remove_cb_ = cb; }

	void set_message_recv_cb(const message_recv_cb_t& cb) { message_recv_cb_ = cb; }

	void on_conn_add(tcp_connection_t *conn);

	void on_conn_remove(int fd);

	bool on_message_recv(tcp_connection_t *conn);

	static void on_socket_connect(int fd, void *ud);

	void start();

	void process();	

private:
	bool 						is_quit_;
	conn_map_t 					conns_;
	thread_map_t 				threads_;
	poller_t 					*poller_;
	net_address_t 				addr_;
	conn_add_cb_t 				conn_add_cb_;
	conn_remove_cb_t 			conn_remove_cb_;
	message_recv_cb_t			message_recv_cb_;
};

#endif