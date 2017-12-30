#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "net_address.h"
#include "net_input_stream.h"
#include "net_output_stream.h"
//#include "net_thread.h"
//#include "tcp_network.h"

#define CNT_STATE_CONNECTING 		1
#define CNT_STATE_CONNECTED 		2
#define CNT_STATE_DISCONNECTED 		3

//class net_address_t;
class tcp_network_t;
class net_thread_t;
//class net_output_stream_t;
//class net_input_stream_t;

class tcp_connection_t
{
public:
	tcp_connection_t(int fd, sockaddr_in peer_addr);

	~tcp_connection_t();

	void set_network(tcp_network_t *network) { network_ = network; }

	tcp_network_t* get_network() { return network_; }

	void set_thread(net_thread_t *thread) { thread_ = thread; }

	net_thread_t* get_thread() { return thread_; }

	net_output_stream_t& get_output_stream() { return output_stream_; }

	net_input_stream_t& get_input_stream() { return input_stream_; }

	int get_fd() { return fd_; }

	bool is_closed() { return closed_; }

	void set_closed(bool bclose) { closed_ = bclose; }

	bool connected() { return state_ == CNT_STATE_CONNECTED; }

private:
	int 						fd_;
	int 						state_;
	bool 						closed_;
	tcp_network_t 				*network_;
	net_thread_t 				*thread_;
	net_output_stream_t 		output_stream_;
	net_input_stream_t 			input_stream_;
	net_address_t 				local_addr_;	
	net_address_t 				peer_addr_;
};

#endif