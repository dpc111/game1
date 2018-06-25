#ifndef ENTITY_H
#define ENTITY_H

#include "object_base.h"

class room_t;
class bullet_t;

class entity_t : public object_base_t {
public:
	entity_t(room_t *room, int32 entity_id, int32 type_id);

	virtual ~entity_t();

	void update(double tm);

	void update_gun_state();

	void fire();

	void on_collision(bullet_t *bullet);

private:
	PROPERTY(room_t*, 							room)
	PROPERTY_QUOTE(vector3_t, 					pos)
	PROPERTY_QUOTE(vector3_t, 					gun_pos)
	PROPERTY_QUOTE(collision_cube_simple_t,		collision_box)
	PROPERTY(int32,								camp)
	PROPERTY(bool,								del)
	PROPERTY(int32,								level)
	PROPERTY(float,								cd)
	PROPERTY(double,							last_fire_tm)
	PROPERTY(int32,								blood)
	PROPERTY(int32,								bullet_path)
	PROPERTY(int32,								bullet_id)
	PROPERTY(int32,								damage)
	PROPERTY(float,								frozen)
	PROPERTY(int32,								row)
	PROPERTY(int32,								col)

	PROPERTY(int32,								target)
	PROPERTY(float,								bullet_life_time)
	PROPERTY_QUOTE(vector3_t,					bullet_begin_speed)
};

#endif