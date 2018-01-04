#include "stdio.h"
#include "log.h"
#include "thread.h"
#include "net_address.h"
#include "tcp_network.h"

// net_address_t

int main() {
	printf("server start\n");
	ERROR();
	lock_t lk;
	tcp_network_t network("0.0.0.0", 7768);
	network.start();
	printf("llllllllllsdssd\n");
	getchar();
	return 1;
}
