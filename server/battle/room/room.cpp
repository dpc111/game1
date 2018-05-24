#include "room.h"
#include "entity_mgr.h"
#include "bullet_mgr.h"
#include "entity.h"
#include "grid.h"
#include "player.h"

#define REGISTER_ROOM_BIND(func) \
	std::tr1::bind(&func, \
	this, \
	std::tr1::placeholders::_1, \
	std::tr1::placeholders::_2)

room_t::room_t(int32 rid) :
	dispatcher_t() {
	rid_ = rid;
	entity_mgr_ = new entity_mgr_t(this);
	bullet_mgr_ = new bullet_mgr_t(this);
	grid_ = new grid_t(this);
	room_state_ = ROOM_STATE_WAIT;
	for (int32 k = 0; k < 2; k++) {
		camps_[k] = 0;
	}
}

room_t::~room_t() {
	delete entity_mgr_;
	delete bullet_mgr_;
	delete grid_;
	for (int32 k = 0; k < 2; k++) {
		camps_[k] = 0;
	}
}

int32 room_t::set_random_camp(int64 uid) {
	for (int32 k = 0; k < 2; k++) {
		if (camps_[k] == 0) {
			camps_[k] = uid;
			return k + 1;
		}
	}
	return 0;
}

void room_t::update(int64 tm) {
	bullet_mgr_->update(tm);
}

void room_t::c_create_entity(void *player, const battle_msg::c_create_entity& msg) {
	player_t *p = (player_t *)player;
}

void room_t::register_callback() {
	register_message<battle_msg::c_create_entity>(REGISTER_ROOM_BIND(room_t::c_create_entity));
}
