#ifndef BULLET_HORIZONTAL_H
#define BULLET_HORIZONTAL_H

#include "bullet.h"

class bullet_horizontal_t : public bullet_t {
public:
	bullet_horizontal_t(int32 bullet_id, int32 type_id);

	virtual ~bullet_horizontal_t();

	virtual void update(float stm);

private:

};

#endif