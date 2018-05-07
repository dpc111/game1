#include "client_msg.h"

client_msg_t::client_msg_t() {

}

client_msg_t::~client_msg_t() {
	
}

void client_msg_t::s2c_join(tcp_connection_t *conn, const battle::s2c_join& msg) {
	
}

void client_msg_t::c2s_join(tcp_connection_t *conn, const battle::c2s_join& msg) {
	ERROR("");
	battle::s2c_join res;
	res.set_uid(100001);
	res.set_name("dpc");
	res.set_uid("null");
	get_service()->send(conn, res);
}

