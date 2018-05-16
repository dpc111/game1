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
	delete room_mgr_;
}

void service_t::init() {
	server_t::init();
	dispatch_->register_client_msg();
	dispatch_->register_server_msg();
}

void service_t::init_json_mgr() {
	server_t::init_json_mgr();
	get_json_mgr()->load("entiy.json", "entiy");
	get_json_mgr()->load("bullet.json", "bullet");
}