#ifndef BULLET_H
#define BULLET_H

#include "object_base.h"

#define MAX_LIFE_TM 10

class bullet_t : public object_base_t {
public:
	bullet_t(int32 bullet_id, int32 type_id);

	virtual ~bullet_t();

	int32 get_speed() { return speed_; }

	bool get_del() { return del_; }

	void set_speed(int32 speed) { speed_ = speed; }

	void update(float tm);

private:
	int32 speed_;

	float create_tm_;

	bool del_;
};

#endif