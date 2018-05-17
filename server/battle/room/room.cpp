#include "room.h"
#include "entity_mgr.h"
#include "entity.h"
#include "grid.h"

room_t::room_t() {
	entity_mgr_ = new entity_mgr_t();
	bullet_mgr_ = new bullet_mgr_t();
	grid_ = new grid_t();
}

room_t::~room_t() {

}