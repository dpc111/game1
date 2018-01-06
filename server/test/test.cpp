#include "stdio.h"
#include "log.h"
#include "thread.h"
#include "net_address.h"
#include "tcp_network.h"

#include "timer.h"
#include "timestamp.h"
#include "server.h"

class test_timer : public timer_handler_t {
	virtual void handle_timeout(timer_handle_t handle, void *user) {
		printf("aaaaaahhshsdfdfs%ld\n", getms());
	}
};

int main() {
	printf("server start\n");
	// tcp_network_t network("0.0.0.0", 7768);
	// network.start();

	server_t server("0.0.0.0", 7768);
	server.start();
	test_timer timer;
	server.register_timer(&timer, NULL, 10000, 10000);
	server.process();

	printf("llllllllllsdssd\n");
	getchar();
	return 1;
}
