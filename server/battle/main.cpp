#include "stdafx.h"
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
#include "service.h"

int main() {
	service_t *service = new service_t();
	service->init();
	service->start("0.0.0.0", 7769);
	service->process();
	return 1;
}