#ifndef BULLET_H
#define BULLET_H

#include "object_base.h"

class bullet_t : public object_base_t {
public:
	bullet_t(int32 bullet_id, int32 type_id);

	virtual ~bullet_t();

	int32 get_speed() { return speed_; }

	void set_speed(int32 speed) { speed_ = speed; }

private:
	int32 speed_;

};

#endif