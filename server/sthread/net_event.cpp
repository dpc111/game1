#include "net_event.h"

void net_event_t::ev_listen_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *ud) {
	assert(sa->sa_family == AF_INET);
	tcp_connection_t *conn = connection_alloc();
	conn->set_fd(fd);
	conn->set_peer_addr(*(sockaddr_in *)sa);
	conn->set_network(this);
	conn->set_events(ev_base_, ev_read_cb, ev_write_cb);
	this->add_connection(conn);
}

void net_event_t::ev_read_cb(evutil_socket_t fd, const short which, void *arg) {

}

void net_event_t::ev_write_cb(evutil_socket_t fd, const short which, void *arg) {

}