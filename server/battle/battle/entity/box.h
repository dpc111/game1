#ifndef	BOX_H
#define BOX_H

class box_t {
public:
	box_t();

	~box_t();

	void set_box(float x1_, float x2_, float y1_, float y2_);

	bool collision(vector2_t& pos);

private:
	float x1;

	float x2;

	float y1;
	
	float y2;
}

#endif