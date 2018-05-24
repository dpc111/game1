#include "stdafx.h"
#include "room_mgr.h"
#include "room.h"
#include "id_generate.h"

room_mgr_t::room_mgr_t() :
	id_generate_() {

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

room_t* room_mgr_t::create_room() {
	int32 rid = id_generate_.gen();
	if (get_room(rid)) {
		return NULL;
	}
	room_t *room = new room_t(rid);
	add_room(rid, room);
}

void room_mgr_t::update(int64 tm) {
	for (room_map_t::iterator it = rooms_.begin(); it != rooms_.end(); it++) {
		room_t *room;
		room->update(tm);
	}
}