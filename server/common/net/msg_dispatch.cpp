// #include "msg_dispatch.h"
// #include "log.h"
// #include "tcp_connection.h"
// #include "tcp_network.h"

// msg_dispatch_t::msg_dispatch_t(tcp_network_t *network) {
// 	network_ = network;
// 	on_script_func_ = NULL;
// }

// msg_dispatch_t::~msg_dispatch_t() {
// 	network_ = NULL;
// 	on_script_func_ = NULL;
// }

// int msg_dispatch_t::msg_id(const char *name) {
// 	name_map_t::iterator it = names_.find(name);
// 	if (it == names_.end()) {
// 		return 0;
// 	}
// 	return it->second;
// }

// const char *msg_dispatch_t::msg_name(int id) {
// 	msg_map_t::iterator it = msgs_.find(id);
// 	if (it == msgs_.end()) {
// 		return NULL;
// 	}
// 	return it->second->get_name();
// }

// void msg_dispatch_t::on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg) {
// 	msg_map_t::iterator it = msgs_.find(msgid);
// 	if (it == msgs_.end()) {
// 		ERROR("");
// 		return;
// 	}
// 	cb_t *cb = it->second;
// 	if (conn->get_sid() > 0) {
// 		cb->on_message(conn->get_sid(), msg);
// 	}
// 	cb->on_net_message(conn, msg);
// }

// bool msg_dispatch_t::on_script_func(tcp_connection_t *conn) {
// 	if (on_script_func_) {
// 		return on_script_func_(conn);
// 	}
// 	return false;
// }

// template<typename T>
// void msg_dispatch_t::register_message(const char *name, const typename cbT_t<T>::msg_cb_t& cbfun) {
// 	int msgid = msg_id(name);
// 	if (msgid == 0) {
// 		ERROR("");
// 		return;
// 	}
// 	msg_map_t::iterator it = msgs_.find(msgid);
// 	if (it != msgs_.end()) {
// 		cb_t *cb = it->second;
// 		cb->set_msg_cb(cbfun);
// 		return;
// 	}
// 	cbT_t<T> *cb = new cbT_t<T>();
// 	cb->set_msg_cb(cbfun);
// 	cb->set_name(name);
// }

// template<typename T>
// void msg_dispatch_t::register_net_message(const char *name, const typename cbT_t<T>::net_msg_cb_t& cbfun) {
// 	int msgid = msg_id(name);
// 	if (msgid == 0) {
// 		ERROR("");
// 		return;
// 	}
// 	msg_map_t::iterator it = msgs_.find(msgid);
// 	if (it != msgs_.end()) {
// 		cb_t *cb = it->second;
// 		cb->set_net_msg_cb(cbfun);
// 		return;
// 	}
// 	cbT_t<T> *cb = new cbT_t<T>();
// 	cb->set_net_msg_cb(cbfun);
// 	cb->set_name(name);
// }


/////////////////////////////////////////////////////////////////////////
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
