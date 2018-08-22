#include "udp_connection.h"
#include "udp_network.h"

udp_connection_t::udp_connection_t(udp_network_t *network, struct sockaddr_in& addr, int sid) {
	network_ = network;
	udp_handle_ = network->get_udp_handle_pool()->alloc();
	init_udp_handle(udp_handle_, network->get_udp_chunk_pool(), network->get_tick());
	address_ = addr;
	addr_ = NET_ADDR_TO_INT(addr);
	sid_ = sid;
}

udp_connection_t::~udp_connection_t() {
	network_ = NULL;
	if (udp_handle_ != NULL) {
		destroy_udp_handle(udp_handle_);
		network_->get_udp_handle_pool()->free(udp_handle_);		
		udp_handle_ = NULL;
	}
}

int udp_connection_t::send(void *buff, int size) {
	if (size > UDP_DATA_MAX_LEN) {
		return -1;
	}
	send_buff_write(udp_handle_, buff, size);
	return size;
}
