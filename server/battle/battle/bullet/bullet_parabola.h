#ifndef BULLET_PARABOLA_H
#define BULLET_PARABOLA_H

#include "bullet.h"

class bullet_parabola_t : public bullet_t {
public:
	bullet_parabola_t(int32 bullet_id, int32 type_id);

	virtual ~bullet_parabola_t();

	virtual update(float stm);

private:

}

#endif