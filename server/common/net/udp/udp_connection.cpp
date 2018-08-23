#include "udp_connection.h"
#include "udp_network.h"
#include "log.h"

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

void udp_connection_t::on_recv_udp_chunk(udp_chunk_t *c) {
	// net recv
	recv_buff_write(udp_handle_, c);
}

int udp_connection_t::send(void *buff, int size) {
	if (size > UDP_DATA_MAX_LEN) {
		return -1;
	}
	send_buff_write(udp_handle_, buff, size);
	return size;
}

int udp_connection_t::send_chunk_force(udp_chunk_t *c) {
	if (c->size > UDP_DATA_MAX_LEN) {
		return -1;
	}
	udp_send_chunk_force(udp_handle_, c);
	return c->size;
}

int udp_connection_t::process() {
	// udp hand process
	int res = udp_process(udp_handle_, network_->get_tick());
	if (res < 0){
		return res;
	}
	// logic callback
	udp_chunk_t *c = recv_buff_out(udp_handle_);
	while (c != NULL) {
		if (network_->udp_msg_cb_ != NULL) {
			network_->udp_msg_cb_(sid_, (void *)c->buff, c->size);
		}
		LOG("%d, %d", sid_, c->seq);
		recv_buff_out_process(udp_handle_);
		c = recv_buff_out(udp_handle_);
	}
	// net send
	if (!network_->get_block()) {
		c = send_buff_out(udp_handle_);
		while (c != NULL) {
			if (!network_->single_select_->write_check()) {
				break;
			}
			int n = sendto(network_->socket_fd_, c, c->size + UDP_HEAD_BYTE_ALL, 0, (struct sockaddr*)&address_, sizeof(address_));
			if (n != c->size + UDP_HEAD_BYTE_ALL) {
				network_->set_block(true);
				break;
			}
			send_buff_out_process(udp_handle_);
			c = send_buff_out(udp_handle_);
		}
	}

	return 0;
}
