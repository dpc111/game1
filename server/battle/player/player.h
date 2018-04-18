#ifndef PLAYER_H
#define PLAYER_H

#include "stddef.h"

class player_t{
public:
	player_t();

	~player_t();

private:
	int uid_;

	std::string name_;

	int level_;
};

#endif