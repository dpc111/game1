#include "entity.h"
#include "room.h"

entity_t::entity_t(room_t *room, int32 entity_id, int32 type_id) :
	object_base_t(entity_id, type_id) {
	room_ = room;
	del_ = false;
	level_ = 1;
	cd_ = 1;
	last_fire_tm_ = 0;
	blood_ = 0;
	bullet_id_ = 0;
	damage_ = 0;
	frozen_ = 0;
	row_ = 0;
	col_ = 0;
}

entity_t::~entity_t() {

}

void entity_t::fire() {
	room_->get_bullet_mgr()->create_bullet(bullet_id_);
}

void entity_t::update(float tm) {
	if (del_) {
		return;
	}
	if (tm - last_fire_tm_ > cd_) {
		fire();
	} 
}