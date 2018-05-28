#include "entity.h"
#include "room.h"
#include "bullet.h"
#include "bullet_mgr.h"
#include "box.h"
#include "bullet.h"

entity_t::entity_t(room_t *room, int32 entity_id, int32 type_id) :
	object_base_t(entity_id, type_id),
	room_(room),
	box_() {
	camp_ = 1;
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

void entity_t::set_box(float boxx, float boxy) {
	vector2_t& pos = room_->get_grid()->get_pos(row_, col_);
	box_->set_box(pos_.x - boxx, pos_.x + boxx, pos_.y - boxy, pos_.y + boxy);
}

void entity_t::update(float tm) {
	if (del_) {
		return;
	}
	if (tm - last_fire_tm_ > cd_) {
		fire();
	} 
}

void entity_t::fire() {
	room_->get_bullet_mgr()->create_bullet(this, bullet_id_);
}

void entity_t::on_collision(bullet_t *bullet) {
	int32 damage = bullet->get_damage();
	blood_ = blood_ - damage;
	if (blood_ <= 0) {
		blood_ = 0;
		set_del(true);
	}
}