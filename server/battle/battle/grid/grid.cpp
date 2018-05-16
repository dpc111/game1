#include "grid.h"

grid_t::grid_t() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; i++) {
			grids_[i][j] = 0;
		}
	}
}

grid_t::~grid_t() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; i++) {
			grids_[i][j] = 0;
		}
	}
}