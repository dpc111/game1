#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "room.h"

class player_t {
public:
	player_t(int64 uid, std::string name, int32 level, std::string icon);

	~player_t();

private:
	PROPERTY(int64, 						uid)
	PROPERTY(int32, 						level)
	PROPERTY(room_t *, 						room)
	PROPERTY(int32, 						camp)
	PROPERTY_QUOTE(std::string, 			name);
	PROPERTY_QUOTE(std::string, 			icon);
};

#endif