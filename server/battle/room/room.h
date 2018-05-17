#ifndef ROOM_H
#define ROOM_H

class entity_mgr_t;
class bullet_mgr_t;
class grid_t;

class room_t {
public:
	room_t();

	~room_t();

private:
	int rid_;
	entity_mgr_t *entity_mgr_;
	bullet_mgr_t *bullet_mgr_;
	grid_t *grid_;
};

#endif