#include "client_msg.h"
#include "service.h"
#include "global.h"
#include "player_mgr.h"
#include "player.h"

client_msg_t::client_msg_t() {

}

client_msg_t::~client_msg_t() {
	
}

void client_msg_t::s2c_join(tcp_connection_t *conn, const battle_msg::s2c_join& msg) {
	
}

void client_msg_t::c2s_join(tcp_connection_t *conn, const battle_msg::c2s_join& msg) {
	int64 uid = 1111;
	player_t *player = get_player_mgr()->get_player(uid);
	if (player) {
		return;
	}
	player = new player_t(uid, "dpc", 1, "null");
	get_player_mgr()->add_player(uid, player);

	battle_msg::s2c_join res;
	res.set_uid(uid);
	res.set_name(get_json_mgr()->get_string("battle", 5, "roomName"));
	res.set_icon("null");
	get_service()->send(conn, res);
}

void client_msg_t::c_login(tcp_connection_t *conn, const battle_msg::c_login& msg) {
	battle_msg::s_login res;
	int64 uid = msg.uid;
	player_t *player = get_player_mgr()->get_player(uid)
	if (player) {
		res.set_res("ok");
		res.set_uid(player->get_uid());
		res.set_name(player->get_name());
		res.set_icon(player->get_icon());
		res.set_camp(player->get_camp());
		get_service()->send(conn, res);
		return;
	}
	player = get_player_mgr()->create_player(uid);
	if (!player) {
		res.set_res("fail");
		get_service()->send(conn, res);
		return;
	}
	room_t *room = get_room_mgr()->create_room();
	if (!room) {
		return;
	}
	palyer->set_room(room);
	int32 camp = room->set_random_camp(player->get_uid());
	player->set_camp(camp);
	res.set_res("ok");
	res.set_uid(player->get_uid());
	res.set_name(player->get_name());
	res.set_icon(player->get_icon());
	res.set_camp(player->get_camp());
	get_service()->send(conn, res);
	get_service()->verify((int32)uid, conn);
}