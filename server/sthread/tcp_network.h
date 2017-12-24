#ifndef TCP_NETWORK_H
#define TCP_NETWORK_H

#include <time.h>
#include <event2/event.h>
#include <event2/listener.h>

class tcp_network_t {
public:
	typedef std::map<int, tcp_connection_t *> conn_map_t;

public:
	tcp_network_t(const net_address_t& addr);

	~tcp_network_t();

	void start();

	void shutdown();
 
	void process();

	event_base *get_ev_base() { return ev_base_; }

	bool new_connection(const char *ip, int ip, void *context);

	tcp_connection_t *get_connection(int fd);

	void add_connection(tcp_connection_t *conn);

	const tcp_network_t& get_local_addr() { return addr_; }

private:
	static void ev_listen_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *ud);

	static void ev_read_cb(evutil_socket_t fd, const short which, void *arg);

	static void ev_write_cb(evutil_socket_t fd, const short which, void *arg);

private:
	net_address_t 							addr_;
	conn_map_t 		 						conns_;
	event_base 								*ev_base_;
	evconnlistener 							*ev_listen_;
	obj_pool_t<tcp_connection_t>  			conn_pool_;
}

#endif