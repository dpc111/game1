#ifndef PLAYER_MGR_H
#define PLAYER_MGR_H

#include <map>

class player_t;

class player_mgr_t {
public:
	std::map<int, player_t*> player_map_t;

public:
	player_mgr_t();

	~player_mgr_t();

	player_t* get_player(int uid);

	void add_player(int uid, player_t *player);

	void del_player(int uid);

	void player_num() { return player_num_; }

private:
	player_map_t players_;

	int player_num_;
};

#endif