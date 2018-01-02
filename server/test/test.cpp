#include "net_address.h"
#include "tcp_network.h"

// net_address_t

int main() {
	printf("server start\n");
	net_address_t addr("0.0.0.0", 7768);
	tcp_network_t network(addr);
	network.start();
	return 1;
}