#ifndef SERVER_H
#define SERVER_H

#include "msg_dispatch.h"
#include "tcp_network.h"
#include "udp_network.h"
#include "conn_mgr.h"
// #include "timer.h"
#include "timer_axis.h"
#include "lua_frame.h"
#include "cfg_json_mgr.h"
#include "type.h"

class server_t {
public:
	server_t(const char *ip, int port);

	server_t();

	~server_t();

	virtual void init();

	virtual void init_json_mgr();

	virtual void update(int64 tm) = 0;

	void start_tcp(const char *ip, int port);

	void start_udp(const char *ip, int port);

	void start();

	void process();

	void set_frame_interval(int64 interval);

	tcp_network_t *get_network() { return tcp_network_; }

	udp_network_t *get_udp_network() { return udp_network_; }

	timer_axis_t *get_timer_axis() { return timer_axis_; }

	lua_frame_t *get_lua_frame() { return lua_frame_; }

	cfg_json_mgr_t* get_json_mgr() { return json_mgr_; }

	tcp_connection_t *connect_to(const char *ip, int port, void *context = NULL);

	tcp_connection_t *connect_to(int sid);

	void verify(int32 sid, tcp_connection_t *conn);

	void send(int sid, google::protobuf::Message& msg);

	void send(tcp_connection_t *conn, google::protobuf::Message& msg);

	void send_func(tcp_connection_t *conn, const char *funcname, const char *fmt, ...);

	// timer_handle_t register_timer(timer_handler_t *handler, void *user, timestamp start, timestamp interval);

	// timer_handle_t register_delay_stimer(timer_handler_t *handler, void *user, timestamp delay, timestamp interval);

	// timer_handle_t register_delay_mstimer(timer_handler_t *handler, void *user, timestamp delay, timestamp interval);

	template<typename T>
	void register_message(const typename msg_dispatch_t::cbT_t<T>::msg_cb_t& cb) { tcp_network_->get_msg_dispatch()->register_message<T>(cb); }

	template<typename T>
	void register_net_message(const typename msg_dispatch_t::cb_netT_t<T>::msg_cb_t& cb) { tcp_network_->get_msg_dispatch()->register_net_message<T>(cb); }

	void register_default_message(const msg_dispatch_t::msg_default_cb_t& cb) { tcp_network_->get_msg_dispatch()->register_default_message(cb); }

	void on_disconnect(tcp_connection_t *conn);

	void udp_send_sid(int64 sid, void *buff, int32 size) { udp_network_->send_sid(sid, buff, size); }

	void udp_register_cb(const udp_network_t::udp_msg_cb_t& cb) { udp_network_->set_udp_msg_cb(cb); }

	virtual void udp_on_connect(int sid) {}

	virtual void udp_on_disconnect(int sid) {}

private:
	tcp_network_t *tcp_network_;

	udp_network_t *udp_network_;

	conn_mgr_t *conn_mgr_;

	// timers_t *times_;
	timer_axis_t *timer_axis_;

	lua_frame_t *lua_frame_;

	cfg_json_mgr_t *json_mgr_;

	int64 frame_last_tm_;

	int64 frame_interval_;
};

#endif
