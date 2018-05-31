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
	pos_.x = begin_pos_.x + v_speed_.x * interval;
	pos_.y = begin_pos_.y + v_speed_.y * interval;
	pos_.z = begin_pos_.z;
	bullet_t::update(stm);
}
