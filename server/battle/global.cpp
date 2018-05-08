#include "player_mgr.h"
#include "room_mgr.h"
#include "service.h"

service_t *g_service_ = new service_t();

player_mgr_t* get_player_mgr() {
	return get_service()->get_player_mgr();
}

room_mgr_t* get_room_mgr() {
	return get_service()->get_room_mgr();
}

service* get_service() {
	return g_service_;
}
