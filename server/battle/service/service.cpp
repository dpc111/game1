#include "service.h"
#include "player_mgr.h"
#include "room_mgr.h"

service_t::service_t() 
	: server_t() {
	player_mgr_ = new player_mgr_t();
	room_mgr_ = new room_mgr_t();
}
	
service_t::~service_t() {
	delete player_mgr_;
	delete room_mgr_;
}