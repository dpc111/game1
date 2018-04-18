#ifndef CLIENT_MSG
#define CLIENT_MSG

#include "message/messages.h"
#include "tcp_connection.h"

class client_msg_t {
public:
	client_msg_t();

	~client_msg_t();

	void s2c_join(tcp_connection_t *conn, const battle::s2c_join& msg);
};

#endif