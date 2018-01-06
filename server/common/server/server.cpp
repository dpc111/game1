#include "server.h"

server_t::server_t(const char *ip, int port) {
	network_ = new tcp_network_t(ip, port);
	conn_mgr_ = new conn_mgr_t(this);
	times_ = new timers_t();
}

server_t::~server_t() {
	network_ = NULL;
	conn_mgr_ = NULL;
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

void server_t::send(int sid, google::protobuf::Message& msg) {
	tcp_connection_t *conn = conn_mgr_->get_conn(sid);
	if (!conn) {
		conn = conn_mgr_->connect_to(sid);
	}
	if (!conn) {
		return;
	}
	network_->send(conn, msg);
}

void server_t::send(tcp_connection_t *conn, google::protobuf::Message& msg) {
	network_->send(conn, msg);
}

void server_t::register_timer(timer_handler_t *handler, void *user, timestamp start, timestamp interval) {
	times_->add(handler, user, start, interval);
}
