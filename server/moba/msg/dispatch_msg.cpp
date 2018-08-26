#include "dispatch_msg.h"
#include "message/messages.h"
#include "service.h"
#include "client_msg.h"
#include "server_msg.h"

#define REGISTER_BIND(func, caller) \
	std::tr1::bind(&func, \
	caller, \
	std::tr1::placeholders::_1, \
	std::tr1::placeholders::_2)

dispatch_msg_t::dispatch_msg_t(service_t *service) {
	service_ = service;
	client_msg_ = new client_msg_t();
	server_msg_ = new server_msg_t();
	udp_msg_ = new udp_msg_t();
}

dispatch_msg_t::~dispatch_msg_t() {
	service_ = NULL;
	delete client_msg_;
	client_msg_ = NULL;
	delete server_msg_;
	server_msg_ = NULL;
	delete udp_msg_;
	udp_msg_ = NULL;
}

void dispatch_msg_t::register_client_msg() {
	service_->register_net_message<battle_msg::c_login>(REGISTER_BIND(client_msg_t::c_login, client_msg_));	
	service_->register_net_message<battle_msg::c_login_hall>(REGISTER_BIND(client_msg_t::c_login_hall, client_msg_));	
}

void dispatch_msg_t::register_server_msg() {

}

void dispatch_msg_t::register_udp_msg() {
	service_->udp_register_cb(std::tr1::bind(udp_msg_t::on_message, udp_msg_, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));
}