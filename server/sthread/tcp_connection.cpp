#include "tcp_connection.h"

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

void tcp_connection_t::set_events(event *ev_base, event_callback_fn read_fn, event_callback_fn write_fn) {
	ev_read_ = event_new(ev_base, fd, EV_READ | EV_PERSIST, read_fn, this);
	ev_write_ = event_new(ev_base, fd, EV_WRITE, write_fn, this);
	event_add(ev_read_, NULL);
	ev_read_add_ = true;
	// socket套接字关闭后，等到套接字内数据发送完成后才真正关闭连接
	linger lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	setsocketopt(fd, SOL_SOCKET, SO_LINGER, (const char *)&lin, sizeof(linger));
	// TCP_NODELAY 不使用Nagle算法，不会将小包进行拼接成大包再进行发送，直接将小包发送出去，会使得小包时候用户体验非常好。
	bool nodelay = true;
	setsocketopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char *)&nodelay, sizeof(char));
}

void tcp_connection_t::unset_events() {
	event_free(ev_read_);
	event_free(ev_write_);
	ev_read_ = NULL;
	ev_write_ = NULL;
}

void tcp_connection_t::add_event_write() {
	if (!ev_write_add_) {
		ev_write_add_ = true;
		event_add(ev_write_, NULL);
	}
}

void tcp_connection_t::del_event_write() {
	if (ev_write_add_) {
		ev_write_add_ = false;
		event_del(ev_write_);
	}
}