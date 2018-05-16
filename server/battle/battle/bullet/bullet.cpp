#include "bullet.h"

bullet_t::bullet_t(int32 bullet_id, int32 type_id) :
	object_base_t(bullet_id, type_id) {
	speed_ = 0;
}

bullet_t::~bullet_t() {
	speed_ = 0;
}