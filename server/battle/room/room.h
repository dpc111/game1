#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"
#include "dispatcher.h"
#include "message/messages.h"

#define ROOM_STATE_WAIT 1
#define ROOM_STATE_ING 2

class entity_mgr_t;
class bullet_mgr_t;
class grid_t;

class room_t : public dispatcher_t {
public:
	room_t(int32 rid);

	~room_t();

	entity_mgr_t* get_entity_mgr() { return entity_mgr_; }

	bullet_mgr_t* get_bullet_mgr() { return bullet_mgr_; }

	grid_t* get_grid() { return grid_; }

	int32 set_random_camp(int64 uid);

	void c_create_entity(void *player, const battle_msg::c_create_entity& msg);

	virtual void register_callback();

	void update(int64 tm);

private:
	int32 rid_;

	entity_mgr_t *entity_mgr_;

	bullet_mgr_t *bullet_mgr_;

	grid_t *grid_;
	
	int64 camps_[2];

	int32 room_state_;
};

#endif