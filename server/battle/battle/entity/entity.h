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

	vector2_t& get_pos() { return pos_; }

	vector2_t& get_gun_pos() { return gun_pos_; }

	int32 get_camp() { return camp_; }

	bool get_del() { return del_; }

	int32 get_level() { return level_; }

	float get_cd() { return cd_; }

	int32 get_blood() { return blood_; }

	int32 get_damage() { return damage_; }

	float get_frozen() { return frozen_; }

	int32 get_row() { return row_; }

	int32 get_col() { return col_; }

	void set_pos(const vector2_t& pos) { pos_ = pos; }

	void set_gun_pos(const vector2_t& pos) { gun_pos_ = pos; }

	void set_camp(int32 camp) { camp_ = camp; } 

	void set_del(bool del) { del_ = del; }

	void set_level(int32 level) { level_ = level; }

	void set_cd(float cd) { cd_ = cd; }

	void set_blood(int32 blood) { blood_ = blood; }

	void set_bullet(int32 bullet_id) { bullet_id_ = bullet_id; }

	void set_damage(int32 damage) { damage_ = damage; }

	void set_frozen(float frozen) { frozen_ = frozen; }

	void set_grid(int32 row, int32 col) { row_ = row; col_ = col; }

	void set_box(float boxx, float boxy);

	void update(float tm);

	void fire();

	void on_collision(bullet_t *bullet);

private:
	room_t *room_;

	box_t box_;

	vector2_t pos_;

	vector2_t gun_pos_;

	int32 camp_;

	bool del_;

	int32 level_;

	float cd_;

	float last_fire_tm_;

	int32 blood_;

	int32 bullet_id_;

	int32 damage_;

	float frozen_;

	int32 row_;

	int32 col_;
};

#endif