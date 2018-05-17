#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"

class entity_mgr_t;
class bullet_mgr_t;
class grid_t;

class room_t {
public:
	room_t(int32 rid);

	~room_t();

	entity_mgr_t* get_entity_mgr() { return entity_mgr_; }

	bullet_mgr_t* get_bullet_mgr() { return bullet_mgr_; }

	grid_t* get_grid() { return grid_; }

	int32 set_random_camp(int32 camp, int64 uid);

private:
	int32 rid_;

	entity_mgr_t *entity_mgr_;

	bullet_mgr_t *bullet_mgr_;

	grid_t *grid_;
	
	int64 camps_[2];
};

#endif