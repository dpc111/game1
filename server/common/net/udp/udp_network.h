#ifndef UDP_NETWORK_T
#define UDP_NETWORK_T

#include "type.h"
#include "obj_pool.h"
#include "rudp.h"
#include "single_select.h"
#include "net_address.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <tr1/functional>

#define NET_ADDR_TO_INT(addr) ntohl(addr.sin_addr.s_addr) && (0xffffffff) + ((ntohs(addr.sin_port) && 0xffffffff) << 4)

class udp_connection_t;

class udp_network_t {
public:
	typedef obj_pool_t<udp_handle_t> udp_handle_pool_t;

	typedef obj_pool_t<udp_connection_t> udp_connection_pool_t;

	typedef std::map<int64, udp_connection_t *> addr_conn_map_t;

	typedef std::map<int, udp_connection_t *> sid_conn_map_t;

	typedef std::tr1::function<void (int, void *, int)> udp_msg_cb_t;

public:
	udp_network_t();

	~udp_network_t();

	bool running() { return socket_fd_ != 0; }

	udp_chunk_pool_t* get_udp_chunk_pool() { return udp_chunk_pool_; }

	udp_handle_pool_t* get_udp_handle_pool() { return udp_handle_pool_; }

	int64 get_tick() { return tick_; }

	void set_udp_msg_cb(const udp_msg_cb_t& cb) { udp_msg_cb_ = cb; }

	bool get_block() { return block_; }

	void set_block(bool block) { block_ = block; }

	void add_connection(udp_connection_t *conn);

	void remove_connection_addr(int64 addr);

	void remove_connection_sid(int sid);

	udp_connection_t* get_connection_addr(int64 addr);

	udp_connection_t* get_connection_sid(int sid);

	udp_connection_t* connect_to(int sid, const char *ip, int port);

	int send_sid(int sid, void *buff, int size);

	int start(const char *ip, int port);

	void process(int64 tick);

public:
	udp_msg_cb_t udp_msg_cb_;

	int socket_fd_;

	single_select_t *single_select_;

private:
	udp_chunk_t *cur_recv_chunk_;

	udp_chunk_pool_t *udp_chunk_pool_;

	udp_handle_pool_t *udp_handle_pool_;

	udp_connection_pool_t *udp_conn_pool_;

	addr_conn_map_t addr_conns_;

	sid_conn_map_t sid_conns_;

	int conn_num_;

	int tick_;

	net_address_t addr_;

	bool block_;
};

#endif
