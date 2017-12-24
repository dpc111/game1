tcp_network_t::tcp_network_t(const net_address_t& addr){
	ev_base_ = NULL;
	ev_listen_ = NULL;
	addr_ = addr;
}

tcp_network_t::~tcp_network_t() {
	this->shutdown();
}

void tcp_network_t::start() {
	assert(ev_base_ == NULL);
	assert(ev_listen_ == NULL);
	ev_base_ = event_base_new();	
	ev_listen_ = evconnlistener_new_bind(
		get_ev_base(),
		ev_listen_cb,
		this,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		-1,
		(struct sockaddr*)&addr_,
		sizeof(addr_);
	);
}

void tcp_network_t::shutdown() {
	if (ev_listen_ != NULL) {
		evconnlistener_free(ev_listen_);
		ev_listen_ = NULL;
	}
	if (ev_base_ != NULL) {
		event_base_free(ev_base_);
		ev_base_ = NULL;
	}
	for (conn_map_t::iterator itr = conns_.begin(); itr != conns_.end(); ++itr) {
		tcp_connection_t *conn = itr->second;
		conn_pool_.free(conn);
	}
	conns_.clear();
}

void tcp_network_t::process() {

}

void tcp_network_t::ev_listen_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *ud) {
	assert(sa->sa_family == AF_INET);
	tcp_connection_t *conn = conn_pool_.alloc();
	conn->set_fd(fd);
	conn->set_peer_addr(*(sockaddr_in *)sa);
	conn->set_network(this);
	conn->set_events(ev_base_, ev_read_cb, ev_write_cb);
	this->add_connection(conn);
}

void tcp_network_t::ev_read_cb(evutil_socket_t fd, const short which, void *arg) {

}

void tcp_network_t::ev_write_cb(evutil_socket_t fd, const short which, void *arg) {

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
	tcp_network_t *conn = conn_pool_.alloc();
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

