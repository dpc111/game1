#ifndef BULLET_H
#define BULLET_H

#include "object_base.h"

class Bullet_t {
public:
	bullet_t(int32 bullet_id, int32 type_id);

	~bullet_t();

private:
	int32 speed_;

};

#endif