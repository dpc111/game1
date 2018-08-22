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

void udp_connection_t::process() {
	while (1) {
		udp_chunk_t *c = recv_buff_out(udp_handle_);
		if (c == NULL) {
			break;
		}
		if (network_->udp_msg_cb_ != NULL) {
			network_->udp_msg_cb_((void *)c->buff, c->size);
		}
		recv_buff_out_process(udp_handle_);
	}
}
