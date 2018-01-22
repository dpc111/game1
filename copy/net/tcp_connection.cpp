#include "tcp_connection.h"
#include "net_stream.h"
#include "net_input_stream.h"
#include "net_output_stream.h"
#include "net_address.h"
#include "net_thread.h"
#include "tcp_network.h"

tcp_connection_t::tcp_connection_t(int fd, sockaddr_in peer_addr) 
	: input_stream_(this)
	, output_stream_(this)
	, fd_(fd)
	, peer_addr_(peer_addr)
	, closed_(false) {
}

tcp_connection_t::~tcp_connection_t() {
	closed_ = true;
}
