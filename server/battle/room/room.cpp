#include "room.h"
#include "entity_mgr.h"
#include "bullet_mgr.h"
#include "entity.h"
#include "bullet.h"
#include "grid.h"
#include "player.h"
#include "service.h"
#include "global.h"

#define REGISTER_ROOM_BIND(func) \
	std::tr1::bind(&func, \
	this, \
	std::tr1::placeholders::_1, \
	std::tr1::placeholders::_2)

room_t::room_t(int32 rid) :
	dispatcher_t(),
	wait_timer_(),
	ing_timer_(),
	end_timer_() {
	rid_ = rid;
	entity_mgr_ = new entity_mgr_t(this);
	bullet_mgr_ = new bullet_mgr_t(this);
	grid_ = new grid_t(this);
	del_ = false;
	room_state_ = ROOM_STATE_WAIT;
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
	wait_timer_.cancel();
	ing_timer_.cancel();
	end_timer_.cancel();
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

void room_t::update(int64 tm) {
	if (del_ || room_state_ != ROOM_STATE_ING) {
		return;
	}
	entity_mgr_->update(tm);
	bullet_mgr_->update(tm);
}

void room_t::register_callback() {
	register_message<battle_msg::c_create_entity>(REGISTER_ROOM_BIND(room_t::c_create_entity));
}

void room_t::broadcast(google::protobuf::Message& msg) {
	for (int32 k = 0; k < 2; k++) {
		int64 uid = camps_[k];
		if (uid > 0) {
			get_service()->send(uid, msg);
		}
	}
}

void room_t::c_create_entity(void *player, const battle_msg::c_create_entity& msg) {
	player_t *p = (player_t *)player;
	if (room_state_ != ROOM_STATE_ING) {
		return;
	}
	entity_mgr_->create_entity(p->get_camp(), msg.type_id(), msg.row(), msg.col())
}

void room_t::start_wait() {
	ERROR("room wait");
	room_state_ = ROOM_STATE_WAIT;
	this->on_room_state_change(room_state_);
	wait_timer_ = get_service()->register_delay_stimer(this, NULL, ROOM_WAIT_TIME, 0);
}

void room_t::start_ing() {
	ERROR("room ing");
	room_state_ = ROOM_STATE_ING;
	this->on_room_state_change(room_state_);
	ing_timer_ = get_service()->register_delay_stimer(this, NULL, ROOM_ING_TIME, 0);
}

void room_t::start_end() {
	ERROR("room end");
	room_state_ = ROOM_STATE_END;
	this->on_room_state_change(room_state_);
	end_timer_ = get_service()->register_delay_stimer(this, NULL, ROOM_END_TIME, 0);
}

void room_t::handle_timeout(timer_handle_t handle, void *user) {
	// if (handle == wait_timer_ && room_state_ == ROOM_STATE_WAIT) {
	// 	start_ing();
	// }
	// if (handle == ing_timer_ && room_state_ == ROOM_STATE_ING) {
	// 	start_end();
	// }
	// if (handle == end_timer_ && room_state_ == ROOM_STATE_END) {
	// 	set_del(true);
	// }
	if (handle == wait_timer_) {
		start_ing();
	}
	if (handle == ing_timer_) {
		start_end();
	}
	if (handle == end_timer_) {
		set_del(true);
	}
}

void room_t::pack_entity_info(entity_t *entity, battle_msg::entity_info *info) {
	info->set_id(entity->get_id());
	info->set_type_id(entity->get_type_id());
	info->set_camp(entity->get_camp());
	info->set_blood(entity->get_blood());
	info->set_cd(entity->get_cd());
	info->set_row(entity->get_row());
	info->set_col(entity->get_col());
	battle_msg::vector *pos = info->mutable_pos();
	pos->set_x(entity->get_pos().x);
	pos->set_y(entity->get_pos().y);
	pos->set_z(entity->get_pos().z);
}

void room_t::pack_bullet_info(bullet_t *bullet, battle_msg::bullet_info *info) {
	info->set_id(bullet->get_id());
	info->set_type_id(bullet->get_type_id());
	info->set_camp(bullet->get_camp());
	info->set_damage(bullet->get_damage());
	battle_msg::vector *pos = info->mutable_pos();
	pos->set_x(bullet->get_pos().x);
	pos->set_y(bullet->get_pos().y);
	pos->set_z(bullet->get_pos().z);
	battle_msg::vector *speed = info->mutable_speed();
	speed->set_x(bullet->get_v_speed().x);
	speed->set_y(bullet->get_v_speed().y);
	speed->set_z(bullet->get_v_speed().z);
}

void room_t::on_room_state_change(int32 state) {
	battle_msg::s_room_state msg;
	msg.set_state(state);
	broadcast(msg);
}

void room_t::on_create_entity(entity_t *entity) {
	battle_msg::s_create_entity msg;
	battle_msg::entity_info *info = msg.mutable_einfo();
	pack_entity_info(entity, info);
	msg.set_use_gold(0);
	msg.set_cur_gold(0);
	broadcast(msg);
}

void room_t::on_del_entity(entity_t *entity) {

}

void room_t::on_create_bullet(entity_t *entity, bullet_t *bullet) {

}

void room_t::on_del_bullet(bullet_t *bullet) {

}

void room_t::on_entity_damage(entity_t *entity, int32 damage) {

}

void room_t::on_fire(entity_t *entity, bullet_t *bullet) {
	battle_msg::s_fire msg;
	msg.set_eid(entity->get_id());
	battle_msg::bullet_info *info = msg.mutable_binfo();
	pack_bullet_info(bullet, info);
	broadcast(msg);
}

void room_t::on_collision(entity_t *entity, bullet_t *bullet) {
	battle_msg::s_collision msg;
	battle_msg::entity_info *einfo = msg.mutable_einfo();
	pack_entity_info(entity, einfo);
	battle_msg::bullet_info *binfo = msg.mutable_binfo();
	pack_bullet_info(bullet, binfo);
	msg.set_bullet_destroy(true);
	broadcast(msg);
}