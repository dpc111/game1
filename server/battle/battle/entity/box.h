#ifndef	BOX_H
#define BOX_H

#include "gmath.h"

class box_t {
public:
	box_t();

	~box_t();

	void set_box(float x1_, float x2_, float y1_, float y2_, float z1_, float z2_);

	bool collision(const vector3_t& pos);

private:
	float x1;

	float x2;

	float y1;

	float y2;

	float z1;

	float z2;
};

#endif