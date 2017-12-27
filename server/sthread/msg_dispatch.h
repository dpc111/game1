#ifndef MESSAGE_DISPATCH
#define MESSAGE_DISPATCH

class msg_dispatch_t {
public:
	msg_dispatch_t(tcp_network_t *network);

	msg_dispatch_t();

	void on_message(tcp_connection_t *conn, int msgid, google::protobuf::Message *msg);

private:
	tcp_network_t *network_;
}

#endif