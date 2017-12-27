#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

class msg_operate_t {
public:
	msg_operate_t(tcp_network_t *network);

	~msg_operate_t();

	google::protobuf::Message *gen_message(int msg_id);

	void free_message(google::protobuf::Message *msg);

	void send(tcp_connection_t *conn, google::protobuf::Message& msg);

	bool on_message(tcp_connection_t *conn);

private:
	tcp_network_t *network_;
};

#define