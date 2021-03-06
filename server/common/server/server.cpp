#include "log.h"
#include "server.h"
#include "timestamp.h"
#include "msg_operate.h"

server_t::server_t() {
	tcp_network_ = new tcp_network_t();
	udp_network_ = new udp_network_t();
	conn_mgr_ = new conn_mgr_t(this);
	// times_ = new timers_t();
	timer_axis_ = new timer_axis_t();
	lua_frame_ = new lua_frame_t(this);
	json_mgr_ = new cfg_json_mgr_t();	
	frame_last_tm_ = 0;
	frame_interval_ = 0;
	tcp_network_->set_on_disconnect_cb(std::tr1::bind(&server_t::on_disconnect, this, std::tr1::placeholders::_1));
	udp_network_->set_udp_connect_cb(std::tr1::bind(&server_t::udp_on_connect, this, std::tr1::placeholders::_1));
	udp_network_->set_udp_disconnect_cb(std::tr1::bind(&server_t::udp_on_disconnect, this, std::tr1::placeholders::_1));
}

server_t::~server_t() {
	delete tcp_network_;
	tcp_network_ = NULL;
	delete udp_network_;
	udp_network_ = NULL;
	delete conn_mgr_;
	conn_mgr_ = NULL;
	delete timer_axis_;
	timer_axis_ = NULL;
	delete lua_frame_;
	lua_frame_ = NULL;
	// delete times_;
	// times_ = NULL;
}

void server_t::init() {
	conn_mgr_->init();
	this->init_json_mgr();
}

void server_t::init_json_mgr() {

}

void server_t::start_tcp(const char *ip, int port) {
	tcp_network_->start(ip, port);
}

void server_t::start_udp(const char *ip, int port) {
	udp_network_->start(ip, port);
}

void server_t::process() {
	while (true) {
		int64 tm = getms();
		if (tcp_network_->running()) {
			tcp_network_->process(tm);
		}
		if (udp_network_->running()) {
			udp_network_->process(tm);
		}
		// times_->process(tm);
		timer_axis_->process();
		if (frame_interval_ > 0 && tm - frame_last_tm_ > frame_interval_) {
			frame_last_tm_ = tm;
			this->update(tm);
		}
		sleepms(1);
	}
}

void server_t::set_frame_interval(int64 interval) {
	frame_interval_ = interval;
}

tcp_connection_t *server_t::connect_to(const char *ip, int port, void *context) {
	return tcp_network_->connect_to(ip, port, context);
}

tcp_connection_t *server_t::connect_to(int sid) {
	return conn_mgr_->connect_to(sid);
}

void server_t::verify(int32 sid, tcp_connection_t *conn) {
	conn_mgr_->verify(sid, conn);
}

void server_t::send(int sid, google::protobuf::Message& msg) {
	tcp_connection_t *conn = conn_mgr_->get_conn(sid);
	if (!conn) {
		conn = conn_mgr_->connect_to(sid);
	}
	if (!conn) {
		LOG("send %s to %d : disconnect", msg.GetDescriptor()->full_name().c_str(), sid);
		return;
	}
	tcp_network_->get_msg_operate()->send(conn, msg);
}

void server_t::send(tcp_connection_t *conn, google::protobuf::Message& msg) {
	tcp_network_->get_msg_operate()->send(conn, msg);
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
	tcp_network_->get_msg_operate()->send_func(conn, funcname, fmt, vlist, len);
	va_end(vlist);
}

// timer_handle_t server_t::register_timer(timer_handler_t *handler, void *user, timestamp start, timestamp interval) {
// 	return times_->add(handler, user, start, interval);
// }

// timer_handle_t server_t::register_delay_stimer(timer_handler_t *handler, void *user, timestamp delay, timestamp interval) {
// 	return times_->add(handler, user, delay * 1000 + getms(), interval);
// }

// timer_handle_t server_t::register_delay_mstimer(timer_handler_t *handler, void *user, timestamp delay, timestamp interval) {
// 	return times_->add(handler, user, delay + getms(), interval);
// }

void server_t::on_disconnect(tcp_connection_t *conn) {
	conn_mgr_->on_disconnect(conn);
}