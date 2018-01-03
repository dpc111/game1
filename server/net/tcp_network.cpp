#include "tcp_network.h"
#include "log.h"
#include "net_pool.h"
#include "msg_dispatch.h"

tcp_network_t::tcp_network_t(const net_address_t& addr) {
	addr_ = addr;
	msg_operate_ = new msg_operate_t(this);
	msg_dispatch_ = new msg_dispatch_t(this);
	ev_base_ = NULL;
	ev_listen_ = NULL;
	sid_ = 0;
}

tcp_network_t::~tcp_network_t() {
	this->shutdown();
	sid_ = 0;
}

void tcp_network_t::start() {
	this->ev_start();
}

void tcp_network_t::shutdown() {
	ev_close();
	for (conn_map_t::iterator itr = conns_.begin(); itr != conns_.end(); ++itr) {
		tcp_connection_t *conn = itr->second;
		connection_free(conn);
	}
	conns_.clear();
}

void tcp_network_t::process() {

}

bool tcp_network_t::new_connection(const char *ip, int port, void *context) {
	net_address_t addr(ip, port);
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	int res = ::connect(fd, (const sockaddr *)&addr, sizeof(addr));
	if (res) {
		ERROR();
		evutil_closesocket(fd);
		return false;
	}
	tcp_connection_t *conn = connection_alloc();
	conn->set_fd(fd);
	conn->set_network(this);
	conn->set_context(context);
	conn->set_events(ev_base_, ev_read_cb, ev_write_cb);
	this->add_connection(conn);
	return true;
}

tcp_connection_t* tcp_network_t::get_connection(int fd) {
	conn_map_t::iterator it = conns_.find(fd);
	if (it == conns_.end()) {
		return NULL;
	}
	return it->second;
}

void tcp_network_t::add_connection(tcp_connection_t *conn) {
	tcp_connection_t *rm_conn = get_connection(conn->get_fd());
	assert(rm_conn == NULL);
	conns_.insert(conn_map_t::value_type(conn->get_fd(), conn));
}

void tcp_network_t::remove_connection(int fd) {
	conn_map_t::iterator it = conns_.find(fd);
	if (it == conns_.end()) {
		return;
	}
	conns_.erase(it);
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
		sizeof(addr_)
	);
}

void tcp_network_t::ev_close() {
	if (ev_listen_ != NULL) {
		evconnlistener_free(ev_listen_);
		ev_listen_ = NULL;
	}
	if (ev_base_ != NULL) {
		event_base_free(ev_base_);
		ev_base_ = NULL;
	}
}

void tcp_network_t::ev_listen_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *ud) {
	tcp_network_t *network = (tcp_network_t *)ud;
	assert(sa->sa_family == AF_INET);
	tcp_connection_t *conn = connection_alloc();
	conn->set_fd(fd);
	conn->set_peer_addr(*(sockaddr_in *)sa);
	conn->set_network(network);
	conn->set_events(network->get_ev_base(), ev_read_cb, ev_write_cb);
	network->add_connection(conn);
}

void tcp_network_t::ev_read_cb(evutil_socket_t fd, const short which, void *arg) {
	tcp_connection_t *conn = (tcp_connection_t *)arg;
	tcp_network_t *network = conn->get_network();
	net_input_stream_t& stream = conn->get_input_stream();
	int n = stream.read_fd(conn, fd);
	if (n <= 0) {
		if (n == 0 || !conn->reliable()) {
			network->remove_connection(fd);
		}
		return;
	}
	if (!network->get_msg_operate()->on_message(conn)) {
		stream.reset();
	}
}

void tcp_network_t::ev_write_cb(evutil_socket_t fd, const short which, void *arg) {
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
