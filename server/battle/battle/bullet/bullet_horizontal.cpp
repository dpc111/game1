#include "bullet_horizontal.h"

bullet_horizontal_t::bullet_horizontal_t(int32 bullet_id, int32 type_id) 
	: bullet_t(bullet_id, type_id) {

}

bullet_horizontal_t::~bullet_horizontal_t() {

}

void bullet_horizontal_t::update(float stm) {
	bullet_t::update(stm);
	float interval = stm - begin_tm_;
	pos_ = begin_pos_ + interval * v_speed_;
}
