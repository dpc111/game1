#include "grid.h"
#include "room.h"
#include "entity.h"
#include "bullet.h"

grid_t::grid_t(room_t *room) :
	room_(room) {
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
			grids_[i][j] = NULL;
		}
	}
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
			int x = (i - 1) * ROW_LENGTH + (ROW_LENGTH / 2);
			int y = (j - 1) * COL_LENGTH + (COL_LENGTH / 2);
			pos_[i][j] = vector2_t(x, y);	
		}
	}
}

grid_t::~grid_t() {
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
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
	return grids_[row - 1][col - 1];
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

vector2_t& grid_t::get_pos(int32 row, int32 col) {
	assert(pos_[row][col]);
	return pos_[row][col];
}

void grid_t::process_collision(bullet_t *bullet) {
	int32 camp = bullet->get_camp();
	int32 line = bullet->get_line();
	vector2_t& pos = bullet->get_pos();
	if (pos.x < BULLET_OUT_MIN_X ||
		pos.x > BULLET_OUT_MAX_X ||
		pos.y < BULLET_OUT_MIN_Y ||
		pos.y > BULLET_OUT_MAX_Y) {
		bullet->on_bullet_out();
		return;
	}
	for (int32 k = 0; k < ROW_NUM; k++) {
		entity_t* entity = grids_[k][line];
		if (camp != entity->get_camp() &&
			entity->get_box().collision(pos)) {
			entity->on_collision(bullet);
			bullet->on_collision(entity);
			return;
		}
	}
}