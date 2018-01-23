#include "msg_dispatch.h"
#include "log.h"
#include "tcp_connection.h"
#include "tcp_network.h"

msg_dispatch_t::msg_dispatch_t(tcp_network_t *network)
	: network_(network)
	, on_script_func_(NULL) {
}

msg_dispatch_t::~msg_dispatch_t() {
	network_ = NULL;
	on_script_func_ = NULL;
}

void msg_dispatch_t::on_message(tcp_connection_t *conn, google::protobuf::Message *msg) {
	std::string name = msg->GetDescriptor()->full_name();
	const google::protobuf::Descriptor* des = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(name.c_str());
	msg_map_t::iterator it = msg_map_.find(des);
	if (it != msg_map_.end()) {
		if (conn->get_sid() > 0) {
			cb_t *cb = it->second;
			cb->on_message(conn->get_sid(), msg);
		} else {
			ERROR("connection not verify");
		}
	}
	msg_net_map_t::iterator it1 = msg_net_map_.find(des);
	if (it1 != msg_net_map_.end()) {
		cb_net_t *cb = it1->second;
		cb->on_message(conn, msg);
	}
	ERROR("msg call back not found");
}

bool msg_dispatch_t::on_script_func(tcp_connection_t *conn, const char *name) {
	if (on_script_func_) {
		return on_script_func_(conn, name);
	}
	return false;
}
