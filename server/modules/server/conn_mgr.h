#ifndef CONN_MGR_H
#define CONN_MGR_H

#include <map>

class conn_mgr_t {
public:
	typedef std::map<int, int> sid_fd_t;

	typedef std::map<int, int> fd_sid_t;

public:
	conn_mgr_t();

	~conn_mgr_t();


private:
	sid_fd_t sid_fd_map_;

	fd_sid_t fd_sid_map_;
};

#endif