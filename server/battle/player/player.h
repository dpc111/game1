#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"

class player_t{
public:
	player_t(int64 uid, std::string name, int32 level, std::string icon);

	~player_t();

public:
	int64 uid_;

	std::string name_;

	int32 level_;

	std::string icon_;

	bool is_online_;
};

#endif