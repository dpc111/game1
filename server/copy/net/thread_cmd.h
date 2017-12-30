#ifndef THREAD_CMD_H
#define THREAD_CMD_H

#include <tr1/functional>
#include "tcp_connection.h"

#define NET_RECV_MSG  		0x01
#define NET_FREE_MSG		0x02
#define MAIN_SEND_MSG		0x11
#define MAIN_FREE_MSG		0x12
#define CMD_CONN_ADD		1
#define CMD_CONN_REMOVE 	2

typedef std::tr1::function<void (const tcp_connection_t *)> conn_add_cb_t;
typedef std::tr1::function<void (int)> conn_remove_cb_t;
typedef std::tr1::function<void (tcp_connection_t *)> message_recv_cb_t;
//class tcp_connection_t;

struct thread_cmd_t {
	int op_;
};

struct cmd_add_t : public thread_cmd_t {
	cmd_add_t(tcp_connection_t *conn) {
		conn_ = conn;
		op_ = CMD_CONN_ADD;
	}

	tcp_connection_t *conn_;
};

struct cmd_remove_t : public thread_cmd_t {
	cmd_remove_t(int fd) {
		fd_	= fd;
		conn_ = NULL;
		op_ = CMD_CONN_REMOVE;
	}

	cmd_remove_t(tcp_connection_t *conn) {
		fd_ = conn->get_fd();
		conn_ = conn;
		op_ = CMD_CONN_REMOVE;
	}

	int fd_;
	tcp_connection_t *conn_;
};


struct cmd_message_recv_t : public thread_cmd_t {
	
};

struct cmd_message_free_t : public thread_cmd_t {
	
};

#endif