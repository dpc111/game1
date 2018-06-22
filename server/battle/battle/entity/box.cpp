#include "box.h"

box_t::box_t() :
	x1(0),
	x2(0),
	y1(0),
	y2(0),
	z1(0),
	z2(0) {

}

box_t::~box_t() {

}

void box_t::set_box(float x1_, float x2_, float y1_, float y2_, float z1_, float z2_) {
	x1 = x1_;
	x2 = x2_;
	y1 = y1_;
	y2 = y2_;
	z1 = z1_;
	z2 = z2_;
}

bool box_t::collision(const vector3_t& pos) {
	if (pos.x > x1 
		and pos.x < x2
		and pos.y > y1
		and pos.y < y2
		and pos.z > z1
		and pos.z < z2) {
		return true;
	}
	return false;
}