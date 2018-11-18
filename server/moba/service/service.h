#ifndef SERVICE_H
#define SERVICE_H

#include "server.h"
#include "stdafx.h"

class player_mgr_t;
class room_mgr_t;
class dispatch_msg_t;

class service_t : public server_t {
public:
	service_t();

	~service_t();

	virtual void init();

	virtual void init_json_mgr();

	virtual void update(int64 tm);

	virtual void udp_on_connect(int32 uid);

	virtual void udp_on_disconnect(int32 uid);

	player_mgr_t* get_player_mgr() { return player_mgr_; }

	room_mgr_t* get_room_mgr() { return room_mgr_; }

private:
	player_mgr_t *player_mgr_;
	
	room_mgr_t *room_mgr_;

	dispatch_msg_t *dispatch_;
};

#endif
