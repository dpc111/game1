#ifndef GLOBAL_H
#define GLOBAL_H

class player_mgr_t;
class room_mgr_t;
class service_t;

player_mgr_t* get_player_mgr();

room_mgr_t* get_room_mgr();

service_t* get_service();

#endif