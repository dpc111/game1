#ifndef ENTITY_H
#define ENTITY_H

#include "object_base.h"

class room_t;

class entity_t : public object_base_t {
public:
	entity_t(room_t *room, int32 entity_id, int32 type_id);

	virtual ~entity_t();

	bool get_del() { return del_; }

	int32 get_level() { return level_; }

	float get_cd() { return cd_; }

	int32 get_blood() { return blood_; }

	int32 get_damage() { return damage_; }

	float get_frozen() { return frozen_; }

	void set_level(int32 level) { level_ = level; }

	void set_cd(float cd) { cd_ = cd; }

	void set_blood(int32 blood) { blood_ = blood; }

	void set_bullet(int32 bullet_id) { bullet_id_ = bullet_id; }

	void set_damage(int32 damage) { damage_ = damage; }

	void set_frozen(float frozen) { frozen_ = frozen; }

	void set_grid(int32 row, int32 col) { row_ = row; col_ = col; }

	void fire();

	void update(float tm);

private:
	room_t *room_;

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