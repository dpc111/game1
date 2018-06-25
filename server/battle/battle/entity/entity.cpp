#include "entity.h"
#include "room.h"
#include "bullet.h"
#include "bullet_mgr.h"
#include "bullet.h"
#include "grid.h"

entity_t::entity_t(room_t *room, int32 entity_id, int32 type_id) :
	object_base_t(entity_id, type_id),
	room_(room) {
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

void entity_t::update(double tm) {
	if (del_) {
		return;
	}
	if (tm - last_fire_tm_ > cd_) {
		update_gun_state();
		fire();
		last_fire_tm_ = tm;
	} 
}

void entity_t::update_gun_state() {
	entity_t *entity = room_->get_grid()->pick_enemy(camp_, col_);
	if (! entity) {
		target_ = 0;
		return;
	}
	target_ = entity->get_id();
	if (bullet_path_ == BULLET_PATCH_PARABOLA) {
		vector3_t& pos = entity->get_pos();
		float vx = FABS(pos.x - pos_.x) / bullet_life_time_;
		if (IS_LEFT_CAMP(camp_)) {
			vx = vx;
		} else {
			vx = -vx;
		}
		bullet_begin_speed_.x = vx;
	}
}

void entity_t::fire() {
	if (target_ == 0) {
		return;
	}
	bullet_t *bullet = room_->get_bullet_mgr()->create_bullet(this, bullet_id_);
	room_->on_fire(this, bullet);
}

void entity_t::on_collision(bullet_t *bullet) {
	int32 damage = bullet->get_damage();
	blood_ = blood_ - damage;
	room_->on_entity_damage(this, damage);
	if (blood_ <= 0) {
		blood_ = 0;
		set_del(true);
	}
}