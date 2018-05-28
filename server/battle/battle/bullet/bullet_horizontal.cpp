#include "bullet_horizontal.h"

bullet_horizontal_t::bullet_horizontal_t(room_t	*room, int32 bullet_id, int32 type_id) 
	: bullet_t(room, bullet_id, type_id) {

}

bullet_horizontal_t::~bullet_horizontal_t() {

}

void bullet_horizontal_t::update(float stm) {
	if (del_) {
		return;
	}
	float interval = stm - begin_tm_;
	pos_ = begin_pos_ + v_speed_ * interval;
	bullet_t::update(stm);
}
