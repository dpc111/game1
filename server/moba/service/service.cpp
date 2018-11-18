#include "service.h"
#include "player_mgr.h"
#include "room_mgr.h"
#include "dispatch_msg.h"

service_t::service_t()
	: server_t() {
	player_mgr_ = new player_mgr_t();
	room_mgr_ = new room_mgr_t();
	dispatch_ = new dispatch_msg_t(this);
}
	
service_t::~service_t() {
	delete player_mgr_;
	player_mgr_ = NULL;
	delete room_mgr_;
	room_mgr_ = NULL;
	delete dispatch_;
	dispatch_ = NULL;
}

void service_t::init() {
	server_t::init();
	dispatch_->register_client_msg();
	dispatch_->register_server_msg();
	dispatch_->register_udp_msg();
	set_frame_interval(20);
}

void service_t::init_json_mgr() {
	server_t::init_json_mgr();
	get_json_mgr()->load("../etc/config/json/entity.json", "entity");
	get_json_mgr()->load("../etc/config/json/bullet.json", "bullet");
}

void service_t::udp_on_connect(int32 uid) {
	server_t::udp_on_connect(uid);
	ERROR("%d", uid);
	player_t *player = get_player_mgr()->get_player(uid);
	if (player) {
		return;
	}
	player = get_player_mgr()->create_player(uid);		
	room_t *room = player->get_room();
	if (room) {
		return;
	}
	room = get_room_mgr()->create_room();
	room->add_player(player);
	player->set_room(room);
}

void service_t::udp_on_disconnect(int32 uid) {
	server_t::udp_on_disconnect(uid);
	ERROR("%d", uid);
	player_t *player = get_player_mgr()->get_player(uid);
	if (!player) {
		return;
	}
	room_t *room = player->get_room();
	if (room) {
		room->del_player(player->get_uid());
		if (!room->has_player()) {
			get_room_mgr()->del_room(room->get_rid());
		}
		player->set_room(NULL);
	}
	get_player_mgr()->del_player(player->get_uid());
}

void service_t::update(int64 tm) {
	room_mgr_->update(tm);
}