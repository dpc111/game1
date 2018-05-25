#include "bullet.h"

bullet_t::bullet_t(int32 bullet_id, int32 type_id) :
	object_base_t(bullet_id, type_id),
	speed_(0),
	del_(false),
	begin_tm_(0),
	v_speed_(0, 0),
	begin_pos_(0, 0),
	pos_(0, 0) {
}

bullet_t::~bullet_t() {

}

void bullet_t::update(float stm) {
	if (del_) {
		return;
	} 
	if (stm - begin_tm_ > MAX_LIFE_TM) {
		ERROR("");
		del_ = true;
		return;
	}
	// collision
}