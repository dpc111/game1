#include "service.h"

service_t::service_t() {
	player_mgr_ = new player_mgr_t();
	room_mgr_ = new room_mgr_t();
}
	
service_t::~service_t() {

}