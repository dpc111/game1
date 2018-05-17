#ifndef GRID_H
#define GRID_H

#define ROW_NUM 10
#define COL_NUM 5

#include "stdafx.h"

class entity_t;
class room_t;

class grid_t {
public:
	grid_t(room_t *room);

	~grid_t();

	void set_entity(int32 row, int32 col, entity_t *entity);

	void del_entity(int32 row, int32 col);

	entity_t* get_entity(int32 row, int32 col);

	bool exist(int32 row, int32 col);

public:
	entity_t* grids_[ROW_NUM][COL_NUM];
	room_t* room_;
};

#endif