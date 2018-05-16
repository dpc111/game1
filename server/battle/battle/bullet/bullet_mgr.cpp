#include "bullet_mgr.h"
#include "bullet.h"

bullet_mgr_t::bullet_mgr_t() :
	object_mgr_base_t() {

}

bullet_mgr_t::~bullet_map_t() {

}

bullet_t *bullet_mgr_t::create_bullet(int32 type_id) {
	int32 bullet_id = gen_id()
	if (get_object(bullet_id)) {
		return NULL;
	}
	bullet_t *bullet = new bullet_t(bullet_id, type_id);
	add_object(bullet_id, bullet);
}

void bullet_mgr_t::delete_bullet(int32 bullet_id) {
	bullet_t *bullet = get_object(bullet_id);
	if (!bullet) {
		return;
	}
	delete bullet;
	del_object(bullet_id);
}