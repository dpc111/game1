#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "net_address.h"
#include "net_input_stream.h"
#include "net_output_stream.h"

#include <event2/event.h>
#include <event2/event_struct.h>

#define CNT_STATE_CONNECTING 		1
#define CNT_STATE_CONNECTED 		2
#define CNT_STATE_DISCONNECTED 		3

class tcp_network_t;
class net_thread_t;

class tcp_connection_t
{
public:
	tcp_connection_t(int fd, sockaddr_in& peer_addr);

	tcp_connection_t();

	~tcp_connection_t();

	bool reliable();

	void set_network(tcp_network_t *network) { network_ = network; }

	tcp_network_t* get_network() { return network_; }

	net_output_stream_t& get_output_stream() { return output_stream_; }

	net_input_stream_t& get_input_stream() { return input_stream_; }

	void set_peer_addr(sockaddr_in& peer_addr) { peer_addr_ = peer_addr; }

	void set_context(void *context) { context_ = context; }

	void* get_context() const { return context_; }

	int set_fd(int fd) { fd_ = fd; }

	int get_fd() { return fd_; }

	bool is_closed() { return closed_; }

	void set_closed(bool bclose) { closed_ = bclose; }

	bool connected() { return state_ == CNT_STATE_CONNECTED; }

	void set_events(event_base *ev_base, event_callback_fn read_fn, event_callback_fn write_fn);

	void unset_events();

	void add_event_write();

	void del_event_write();

	void set_peer_id(int peer_id) { peer_id_ = peer_id; }

	int get_peer_id() { return peer_id_; }

private:
	tcp_network_t *network_;

	net_output_stream_t output_stream_;

	net_input_stream_t input_stream_;

	net_address_t peer_addr_;

	void *context_;

	int fd_;

	int state_;

	bool closed_;

	event *ev_read_;

	event *ev_write_;

	bool ev_read_add_;
	
	bool ev_write_add_;

	int peer_id_;
};

#endif
