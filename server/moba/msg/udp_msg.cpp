#include "udp_msg.h"
#include "player_mgr.h"
#include "player.h"
#include "room_mgr.h"
#include "room.h"
#include "frame_mgr.h"

udp_msg_t::udp_msg_t() {

}

udp_msg_t::~udp_msg_t() {

}

void udp_msg_t::on_message(int sid, void *buff, int size) {
	player_t *player = get_player_mgr()->get_player(sid);
	if (!player) {
		return;
	}
	room_t *room = player->get_room();
	if (!room) {
		return;
	}
	room->get_frame_mgr()->recv(sid, buff, size);
}
