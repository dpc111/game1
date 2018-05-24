#include "bullet.h"

bullet_t::bullet_t(int32 bullet_id, int32 type_id) :
	object_base_t(bullet_id, type_id) {
	speed_ = 0;
	create_tm_ = 0;
	del_ = false;
}

bullet_t::~bullet_t() {
	speed_ = 0;
	create_tm_ = 0;
}

void bullet_t::update(float tm) {
	if (del_) {
		return;
	} 
	if (tm - create_tm_ > MAX_LIFE_TM) {
		ERROR("");
		del_ = true;
		return;
	}
}