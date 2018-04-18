#ifndef PLAYER_MGR_H
#define PLAYER_MGR_H

#include <map>

class player_mgr_t {
public:
	std::map<int, player_t> player_map_t;

public:
	player_mgr_t();

	~player_mgr_t();

	player_t* get_player(int uid);

	void add_player(int uid, player_t *player);

private:
	player_map_t players_;
	int player_num_;
};

#endif