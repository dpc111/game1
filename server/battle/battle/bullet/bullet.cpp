#include "bullet.h"
#include "room.h"
#include "grid.h"

bullet_t::bullet_t(room_t *room, int32 bullet_id, int32 type_id) :
	object_base_t(bullet_id, type_id),
	room_(room),
	speed_(0),
	del_(false),
	begin_tm_(0),
	v_speed_(0, 0, 0),
	begin_pos_(0, 0, 0),
	pos_(0, 0, 0),
	line_(0) {
}

bullet_t::~bullet_t() {

}

void bullet_t::update(double stm) {
	if (del_) {
		return;
	}
	if (stm - begin_tm_ > MAX_LIFE_TM) {
		ERROR("");
		set_del(true);
		return;
	}
	if (del_) {
		return;
	}
	// collision
	room_->get_grid()->process_collision(this);
}

void bullet_t::on_collision(entity_t *entity) {
	set_del(true);
}

void bullet_t::on_bullet_out() {
	set_del(true);
}