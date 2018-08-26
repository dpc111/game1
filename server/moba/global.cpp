#include "global.h"
#include "player_mgr.h"
#include "room_mgr.h"
#include "service.h"
#include "cfg_json_mgr.h"

thread_private_obj_t<service_t> g_service;

player_mgr_t* get_player_mgr() {
	return g_service.self()->get_player_mgr();
}

room_mgr_t* get_room_mgr() {
	return g_service.self()->get_room_mgr();
}

cfg_json_mgr_t* get_json_mgr() {
	return g_service.self()->get_json_mgr();
}

service_t* get_service() {
	return g_service.self();
}
