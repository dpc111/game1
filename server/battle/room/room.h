#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"
#include "dispatcher.h"
#include "message/messages.h"
#include "timer_handle.h"

#define ROOM_STATE_WAIT 1
#define ROOM_STATE_ING 2
#define ROOM_STATE_END 3
#define ROOM_WAIT_TIME 2 
#define ROOM_ING_TIME 5
#define ROOM_END_TIME 5 

class entity_mgr_t;
class bullet_mgr_t;
class grid_t;
class entity_t;
class bullet_t;

class room_t : public dispatcher_t, timer_handle_t {
public:
	room_t(int32 rid);

	~room_t();

	int32 get_rid() { return rid_; }

	entity_mgr_t* get_entity_mgr() { return entity_mgr_; }

	bullet_mgr_t* get_bullet_mgr() { return bullet_mgr_; }

	grid_t* get_grid() { return grid_; }

	bool get_del() { return del_; }

	int32 set_random_camp(int64 uid);

	void set_del(bool del) { del_ = del; }

	int32 get_player_num() { return player_num_; }

	void update(int64 tm);

	virtual void register_callback();

	void broadcast(google::protobuf::Message& msg);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// message
	void c_get_room_info(void *player, const battle_msg::c_get_room_info& msg);
	
	void c_create_entity(void *player, const battle_msg::c_create_entity& msg);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// timer
	void start_wait();

	void start_ing();

	void start_end();

	void start_wait_timeout(void *data);

	void start_ing_timeout(void *data);

	void start_end_timeout(void *data);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// pack message
	void pack_entity_info(entity_t *entity, battle_msg::entity_info *info);

	void pack_bullet_info(bullet_t *bullet, battle_msg::bullet_info *info);

	void pack_room_info(battle_msg::room_info *info);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// event
	void on_room_state_change(int32 state);

	void on_create_entity(entity_t *entity);

	void on_del_entity(entity_t *entity);

	void on_create_bullet(entity_t *entity, bullet_t *bullet);

	void on_del_bullet(bullet_t *bullet);

	void on_entity_damage(entity_t *entity, int32 damage);

	void on_fire(entity_t *entity, bullet_t *bullet);

	void on_collision(entity_t *entity, bullet_t *bullet);

private:
	int32 rid_;

	entity_mgr_t *entity_mgr_;

	bullet_mgr_t *bullet_mgr_;

	grid_t *grid_;
	
	int64 camps_[2];

	bool del_;

	int32 room_state_;

	double begin_time_;

	int32 player_num_;
};

#endif