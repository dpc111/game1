#include "room.h"
#include "frame_mgr.h"

room_t::room_t(int32 rid) {
	rid_ = rid;
	start_time_ = 0;
	over_ = false;
	players_.clear();
	player_num_ = 0;
	frame_mgr_ = new frame_mgr_t(this);
}

room_t::~room_t() {
	rid_ = 0;
	start_time_ = 0;
	over_ = false;
	players_.clear();
	player_num_ = 0;
	delete frame_mgr_;
	frame_mgr_ = NULL;
}

void room_t::start() {
	start_time_ = getms();
}

void room_t::end() {
	set_over(true);
}

void room_t::update(int64 ms) {
	if (get_over()) {
		return;
	}
	if (ms - start_time_ > ROOM_MAX_GAME_TIME) {
		end();
		return
	}
	frame_mgr_->refresh();
}

void room_t::add_player(int64 uid, player_t *player) {
	player_map_t::iterator it = players_.fine(uid);
	if (it != players_.end()) {
		return;
	}
	players_[uid] = player;
	++player_num_;
}

void room_t::del_player(int64 uid) {
	player_map_t::iterator it = players_.fine(uid);
	if (it == players_.end()) {
		return;
	}
	players_.erase(it);
	--player_num_;
}