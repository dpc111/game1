#include "bullet_parabola.h"

bullet_parabola_t::bullet_parabola_t(room_t *room, int32 bullet_id, int32 type_id) 
	: bullet_t(room, bullet_id, type_id) {

}

bullet_parabola_t::~bullet_parabola_t() {

}

void bullet_parabola_t::init(entity_t * entity) {
	bullet_t::init(entity);
	set_v_speed(vector3_t(0, get_speed(), 0));
	set_begin_v_speed(get_v_speed());
}

void bullet_parabola_t::update(double stm) {
	if (del_) {
		return;
	}
	double interval = stm - begin_time_;
	pos_.x = begin_pos_.x + v_speed_.x * interval;
	pos_.y = pos_.y + v_speed_.y * interval + 0.5 * GRAVITY * interval * interval;
	pos_.z = pos_.z;
	bullet_t::update(stm);
}

void bullet_horizontal_t::update_speed() {

}