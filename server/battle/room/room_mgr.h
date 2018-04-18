#ifndef ROOM_MGR_H
#define ROOM_MGR_H

#include <map>

class room_mgr_t {
public:
	typedef std::map<int, room_t*> room_map_t;
public:
	room_mgr_t();

	~room_mgr_t();

	room_t* get_room(int rid);

	void add_room(int rid, room_t *room);

	void del_room(int rid);

	void room_num() { return room_num_; }

private:
	room_map_t rooms_;

	int room_num_;
};

#endif