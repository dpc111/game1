#include "net_event.h"

net_event_t::net_event_t() {
	ev_base_ = NULL;
	ev_listen_ = NULL;
}

net_event_t::~net_event_t() {

}

void tcp_network_t::ev_start() {
	assert(ev_base_ == NULL);
	assert(ev_listen_ == NULL);
	ev_base_ = event_base_new();	
	ev_listen_ = evconnlistener_new_bind(
		ev_base_,
		ev_listen_cb,
		this,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		-1,
		(struct sockaddr*)&addr_,
		sizeof(addr_);
	);
}

void net_event_t::ev_close() {
	if (ev_listen_ != NULL) {
		evconnlistener_free(ev_listen_);
		ev_listen_ = NULL;
	}
	if (ev_base_ != NULL) {
		event_base_free(ev_base_);
		ev_base_ = NULL;
	}
}

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
	if (!network->get_msg_operate()->on_message(conn)) {
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