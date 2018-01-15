#include "log.h"
#include "server.h"
#include "timestamp.h"
#include "msg_operate.h"

server_t::server_t(const char *ip, int port) {
	network_ = new tcp_network_t(ip, port);
	conn_mgr_ = new conn_mgr_t(this);
	times_ = new timers_t();
	lua_frame_ = new lua_frame_t(this);
}

server_t::~server_t() {
	delete network_;
	delete conn_mgr_;
	delete times_;
	delete lua_frame_;
	network_ = NULL;
	conn_mgr_ = NULL;
	times_ = NULL;
	lua_frame_ = NULL;
}

void server_t::init() {
	conn_mgr_->init();
}

void server_t::start() {
	network_->start();
}

void server_t::process() {
	while (true) {
		network_->process();
		times_->process(getms());
		sleepms(1);
	}
}

tcp_connection_t *server_t::connect_to(const char *ip, int port, void *context) {
	return network_->connect_to(ip, port, context);
}

tcp_connection_t *server_t::connect_to(int sid) {
	return conn_mgr_->connect_to(sid);
}

void server_t::send(int sid, google::protobuf::Message& msg) {
	tcp_connection_t *conn = conn_mgr_->get_conn(sid);
	if (!conn) {
		conn = conn_mgr_->connect_to(sid);
	}
	if (!conn) {
		return;
	}
	network_->get_msg_operate()->send(conn, msg);
}

void server_t::send(tcp_connection_t *conn, google::protobuf::Message& msg) {
	network_->get_msg_operate()->send(conn, msg);
}

void server_t::send_func(tcp_connection_t *conn, const char *funcname, const char *fmt, ...) {
	va_list vlist;
	va_start(vlist, fmt);
	int len = 0;
	len += sizeof(int);
	len += strlen(funcname);
	len += sizeof(int);
	len += strlen(fmt);
	const char *walk = fmt;
	while (*walk != '\0') {
		switch (*walk) {
		case 'i' :
			len += sizeof(int);
			len += sizeof(int);
			va_arg(vlist, int);
		case 'd' :
			len += sizeof(int);
			len += sizeof(double);
			va_arg(vlist, double);
		case 's' :
			len += sizeof(int);
			len += strlen((char *)vlist);
			va_arg(vlist, char *);
		default :
			ERROR("");
			break;
		}
		++walk;
	}
	va_start(vlist, fmt);
	network_->get_msg_operate()->send_func(conn, funcname, fmt, vlist, len);
	va_end(vlist);
}

void server_t::register_timer(timer_handler_t *handler, void *user, timestamp start, timestamp interval) {
	times_->add(handler, user, start, interval);
}
