#include "dispatch_msg.h"
#include "message/messages.h"
#include "service.h"

dispatch_msg_t::dispatch_msg_t(service_t service) {
	service_ = service;
	client_msg_ = new client_msg_t();
	server_msg_ = new server_msg_t();
}

dispatch_msg_t::~dispatch_msg_t() {
	
}

void dispatch_msg_t::register_client_msg() {
	server.register_net_message<battle::s2c_join>(std::tr1::bind(&client_msg_t::test_msg, &client_msg_, std::tr1::placeholders::_1, std::tr1::placeholders::_2));	
}

void dispatch_msg_t::register_server_msg() {

}