#include "room.h"
#include "entity_mgr.h"
#include "bullet_mgr.h"
#include "entity.h"
#include "grid.h"

room_t::room_t(int32 rid) {
	rid_ = rid;
	entity_mgr_ = new entity_mgr_t(this);
	bullet_mgr_ = new bullet_mgr_t(this);
	grid_ = new grid_t(this);
	for (int32 k = 0; k < 2; k++) {
		camps_[k] = 0;
	}
}

room_t::~room_t() {
	delete entity_mgr_;
	delete bullet_mgr_;
	delete grid_;
	for (int32 k = 0; k < 2; k++) {
		camps_[k] = 0;
	}
}

int32 room_t::set_random_camp(int64 uid) {
	for (int32 k = 0; k < 2; k++) {
		if (camps_[k] == 0) {
			camps_[k] = uid;
			return k + 1;
		}
	}
	return 0;
}