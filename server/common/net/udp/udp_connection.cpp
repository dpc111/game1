#include "udp_connection.h"

udp_connection_t::udp_connection_t(udp_network_t *network, struct sockaddr_in& addr, int sid) {
	network_ = network;
	udp_handle_ = network->get_udp_handle_pool()->alloc();
	address_ = addr;
	addr_ = NET_ADDR_TO_INT(addr);
	sid_ = sid;
}

udp_connection_t::~udp_connection_t() {
	network_ = NULL;
	if (udp_handle_ != NULL) {
		network->get_udp_handle_pool()->free(udp_handle_);		
		udp_handle_ = NULL;
	}
}