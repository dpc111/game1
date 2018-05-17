#include "dispatch_msg.h"
#include "message/messages.h"
#include "service.h"
#include "client_msg.h"
#include "server_msg.h"

dispatch_msg_t::dispatch_msg_t(service_t *service) {
	service_ = service;
	client_msg_ = new client_msg_t();
	server_msg_ = new server_msg_t();
}

dispatch_msg_t::~dispatch_msg_t() {
	
}

void dispatch_msg_t::register_client_msg() {
	service_->register_net_message<battle_msg::s2c_join>(std::tr1::bind(&client_msg_t::s2c_join, client_msg_, std::tr1::placeholders::_1, std::tr1::placeholders::_2));	
	service_->register_net_message<battle_msg::c2s_join>(std::tr1::bind(&client_msg_t::c2s_join, client_msg_, std::tr1::placeholders::_1, std::tr1::placeholders::_2));	
	service_->register_net_message<battle_msg::c_login>(std::tr1::bind(&client_msg_t::c_login, client_msg_, std::tr1::placeholders::_1, std::tr1::placeholders::_2));	
	service_->register_message<battle_msg::c_create_entity>(std::tr1::bind(&client_msg_t::c_create_entity, client_msg_, std::tr1::placeholders::_1, std::tr1::placeholders::_2));	
}

void dispatch_msg_t::register_server_msg() {

}