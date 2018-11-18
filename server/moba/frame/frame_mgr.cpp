#include "frame_mgr.h"
#include "room.h"
#include "service.h"

frame_mgr_t::frame_mgr_t(room_t *room) {
	room_ = room;
	frame_ = 0;
	last_update_tm_ = 0;
}

frame_mgr_t::~frame_mgr_t() {
	room_ = NULL;
	frame_ = 0;
}

void frame_mgr_t::recv(int uid, void *buff, int size)  {
	if (chunk_.write_size() < size + 4) {
		return;
	}
	memcpy(chunk_.write_ptr(), &uid, 4);
	chunk_.write_offset_ += 4;
	memcpy(chunk_.write_ptr(), buff, size); 
	chunk_.write_offset_ += size;
}

void frame_mgr_t::refresh()  {
	sync();
	cache_.write(chunk_.read_ptr(), chunk_.read_size());
	chunk_.read_offset_ = 0;
	chunk_.write_offset_ = 0;
	++frame_;
	memcpy(chunk_.write_ptr(), &frame_, 4);
	chunk_.write_offset_ += 4;
}

void frame_mgr_t::sync()  {
	room_t::player_map_t& players = room_->get_players();
	room_t::player_map_t::iterator it = players.begin();
	for ( ; it != players.end(); it++) {
		get_service()->udp_send_sid(it->first, chunk_.buff_, chunk_.read_size());
	}
}

void frame_mgr_t::update(int64 ms) {
	if (ms - last_update_tm_ < FRAME_INTERVAL_MS) {
		return;
	}
	last_update_tm_ = ms;
	refresh();
}