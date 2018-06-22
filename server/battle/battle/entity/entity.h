#ifndef ENTITY_H
#define ENTITY_H

#include "object_base.h"
#include "box.h"

class room_t;
class bullet_t;

class entity_t : public object_base_t {
public:
	entity_t(room_t *room, int32 entity_id, int32 type_id);

	virtual ~entity_t();

	box_t& get_box() { return box_; }

	void set_box(const vector3_t& box);

	void update(double tm);

	void fire();

	void on_collision(bullet_t *bullet);

private:
	PROPERTY(room_t*, 				room)
	PROPERTY_QUOTE(vector3_t, 		pos)
	PROPERTY_QUOTE(vector3_t, 		gun_pos)
	PROPERTY(int32,					camp)
	PROPERTY(bool,					del)
	PROPERTY(int32,					level)
	PROPERTY(float,					cd)
	PROPERTY(double,				last_fire_tm)
	PROPERTY(int32,					blood)
	PROPERTY(int32,					bullet_id)
	PROPERTY(int32,					damage)
	PROPERTY(float,					frozen)
	PROPERTY(int32,					row)
	PROPERTY(int32,					col)
	box_t 							box_;
};

#endif