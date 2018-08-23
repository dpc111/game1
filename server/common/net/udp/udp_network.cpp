#include "udp_network.h"
#include "udp_connection.h"

udp_network_t::udp_network_t() {
	cur_recv_chunk_ = NULL;
	udp_chunk_pool_ = new udp_chunk_pool_t;
	udp_handle_pool_ = new udp_handle_pool_t;
	udp_conn_pool_ = new udp_connection_pool_t;
	conn_num_ = 0;
	udp_msg_cb_ = NULL;
	socket_fd_ = 0;
	single_select_ = NULL;
}

udp_network_t::~udp_network_t() {
	if (cur_recv_chunk_ != NULL) {
		chunk_pool_free(udp_chunk_pool_, cur_recv_chunk_);
		cur_recv_chunk_ = NULL;
	}
	delete udp_chunk_pool_;
	delete udp_handle_pool_;
	delete udp_conn_pool_;
	conn_num_ = 0;
	udp_msg_cb_ = NULL;
	socket_fd_ = 0;
	if (single_select_ != NULL) {
		delete single_select_;
		single_select_ = NULL;
	}
}

void udp_network_t::add_connection(udp_connection_t *conn) {
	addr_conn_map_t::iterator it1 = addr_conns_.find(conn->addr_);
	if (it1 != addr_conns_.end()) {
		return;
	}
	sid_conn_map_t::iterator it2 = sid_conns_.find(conn->sid_);
	if (it2 != sid_conns_.end()) {
		return;
	}
	addr_conns_[conn->addr_ ] = conn;
	sid_conns_[conn->sid_] = conn;
	++conn_num_;
}

void udp_network_t::remove_connection_addr(int64 addr) {
	addr_conn_map_t::iterator it1 = addr_conns_.find(addr);
	if (it1 == addr_conns_.end()) {
		return;
	}
	udp_connection_t *conn = it1->second;
	addr_conns_.erase(it1);
	sid_conn_map_t::iterator it2 = sid_conns_.find(conn->sid_);
	if (it2 != sid_conns_.end()) {
		sid_conns_.erase(it2);
	}
	udp_conn_pool_->free(conn);
	--conn_num_;
}

void udp_network_t::remove_connection_sid(int sid) {
	sid_conn_map_t::iterator it1 = sid_conns_.find(sid);
	if (it1 == sid_conns_.end()) {
		return;
	}
	udp_connection_t *conn = it1->second;
	sid_conns_.erase(it1);
	addr_conn_map_t::iterator it2 = addr_conns_.find(conn->addr_);
	if (it2 != addr_conns_.end()) {
		addr_conns_.erase(it2);
	}
	udp_conn_pool_->free(conn);
	--conn_num_;
}

udp_connection_t* udp_network_t::get_connection_addr(int64 addr) {
	addr_conn_map_t::iterator it = addr_conns_.find(addr);
	if (it == addr_conns_.end()) {
		return NULL;
	}
	return it->second;
}

udp_connection_t* udp_network_t::get_connection_sid(int sid) {
	sid_conn_map_t::iterator it = sid_conns_.find(sid);
	if (it == sid_conns_.end()) {
		return NULL;
	}
	return it->second;
}

int udp_network_t::send_sid(int sid, void *buff, int size) {
	udp_connection_t *conn = get_connection_sid(sid);
	if (conn == NULL) {
		return 0;
	}
	return conn->send(buff, size);
}

int udp_network_t::start(const char *ip, int port) {
	addr_ = net_address_t(ip, port);
	socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd_ < 0) {
		return -1;
	}
	if (bind(socket_fd_, (struct sockaddr*)&addr_, sizeof(addr_)) < 0) {
		return -1;
	}
	if (single_select_ != NULL) {
		delete single_select_;
		single_select_ = NULL;
	}
	single_select_ = new single_select_t(socket_fd_);
	return 0;
}

void udp_network_t::process(int64 tick) {
	tick_ = tick;
	// net recv
	struct sockaddr_in address;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	int addr;
	int len;
	while (1) {
		if (cur_recv_chunk_ == NULL) {
			cur_recv_chunk_ = chunk_pool_malloc(udp_chunk_pool_);
		}
		if (!single_select_->read_check()) {
			break;
		}
		len = recvfrom(socket_fd_, cur_recv_chunk_, UDP_HEAD_BYTE_ALL + UDP_DATA_MAX_LEN, 0, (struct sockaddr*)&address, &addr_len);
		if (len <= 0) {
			break;
		}
		if (len != cur_recv_chunk_->size + UDP_HEAD_BYTE_ALL) {
			// ERROR
			continue;
		}
		addr = NET_ADDR_TO_INT(address);
		udp_connection_t *conn =  get_connection_addr(addr);
		if (conn == NULL) {
			if (cur_recv_chunk_->type == UDP_TYPE_CONNECT) {
				udp_connection_t *conn = udp_conn_pool_->alloc();
				new (conn) udp_connection_t(this, address, cur_recv_chunk_->ack);
				add_connection(conn);
			}
			continue;
		}
		if (cur_recv_chunk_ != NULL) {
			conn->on_recv_udp_chunk(cur_recv_chunk_);
			cur_recv_chunk_ = NULL;
		}
	}
	// conn process
	for (addr_conn_map_t::iterator it = addr_conns_.begin(); it != addr_conns_.end(); ) {
		udp_connection_t *conn = it->second;
		if (conn->process() < 0) {
			++it;
			remove_connection_addr(conn->get_addr());
			continue;
		}
		++it;
	}
}