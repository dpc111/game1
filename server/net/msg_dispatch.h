#ifndef MESSAGE_DISPATCH_T
#define MESSAGE_DISPATCH_T

#include <string>
#include <map>
#include <tr1/functional>
#include "google/protobuf/message.h"

class tcp_connection_t;
class tcp_network_t;

template<typename T>
typedef std::tr1::function<void (int, const T&)> msg_cb;

typedef std::tr1::function<void (tcp_connection_t *, const T&)> net_msg_cb;

class msg_dispatch_t {
public:
	typedef struct {
		msg_cb *msg_cb_;
		net_msg_cb *net_msg_cb_;
		const char *name_;
	} info_t;

	typedef std::map<int, info_t *> msg_map_t;

	typedef std::map<const char *, int> name_map_t;

public:
	msg_dispatch_t(tcp_network_t *network);

	~msg_dispatch_t();

	int msg_id(const char *name);	

	const char *msg_name(int id);

	void on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg);

private:
	tcp_network_t *network_;

	msg_map_t msgs_;

	name_map_t names_;
};

#endif