#ifndef CLIENT_MSG
#define CLIENT_MSG

#include "stdafx.h"
#include "message/messages.h"
#include "tcp_connection.h"

class client_msg_t {
public:
	client_msg_t();

	~client_msg_t();

	void s2c_join(tcp_connection_t *conn, const battle_msg::s2c_join& msg);

	void c2s_join(tcp_connection_t *conn, const battle_msg::c2s_join& msg);

	void c_login(tcp_connection_t *conn, const battle_msg::c_login& msg);

	void c_create_entity(int32 uid, const battle_msg::c_create_entity& msg);
};

#endif