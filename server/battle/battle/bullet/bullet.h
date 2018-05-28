#ifndef BULLET_H
#define BULLET_H

#include "object_base.h"
#include "gmath.h"

class room_t;
class entity_t;

#define MAX_LIFE_TM 10

class bullet_t : public object_base_t {
public:
	bullet_t(room_t *room, int32 bullet_id, int32 type_id);

	virtual ~bullet_t();

	virtual void update(float stm);

	int32 get_speed() { return speed_; }

	int32 get_camp() { return camp_; }

	bool get_del() { return del_; }

	vector2_t& get_pos() { return pos_; }

	int get_damage() { return damage_; }

	int32 get_line() { return line_; }

	void set_speed(int32 speed) { speed_ = speed; }

	void set_camp(int32 camp) { camp_ = camp; }

	void set_del(bool del) { del_ = del; }

	void set_v_speed(vector2_t& speed) { v_speed_ = speed; }

	void set_begin_pos(vector2_t& pos) { begin_pos_ = pos; }

	void set_pos(vector2_t& pos) { pos_ = pos; }

	void set_damage(int32 damage) { damage_ = damage; }

	void set_line(int32 line) { line_ = line; }

	void on_collision(entity_t *entity);

	void on_bullet_out();

protected:
	int32 speed_;

	room_t *room_;

	int32 camp_;

	bool del_;

	float begin_tm_;

	vector2_t v_speed_;

	vector2_t begin_pos_;

	vector2_t pos_;

	int32 damage_;

	int32 line_;
};

#endif