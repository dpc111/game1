#include "room.h"
#include "entity_mgr.h"
#include "bullet_mgr.h"
#include "entity.h"
#include "bullet.h"
#include "grid.h"
#include "player.h"
#include "service.h"
#include "global.h"

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
	del_ = false;
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
	if (del_ || room_state_ != ROOM_STATE_ING) {
		return;
	}
	entity_mgr_->update(tm);
	bullet_mgr_->update(tm);
}

void room_t::register_callback() {
	register_message<battle_msg::c_create_entity>(REGISTER_ROOM_BIND(room_t::c_create_entity));
}

void room_t::c_create_entity(void *player, const battle_msg::c_create_entity& msg) {
	player_t *p = (player_t *)player;
}

void room_t::start_wait() {
	ERROR("room wait");
	room_state_ = ROOM_STATE_WAIT;
	get_service()->register_delay_stimer(this, NULL, ROOM_WAIT_TIME, 0);
}

void room_t::start_ing() {
	ERROR("room ing");
	room_state_ = ROOM_STATE_ING;
	get_service()->register_delay_stimer(this, NULL, ROOM_ING_TIME, 0);
}

void room_t::start_end() {
	ERROR("room end");
	room_state_ = ROOM_STATE_END;
	get_service()->register_delay_stimer(this, NULL, ROOM_END_TIME, 0);
}

void room_t::handle_timeout(timer_handle_t handle, void *user) {
	if (room_state_ == ROOM_STATE_WAIT) {
		start_ing();
	}
	if (room_state_ == ROOM_STATE_ING) {
		start_end();
	}
	if (room_state_ == ROOM_STATE_END) {
		set_del(true);
	}
}

void room_t::on_create_entity(entity_t *entity) {

}

void room_t::on_del_entity(entity_t *entity) {

}

void room_t::on_create_bullet(entity_t *entity, bullet_t *bullet) {

}

void room_t::on_del_bullet(bullet_t *bullet) {

}

void room_t::on_entity_damage(entity_t *entity, int32 damage) {

}

void room_t::on_fire(entity_t *entity, bullet_t *bullet) {

}

void room_t::on_collision(entity_t *entity, bullet_t *bullet) {

}