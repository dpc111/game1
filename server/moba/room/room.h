#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"
#include "dispatcher.h"
#include "message/messages.h"
#include "timer_handle.h"
#include "obj_pro_def.h"
#include <map>

#define ROOM_MAX_PLAYER_NUM 2
#define ROOM_MAX_GAME_TIME 600000

class frame_mgr_t;
class player_t;

class room_t : public dispatcher_t, timer_handle_t {
public:
	typedef std::map<int64, player_t *> player_map_t;

public:
	room_t(int32 rid);

	~room_t();

	void start();

	void end();

	void update(int64 ms);

	bool is_over() { return over_; }

	void add_player(int64 uid, player_t *player);

	void del_player(int64 uid);
	
private:
	PROPERTY(int32, 							rid)
	PROPERTY(int64, 							start_time)
	PROPERTY(bool, 								over)
	PROPERTY(int32, 							player_num)
	PROPERTY_QUOTE(player_map_t, 				players)
	PROPERTY(frame_mgr_t *, 					frame_mgr)
};

#endif