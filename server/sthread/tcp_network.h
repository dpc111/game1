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

private:
	static void listener_callback(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *ud);

private:
	conn_map_t 		 		conns_;
	event_base 				*ev_base_;
	evconnlistener 			*ev_listener_;

}

#endif