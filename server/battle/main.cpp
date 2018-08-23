#include "stdafx.h"
#include "log.h"
#include "thread.h"
#include "net_address.h"
#include "tcp_network.h"
#include "udp_network.h"
#include "tcp_connection.h"

#include "timer.h"
#include "timestamp.h"
#include "server.h"
#include "lua_frame.h"
#include "message/messages.h"
#include "service.h"
#include "global.h"

int main() {
	service_t *service = get_service();
	service->init();
	service->start_tcp("0.0.0.0", 7001);
	service->start_udp("0.0.0.0", 7002);
	test();
	service->process();
	return 1;
}

void test(service_t *service) {
	udp_network_t *udp_network = service->get_udp_network();
	udp_network->connect_to(1001, "127.0.0.1", 7102)
	const char *s = "aaaaas"; 
	udp_network->send_sid(1001, s, 5);
}