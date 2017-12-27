#include "msg_dispatch.h"

msg_dispatch_t::msg_dispatch_t(tcp_network_t *network) {
	network_ = network;
}

msg_dispatch_t::~msg_dispatch_t() {
}

void msg_dispatch_t::on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg) {
	
}