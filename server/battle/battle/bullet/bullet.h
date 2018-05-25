#ifndef BULLET_H
#define BULLET_H

#include "object_base.h"
#include "math.h"

#define MAX_LIFE_TM 10

class bullet_t : public object_base_t {
public:
	bullet_t(int32 bullet_id, int32 type_id);

	virtual ~bullet_t();

	virtual void update(float stm);

	int32 get_speed() { return speed_; }

	bool get_del() { return del_; }

	void set_speed(int32 speed) { speed_ = speed; }

private:
	int32 speed_;

	bool del_;

	float begin_tm_;

	vector2_t v_speed_;

	vector2_t begin_pos_;

	vector2_t pos_;
};

#endif