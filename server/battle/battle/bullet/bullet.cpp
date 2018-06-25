#include "bullet.h"
#include "room.h"
#include "grid.h"

bullet_t::bullet_t(room_t *room, int32 bullet_id, int32 type_id) :
	object_base_t(bullet_id, type_id),
	room_(room),
	speed_(0),
	del_(false),
	begin_time_(0),
	v_speed_(0, 0, 0),
	begin_pos_(0, 0, 0),
	pos_(0, 0, 0),
	line_(0),
	damage_r_(4) {
}

bullet_t::~bullet_t() {

}

void bullet_t::init(entity_t *entity) {
	set_path(get_json_mgr()->get_int("bullet", get_type_id() - 1, "path"));
	set_speed(get_json_mgr()->get_int("bullet", get_type_id() - 1, "speed"));
	set_camp(entity->get_camp());
	set_begin_time(getfs());
	set_begin_pos(entity->get_gun_pos() + entity->get_pos());
	set_pos(entity->get_gun_pos() + entity->get_pos());
	set_damage(entity->get_damage());
	set_line(entity->get_col());
}

void bullet_t::update(double stm) {
	if (del_) {
		return;
	}
	if (stm - begin_time_ > BULLET_MAX_LIFE_TM) {
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