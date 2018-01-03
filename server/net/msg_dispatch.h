#ifndef MESSAGE_DISPATCH_T
#define MESSAGE_DISPATCH_T

#include <string>
#include <map>
#include "google/protobuf/message.h"

class tcp_connection_t;
class tcp_network_t;

class msg_dispatch_t {
public:
	typedef std::map<int, std::string> msg_id_map_t;

	typedef std::map<std::string, int> msg_name_map_t;

public:
	msg_dispatch_t(tcp_network_t *network);

	~msg_dispatch_t();

	int msg_id(std::string& name);	

	std::string& msg_name(int id);

	void on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg);

private:
	tcp_network_t *network_;

	msg_id_map_t id_map_;

	msg_name_map_t name_map_;
};

#endif