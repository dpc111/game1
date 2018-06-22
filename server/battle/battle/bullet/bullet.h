#ifndef BULLET_H
#define BULLET_H

#include "object_base.h"
#include "gmath.h"

#define MAX_LIFE_TM 20

class room_t;
class entity_t;

class bullet_t : public object_base_t {
public:
	bullet_t(room_t *room, int32 bullet_id, int32 type_id);

	virtual ~bullet_t();

	virtual void update(double stm);

	int32 get_speed() { return speed_; }

	vector3_t& get_begin_pos() { return begin_pos_; }

	vector3_t& get_pos() { return pos_; }

	vector3_t& get_v_speed() { return v_speed_; }

	void set_begin_pos(const vector3_t& pos) { begin_pos_ = pos; }

	void set_pos(const vector3_t& pos) { pos_ = pos; }

	void on_collision(entity_t *entity);

	void on_bullet_out();

protected:
	PROPERTY(int32, speed)

	PROPERTY(room_t*, room)

	PROPERTY(int32, camp)

	PROPERTY(bool, del)

	PROPERTY(double, begin_time)

	vector3_t v_speed_;

	vector3_t begin_pos_;

	vector3_t pos_;

	PROPERTY(int32, damage)

	PROPERTY(float, damage_r)

	PROPERTY(int32, line)
};

#endif