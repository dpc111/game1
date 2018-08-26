#include "udp_msg.h"
#include "player_mgr.h"
#include "player.h"
#include "room_mgr.h"
#include "room.h"

udp_msg_t::udp_msg_t() {

}

udp_msg_t::~udp_msg_t() {

}

void udp_msg_t::on_message(int32 sid, void *buff, void size) {
	player_t *player = get_player_mgr()->get_player(sid);
	if (!player) {
		return;
	}
	room_t *room = get_room_mgr()->get_room(player->get_rid());
	if (!room) {
		return;
	}
	room->get_frame_mgr()->recv(sid, buff, size);
}