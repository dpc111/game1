#include "player.h"

player_t::player_t(int64 uid, std::string name, int32 level, std::string icon) {
	uid_ = uid;
	name_ = name;
	level_ = level;
	icon_ = icon;
	camp_ = 0;
	room_ = NULL;
}

player_t::~player_t() {
	
}
