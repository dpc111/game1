#include "client_msg.h"
#include "service.h"
#include "global.h"
#include "player_mgr.h"
#include "player.h"
#include "room_mgr.h"
#include "room.h"

client_msg_t::client_msg_t() {

}

client_msg_t::~client_msg_t() {
	
}

void client_msg_t::c_login_hall(tcp_connection_t *conn, const battle_msg::c_login_hall& msg) {
}

void client_msg_t::c_login(tcp_connection_t *conn, const battle_msg::c_login& msg) {
}

void client_msg_t::default_message(int32 uid, const google::protobuf::Descriptor* des, const google::protobuf::Message *msg) {
}
