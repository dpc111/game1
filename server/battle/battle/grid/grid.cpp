#include "grid.h"
#include "entity.h"

grid_t::grid_t() {
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; i++) {
			grids_[i][j] = NULL;
		}
	}
}

grid_t::~grid_t() {
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; i++) {
			grids_[i][j] = NULL;
		}
	}
}

void grid_t::set_entity(int32 row, int32 col, entity_t *entity) {
	if (row < 1 || row > ROW_NUM || col < 1 || col > COL_NUM) {
		return;
	}
	if (exist(row, col)) {
		return;
	}
	grids_[row - 1][col - 1] = entity;
}

void grid_t::del_entity(int32 row, int32 col) {
	if (row < 1 || row > ROW_NUM || col < 1 || col > COL_NUM) {
		return;
	}
	if (exist(row, col)) {
		return;
	}
	grids_[row - 1][col - 1] = NULL;
}

entity_t* grid_t::get_entity(int32 row, int32 col) {
	if (row < 1 || row > ROW_NUM || col < 1 || col > COL_NUM) {
		return NULL;
	}
	return grids_[row - 1][col - 1]
}

bool grid_t::exist(int32 row, int32 col) {
	if (row < 1 || row > ROW_NUM || col < 1 || col > COL_NUM) {
		return false;
	}
	if (grids_[row - 1][col - 1]) {
		return true;
	}
	return false;
}