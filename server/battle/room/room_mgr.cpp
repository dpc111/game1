#include "stddef.h"
#include "room_mgr.h"

room_mgr_t::room_mgr_t() {

}

room_mgr_t::~room_mgr_t() {

}

room_t* room_mgr_t::get_room(int rid) {
	room_map_t::iterator it = rooms_.find(rid);
	if (it == rooms_.end()) {
		return NULL;
	}
	return it->second;
}

void room_mgr_t::add_room(int rid, room_t *room) {
	if (this->get_room(rid)) {
		return;
	}
	rooms_[rid] = room;
	++room_num_;
}

void room_mgr_t::del_room(int rid) {
	room_map_t::iterator it = rooms_.find(rid);
	if (it == rooms_.end()) {
		return;
	}
	room_t *room = it->second;
	rooms_.erase(it);
	--room_num_;
	delete room;
}
