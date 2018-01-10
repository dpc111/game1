#include "stdio.h"
#include "log.h"
#include "thread.h"
#include "net_address.h"
#include "tcp_network.h"

#include "timer.h"
#include "timestamp.h"
#include "server.h"
#include "lua_frame.h"

server_t server("0.0.0.0", 7768);

class test_timer : public timer_handler_t {
	virtual void handle_timeout(timer_handle_t handle, void *user) {
		//printf("aaaaaahhshsdfdfs%ld\n", getms());
		int res;
		server.get_lua_frame()->call_func("test_add", "ii:i", 1, 2, &res);
		printf(".....%d\n", res);
	}
};

int main() {
	printf("server start\n");
	// tcp_network_t network("0.0.0.0", 7768);
	// network.start();

	server.get_lua_frame()->load_script("test.lua");
	server.start();
	test_timer timer;
	server.register_timer(&timer, NULL, 10000, 10000);
	server.process();

	printf("llllllllllsdssd\n");
	getchar();
	return 1;
}
