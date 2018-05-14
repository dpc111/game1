#include "log.h"
#include "server.h"
#include "timestamp.h"
#include "msg_operate.h"

server_t::server_t() {
	network_ = new tcp_network_t();
	conn_mgr_ = new conn_mgr_t(this);
	times_ = new timers_t();
	lua_frame_ = new lua_frame_t(this);
	json_mgr_ = new cfg_json_mgr_t();	
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
	json_mgr_->load("battle.json", "battle");
	ERROR("%d", json_mgr_->get_int("battle", 1, "began"));
	ERROR("%s", json_mgr_->get_string("battle", 3, "roomName").c_str());
	ERROR("%s", json_mgr_->get_string("battle", 6, "format").c_str());
	ERROR("%s", json_mgr_->get_string("battle", 30, "format").c_str());
	conn_mgr_->init();
}

void server_t::start(const char *ip, int port) {
	network_->start(ip, port);
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
	len += strlen(fmt) + 1;
	const char *walk = fmt;
	while (*walk != '\0') {
		if (*walk == 'i') {
			va_arg(vlist, int);
			len += sizeof(int);
			len += sizeof(int);
		} else if (*walk == 'd') {
			va_arg(vlist, double);
			len += sizeof(int);
			len += sizeof(double);
		} else if (*walk == 's') {
			char *s = va_arg(vlist, char *);
			len += sizeof(int);
			len += strlen((char *)vlist) + 1;
		} else {
			ERROR("");
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
