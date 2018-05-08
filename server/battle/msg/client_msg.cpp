#include "client_msg.h"
#include "service.h"
#include "global.h"
#include "stdafx.h"

client_msg_t::client_msg_t() {

}

client_msg_t::~client_msg_t() {
	
}

void client_msg_t::s2c_join(tcp_connection_t *conn, const battle::s2c_join& msg) {
	
}

void client_msg_t::c2s_join(tcp_connection_t *conn, const battle::c2s_join& msg) {
	int64 uid = msg.uid();
	player_t *player = get_player_mgr()->get_player(uid);
	if (player) {
		return;
	}
	player = new player_t(uid, "dpc", 1, "null");
	get_player_mgr()->add_player(uid, player);

	battle::s2c_join res;
	res.set_uid(uid);
	res.set_name("dpc");
	res.set_icon("null");
	get_service()->send(conn, res);
}

