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

#define SERVER

int tcp_port = 7001;
int udp_port = 7002;

// void test(service_t *service) {
// 	udp_network_t *udp_network = service->get_udp_network();
// 	udp_network->connect_to(1001, "127.0.0.1", serv_udp_port);
// 	char *s = "aaaaas"; 
// 	udp_network->send_sid(1001, (void *)s, 5);
// }

int main() {
	service_t *service = get_service();
	service->init();
	service->start_tcp("0.0.0.0", 7001);
	service->start_udp("0.0.0.0", 7002);
	service->process();
	return 1;
}