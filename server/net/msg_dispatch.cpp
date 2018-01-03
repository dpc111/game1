#include "msg_dispatch.h"

msg_dispatch_t::msg_dispatch_t(tcp_network_t *network) {
	network_ = network;
}

msg_dispatch_t::~msg_dispatch_t() {
}

int msg_dispatch_t::msg_id(std::string& name) {
	msg_name_map_t::iterator it = name_map_.find(name);
	if (it == name_map_.end()) {
		return 0;
	}
	return it->second;
}

const char *msg_dispatch_t::msg_name(int id) {
	msg_id_map_t::iterator it = id_map_.find(id);
	if (it == id_map_.end()) {
		return NULL;
	}
	return (it->second).c_str();
}

void msg_dispatch_t::on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg) {
	
}