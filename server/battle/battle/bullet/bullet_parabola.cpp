#include "bullet_parabola.h"

bullet_parabola_t::bullet_parabola_t(int32 bullet_id, int32 type_id) 
	: bullet_t(bullet_id, type_id) {

}

bullet_parabola_t::~bullet_parabola_t() {

}

void bullet_parabola_t::update(float stm) {
	bullet_t::update(stm);
	float interval = stm - begin_tm_;
	pos_.x = begin_pos_.x + v_speed_.x * interval;
	pos_.y = pos_.y + v * interval + 0.5 * GRAVITY * interval * interval;
}
