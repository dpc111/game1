#include "msg_dispatch.h"

msg_dispatch_t::msg_dispatch_t(tcp_network_t *network) {
	network_ = network;
}

msg_dispatch_t::~msg_dispatch_t() {
}

int msg_dispatch_t::msg_id(const char *name) {
	name_map_t::iterator it = names_.find(name);
	if (it == names_.end()) {
		return 0;
	}
	return it->second;
}

const char *msg_dispatch_t::msg_name(int id) {
	msg_map_t::iterator it = msgs_.find(id);
	if (it == msgs_.end()) {
		return NULL;
	}
	return it->second->name_;
}

void msg_dispatch_t::on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg) {
	
}