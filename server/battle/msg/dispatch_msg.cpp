#include "dispatch_msg.h"
#include "message/messages.h"
#include "service.h"
#include "client_msg.h"
#include "server_msg.h"

#define REGISTER_CLIENT_BIND(func, caller) \
	std::tr1::bind(&func, \
	caller, \
	std::tr1::placeholders::_1, \
	std::tr1::placeholders::_2)

dispatch_msg_t::dispatch_msg_t(service_t *service) {
	service_ = service;
	client_msg_ = new client_msg_t();
	server_msg_ = new server_msg_t();
}

dispatch_msg_t::~dispatch_msg_t() {
	
}

void dispatch_msg_t::register_client_msg() {
	service_->register_default_message(std::tr1::bind(&client_msg_t::default_message, client_msg_, std::tr1::placeholders::_1, std::tr1::placeholders::_2, std::tr1::placeholders::_3));	
	service_->register_net_message<battle_msg::s2c_join>(REGISTER_BIND(client_msg_t::s2c_join, client_msg_));	
	service_->register_net_message<battle_msg::c2s_join>(REGISTER_BIND(client_msg_t::c2s_join, client_msg_));	
	service_->register_net_message<battle_msg::c_login>(REGISTER_BIND(client_msg_t::c_login, client_msg_));	
}

void dispatch_msg_t::register_server_msg() {

}