#include "bullet_mgr.h"
#include "bullet.h"
#include "global.h"
#include "room.h"

bullet_mgr_t::bullet_mgr_t(room_t *room) :
	object_mgr_base_t(),
	room_(room) {

}

bullet_mgr_t::~bullet_mgr_t() {

}

bullet_t *bullet_mgr_t::create_bullet(int32 type_id) {
	int32 bullet_id = gen_id();
	if (get_object(bullet_id)) {
		return NULL;
	}
	if (!get_json_mgr()->exist("bullet", type_id - 1)) {
		return NULL;
	}
	bullet_t *bullet = new bullet_t(bullet_id, type_id);
	bullet->set_speed(get_json_mgr()->get_int("bullet", type_id - 1, "speed"));
	add_object(bullet_id, bullet);
}

void bullet_mgr_t::delete_bullet(int32 bullet_id) {
	bullet_t *bullet = (bullet_t *)get_object(bullet_id);
	if (!bullet) {
		return;
	}
	delete bullet;
	del_object(bullet_id);
}

void bullet_mgr_t::update(int64 tm) {
	for (object_map_t::iterator it = objects_.begin(); it != objects_.end(); it++) {
		bullet_t *bullet = (bullet_t *)it->second;
		bullet->update(tm);
	}
}