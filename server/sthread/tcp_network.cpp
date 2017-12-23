tcp_network_t::tcp_network_t(const net_address_t& addr){
	ev_base_ = NULL;
	ev_listener_ = NULL;
	addr_ = addr;
}

tcp_network_t::~tcp_network_t() {
	this->shutdown();
}

void tcp_network_t::start() {
	assert(ev_base_ == NULL);
	assert(ev_listener_ == NULL);
	ev_base_ = event_base_new();	
	ev_listener_ = evconnlistener_new_bind(
		get_ev_base(),
		listener_callback,
		this,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		-1,
		(struct sockaddr*)&addr_,
		sizeof(addr_);
	);
}

void tcp_network_t::shutdown() {
	if (ev_listener_ != NULL) {
		evconnlistener_free(ev_listener_);
		ev_listener_ = NULL;
	}
	if (ev_base_ != NULL) {
		event_base_free(ev_base_);
		ev_base_ = NULL;
	}
	for (conn_map_t::iterator itr = conns_.begin(); itr != conns_.end(); ++itr) {
		tcp_connection_t *conn = itr->second;
		// ??
		pool_free(conn);
	}
	conns_.clear();
	// ??
	pool_destroy();
}

void tcp_network_t::process() {

}

void tcp_network_t::listener_callback(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *ud) {
	assert(sa->sa_family == AF_INET);
	tcp_connection_t *conn = this->pool_alloc();
	assert(conn != NULL);
	conn->set_owner(this);
	this->add_connection(conn);
}

bool new_connection(const char *ip, int ip, void *context) {
	net_address_t addr(ip, port);
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	int res = ::connect(fd, (const sockaddr *)&addr, sizeof(addr));
	if (res) {
		ERROR();
		evutil_closesocket(fd);
		return false;
	}
	net_address_t local_addr(0);
	socklen_t len = sizeof(local_addr);
	int res = ::getsockname(fd, reinterpret_cast<sockaddr *>(&local_addr), &len);
	if (res) {
		evutil_closesocket(fd);
		return false;
	}
	// ??
	tcp_connection_t *conn = pool_alloc(fd, local_addr, addr);
	assert(conn);
	conn->set_context(context);
	conn->set_network(this);
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
	assert(rm_conn == NULL || rm_conn->disconnected());
	if (rm_conn != NULL) {

	}
	conns_.insert(conn_map_t::value_type(conn->get_fd(), conn));
}
