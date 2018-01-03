#ifndef CONN_MGR_H
#define CONN_MGR_H

#include <map>
#include "tcp_connection.h"

class conn_mgr_t {
public:
	typedef std::map<int, tcp_connection_t *> conns_t;

	typedef std::map<int, int> peers_t;

public:
	conn_mgr_t();

	~conn_mgr_t();

	void verify(int fd, tcp_connection_t *conn);

	void del_conn(int fd);

	void del_sid(int sid);

	int get_sid(int fd);

	int get_fd(int fd);

private:
	conns_t conns_;

	peers_t peers_;
};

#endif