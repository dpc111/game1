#ifndef NET_EVENT_H
#define NET_EVENT_H

#include <event2/event.h>
#include <event2/listener.h>

class net_event_t {
public:
	net_event_t();

	~net_event_t();

	void ev_close();

	void ev_start();

	static void ev_listen_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *ud);

	static void ev_read_cb(evutil_socket_t fd, const short which, void *arg);

	static void ev_write_cb(evutil_socket_t fd, const short which, void *arg);

protected:
	event_base *ev_base_;

	evconnlistener *ev_listen_;
};

#endif