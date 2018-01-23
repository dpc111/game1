#include "stdio.h"
#include "log.h"
#include "thread.h"
#include "net_address.h"
#include "tcp_network.h"
#include "tcp_connection.h"

#include "timer.h"
#include "timestamp.h"
#include "server.h"
#include "lua_frame.h"
#include "message/messages.h"

//server_t server("0.0.0.0", 7768);
server_t server("0.0.0.0", 7769);
tcp_connection_t *conn;

class test_timer : public timer_handler_t {
public:
	virtual void handle_timeout(timer_handle_t handle, void *user) {
		int res;
		bool ok = server.get_lua_frame()->call_func("test_add", "ii:i", 1, 2, &res);
	//	printf(".....%d\n", res);
	//	ERROR("");
		// test
		for (int i = 1; i < 1000; i++) 
			server.send_func(conn, "msg_func", "iis", 1, 8, "dpc");
	//	ERROR("");
		assert(ok);
	}

	void test_msg(tcp_connection_t *conn, const battle::s2c_join& msg) {
		ERROR("%d", msg.uid());
		ERROR("%s", msg.name());
		ERROR("%s", msg.icon());
	}

	void register_msg(server_t *server) {
		server->register_net_message<battle::s2c_join>(std::tr1::bind(&test_timer::test_msg, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
		// server->get_network()->get_msg_dispatch()->register_net_message<battle::s2c_join>(std::tr1::bind(&test_timer::test_msg, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
	}

};

int main() {
	server.get_lua_frame()->load_script("test.lua");
	server.get_lua_frame()->run_script();
	server.start();
	test_timer timer;
	server.register_timer(&timer, NULL, 10000, 2000);
	timer.register_msg(&server);

	// test
	 conn = server.connect_to("127.0.0.1", 7768);

	server.process();

	return 1;
}


// #include "stdio.h"
// #include "log.h"
// #include "thread.h"
// #include "net_address.h"
// #include "tcp_network.h"
// #include "tcp_connection.h"

// #include "timer.h"
// #include "timestamp.h"
// #include "server.h"
// #include "lua_frame.h"

// server_t server("0.0.0.0", 7768);
// tcp_connection_t *conn;

// class test_timer : public timer_handler_t {
// 	virtual void handle_timeout(timer_handle_t handle, void *user) {
// 		int res;
// 		bool ok = server.get_lua_frame()->call_func("test_add", "ii:i", 1, 2, &res);
// 		printf(".....%d\n", res);
// 		assert(ok);
// 	}
// };

// int main() {
// 	server.get_lua_frame()->load_script("test.lua");
// 	server.get_lua_frame()->run_script();
// 	server.start();
// 	test_timer timer;
// 	server.register_timer(&timer, NULL, 10000, 10000);

// 	server.process();

// 	return 1;
// }

