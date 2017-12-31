#include "tcp_network.h"
#include "net_pool.h"

tcp_network_t::tcp_network_t(const net_address_t& addr) {
	addr_ = addr;
	msg_operate_ = new msg_operate_t(this);
	msg_dispatch_ = new msg_dispatch_t(this);
}

tcp_network_t::~tcp_network_t() {
	this->shutdown();
}

void tcp_network_t::shutdown() {
	ev_close();
	for (conn_map_t::iterator itr = conns_.begin(); itr != conns_.end(); ++itr) {
		tcp_connection_t *conn = itr->second;
		conn_pool_.free(conn);
	}
	conns_.clear();
}

void tcp_network_t::process() {

}

bool new_connection(const char *ip, int port, void *context) {
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
	conn->set_peer_addr(addr);
	conn->set_network(this);
	conn->set_context(context);
	conn->set_events(ev_base_, ev_read_cb, ev_write_cb);
	this->add_connection(conn);
	return true;
}

tcp_connection_t* tcp_network_t::get_connection(int fd) {
	conn_map_t::iterator it = conns_.find(fd);
	if (itr == conns_.end()) {
		return NULL;
	}
	return itr->second;
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