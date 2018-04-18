#ifndef REGISTER_MSG
#define REGISTER_MSG

class register_msg_t {
public:
	register_msg_t();

	~register_msg_t();

	void register_client_msg();

	void register_server_msg();

private:
	service_t *service_;

	client_msg_t *client_msg_;

	server_msg_t *server_msg_;
}

#endif