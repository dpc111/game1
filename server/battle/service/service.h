#ifndef SERVICE_H
#define SERVICE_H

class service_t : public server_t {
public:
	service_t();

	~service_t();

	void init();

	player_mgr_t* get_player_mgr() { return player_mgr_; }

	room_mgr_t* get_room_mgr() { return room_mgr_; }

private:
	player_mgr_t *player_mgr_;
	
	room_mgr_t *room_mgr_;
};

#endif