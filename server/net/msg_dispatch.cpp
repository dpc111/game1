#include "msg_dispatch.h"
#include "log.h"

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
	return it->second->get_name();
}

void msg_dispatch_t::on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg) {
	msg_map_t::iterator it = msgs_.find(msgid);
	if (it == msgs_.end()) {
		ERROR();
		return;
	}
	cb_t *cb = it->second;
	if (conn->get_sid() > 0) {
		cb->on_message(conn->get_sid(), msg);
	}
	cb->on_net_message(conn, msg);
}

template<typename T>
void msg_dispatch_t::register_message(const char *name, const typename cbT_t<T>::msg_cb_t& cbfun) {
	int msgid = msg_id(name);
	if (msgid == 0) {
		ERROR();
		return;
	}
	msg_map_t::iterator it = msgs_.find(msgid);
	if (it != msgs_.end()) {
		cb_t *cb = it->second;
		cb->set_msg_cb(cbfun);
		return;
	}
	cbT_t<T> *cb = new cbT_t<T>();
	cb->set_msg_cb(cbfun);
	cb->set_name(name);
}

template<typename T>
void msg_dispatch_t::register_net_message(const char *name, const typename cbT_t<T>::net_msg_cb_t& cbfun) {
	int msgid = msg_id(name);
	if (msgid == 0) {
		ERROR();
		return;
	}
	msg_map_t::iterator it = msgs_.find(msgid);
	if (it != msgs_.end()) {
		cb_t *cb = it->second;
		cb->set_net_msg_cb(cbfun);
		return;
	}
	cbT_t<T> *cb = new cbT_t<T>();
	cb->set_net_msg_cb(cbfun);
	cb->set_name(name);
}