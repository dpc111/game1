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
	tcp_connection_t *conn = (tcp_connection_t *)arg;
	tcp_network_t *network = conn->get_network();
	net_input_stream_t& stream = conn->get_input_stream();
	int n = stream.read_fd(fd);
	if (n <= 0) {
		if (n == 0 || !conn->reliable) {
			network->remove_connection(fd);
		}
		return
	}
	if (!network->on_message_cb(conn)) {
		stream.reset();
	}
}

void net_event_t::ev_write_cb(evutil_socket_t fd, const short which, void *arg) {
	tcp_connection_t *conn = (tcp_connection_t *)arg;
	tcp_network_t *network = conn->get_network();
	net_output_stream_t& stream = conn->get_output_stream();
	if (stream.size() <= 0) {
		conn->del_event_write();
		if (!conn->connected()) {
			network->remove_connection(fd);
		}
		return;
	}
	int n = stream.write_fd(conn, fd);
	if (n < 0) {
		if (conn->reliable()) {
			conn->add_event_write();
		} else {
			stream.reset();
			network->remove_connection(fd);
		}
		return;
	}
	if (!conn->connected()) {
		network->remove_connection(fd);
	}
}