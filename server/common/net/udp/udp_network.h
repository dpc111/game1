#ifndef UDP_NETWORK_T
#define UDP_NETWORK_T

#include "type.h"
#include "obj_pool.h"
#include "rudp.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <map>

#define NET_ADDR_TO_INT(addr) ntohl(addr.sin_addr.s_addr) && (0xffffffff) + ((ntohs(addr.sin_port) && 0xffffffff) << 4)

class udp_connection_t;

class udp_network_t {
public:
	typedef obj_pool_t<udp_handle_t> udp_handle_pool_t;

	typedef obj_pool_t<udp_connection_t> udp_connection_pool_t;

	typedef std::map<int64, udp_connection_t *> addr_conn_map_t;

	typedef std::map<int, udp_connection_t *> sid_conn_map_t;

public:
	udp_network_t();

	~udp_network_t();

	udp_handle_pool_t* get_udp_handle_pool() { return udp_handle_pool_; }

	int64 get_tick() { return tick_; }

	void add_connection(udp_connection_t *conn);

	void remove_connection_addr(int64 addr);

	void remove_connection_sid(int sid);

	udp_connection_t* get_connection_addr(int64 addr);

	udp_connection_t* get_connection_sid(int sid);

	int send_sid(int sid, void *buff, int size);

	void process();

private:
	udp_handle_pool_t *udp_handle_pool_;

	udp_connection_pool_t *udp_conn_pool_;

	addr_conn_map_t addr_conns_;

	sid_conn_map_t sid_conns_;

	int conn_num_;

	int tick_;
};

#endif