#include "udp_network.h"

udp_network_t::udp_network_t() {
	udp_handle_pool_ = new udp_handle_pool_t;
	udp_conn_pool_ = new udp_connection_pool_t;
	conn_num_ = 0;
}

udp_network_t::~udp_network_t() {
	delete udp_handle_pool_;
	conn_num_ = 0;
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
	udp_conn_pool_.free(conn);
}

void udp_network_t::process() {

}