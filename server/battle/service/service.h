#ifndef SERVICE_H
#define SERVICE_H

#include "server.h"

class player_mgr_t;
class room_mgr_t;
class dispatch_msg_t;

class service_t : public server_t {
public:
	service_t();

	~service_t();

	static service_t* get_service() {
		if (!service_) {
			service_t::service_ = new service_t();
		}
		return service_t::service_;
	}

	void init();

	player_mgr_t* get_player_mgr() { return player_mgr_; }

	room_mgr_t* get_room_mgr() { return room_mgr_; }

private:
	static service_t* service_;

	player_mgr_t *player_mgr_;
	
	room_mgr_t *room_mgr_;

	dispatch_msg_t *dispatch_;
};

#endif