// #include "stdio.h"
// #include "log.h"
// #include "msg_operate.h"
// #include "msg_stream.h"
// #include "tcp_network.h"
// #include "msg_dispatch.h"

// #define MSG_MAX_LEN 10240
// #define MSG_TYPE_PB 0x1
// #define MSG_TYPE_SCRIPT 0x2 

// #pragma pack(push, 1)
// struct msg_header_t {
// 	int len;
// 	int sid;
// 	int tid;
// 	int msgid;
// 	int msgtype;
// 	msg_header_t() : len(0), sid(0), tid(0), msgid(0), msgtype(MSG_TYPE_PB) {}
// }; 
// #pragma pack(pop)

// msg_operate_t::msg_operate_t(tcp_network_t *network) {
// 	network_ = network;
// }

// msg_operate_t::~msg_operate_t() {
// }

// google::protobuf::Message *msg_operate_t::gen_message(int msgid) {
// 	const char *name = network_->get_msg_dispatch()->msg_name(msgid);
// 	if (!name) {
// 		ERROR("");
// 		return NULL;
// 	}
// 	google::protobuf::Message *msg = NULL;
// 	const google::protobuf::Descriptor *des = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(name);
// 	if (des) {
// 		const google::protobuf::Message *tmsg = google::protobuf::MessageFactory::generated_factory()->GetPrototype(des);
//  		if (tmsg) {
//  			msg = tmsg->New(); 
//  		}
// 	}
// 	return msg;
// }

// void msg_operate_t::free_message(google::protobuf::Message *msg) {
// 	delete msg;
// }

// void msg_operate_t::send(tcp_connection_t *conn, google::protobuf::Message& msg) {
// 	std::string name = msg.GetDescriptor()->full_name();
// 	int msgid = network_->get_msg_dispatch()->msg_id(name.c_str());
// 	net_output_stream_t& stream = conn->get_output_stream();
// 	msg_header_t header;
// 	header.len = msg.ByteSize();
// 	header.sid = network_->get_sid();
// 	header.tid = conn->get_sid();
// 	header.msgid = msgid;
// 	header.msgtype = MSG_TYPE_PB;
// 	stream.write(&header, sizeof(header));
// 	msg_output_stream_t os(stream);
// 	if (!msg.SerializeToZeroCopyStream(&os)) {
// 		ERROR("");
// 		stream.backup((int)os.ByteCount());
// 		return;
// 	}
// 	conn->add_event_write();
// }

// void msg_operate_t::send_func(tcp_connection_t *conn, const char *funcname, const char *fmt, va_list vlist, int len) {
// 	net_output_stream_t& stream = conn->get_output_stream();
// 	msg_header_t header;
// 	header.len = len;
// 	header.sid = network_->get_sid();
// 	header.tid = conn->get_sid();
// 	header.msgid = 0;
// 	header.msgtype = MSG_TYPE_SCRIPT;
// 	stream.write(&header, sizeof(header));
// 	int ilen = sizeof(int);
// 	int dlen = sizeof(double);
// 	int slen = 0;
// 	slen = strlen(funcname) + 1;
// 	stream.write(&slen, ilen);
// 	stream.write(funcname, slen);
// 	slen = strlen(fmt) + 1;
// 	stream.write(&slen, ilen);
// 	stream.write(fmt, slen);
// 	const char *walk = fmt;
// 	while (*walk != '\0') {
// 		switch (*walk) {
// 		case 'i' :
// 			stream.write(&ilen, ilen);
// 			stream.write((int *)vlist, ilen);
// 			va_arg(vlist, int);
// 			break;
// 		case 'd' :
// 			stream.write(&dlen, ilen);
// 			stream.write(&vlist, dlen);
// 			va_arg(vlist, double);
// 			break;
// 		case 's' :
// 			slen = strlen((char *)vlist) + 1;
// 			stream.write(&slen, ilen);
// 			stream.write((char *)vlist, slen);
// 			va_arg(vlist, char *);
// 			break;
// 		default :
// 			ERROR("");
// 			stream.reset();
// 			return;
// 		}
// 		++walk;
// 	}
// 	conn->add_event_write();
// }

// bool msg_operate_t::on_message(tcp_connection_t *conn) {
// 	net_input_stream_t& stream = conn->get_input_stream();
// 	tcp_network_t *network = conn->get_network();
// 	while (true) {
// 		int walk = 0;
// 		if (stream.size() <= sizeof(msg_header_t)) {
// 			break;
// 		}
// 		msg_header_t header;
// 		walk += stream.read(&header, sizeof(header));
// 		if (header.len < 0 || header.len > MSG_MAX_LEN) {
// 			ERROR("%d", header.len);
// 			stream.reset();
// 			// break;
// 			return false;
// 		}
// 		if (header.len > stream.size()) {
// 			stream.backup(walk);
// 			break;
// 		}
// 		if (header.msgtype == MSG_TYPE_PB) {
// 			google::protobuf::Message *msg = gen_message(header.msgid);
// 			if (msg == NULL) {
// 				ERROR("");
// 				stream.reset();
// 				// break;
// 				return false;
// 			}
// 			msg_input_stream_t is(stream, header.len);
// 			if (!msg->ParseFromZeroCopyStream(&is)) {
// 				ERROR("");
// 				stream.reset();
// 				free_message(msg);
// 				// break;
// 				return false;
// 			}
// 			network->get_msg_dispatch()->on_message(conn, header.msgid, msg);
// 			free_message(msg);
// 		} else if (header.msgtype == MSG_TYPE_SCRIPT) {
// 			bool ok = network->get_msg_dispatch()->on_script_func(conn);
// 			if (!ok) {
// 				ERROR("");
// 				stream.reset();
// 				// break;
// 				return false;
// 			}
// 		} 
// 	}
// 	stream.finish();
// 	return true;
// }


#include "stdio.h"
#include "log.h"
#include "msg_operate.h"
#include "msg_stream.h"
#include "tcp_network.h"
#include "msg_dispatch.h"

#define MSG_MAX_LEN 10240
#define MSG_TYPE_PB 0x1
#define MSG_TYPE_SCRIPT 0x2 

#pragma pack(push, 1)
struct msg_header_t {
	int name_len;
	int len;
	int msg_type;
	int sid;
	int tid;
	msg_header_t() : name_len(0), len(0), msg_type(MSG_TYPE_PB), sid(0), tid(0) {}
}; 
#pragma pack(pop)

msg_operate_t::msg_operate_t(tcp_network_t *network) {
	network_ = network;
}

msg_operate_t::~msg_operate_t() {
}

google::protobuf::Message *msg_operate_t::gen_message(const char *name) {
	google::protobuf::Message *msg = NULL;
	const google::protobuf::Descriptor *des = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(name);
	if (des) {
		const google::protobuf::Message *tmsg = google::protobuf::MessageFactory::generated_factory()->GetPrototype(des);
 		if (tmsg) {
 			msg = tmsg->New(); 
 		}
	}
	return msg;
}

void msg_operate_t::free_message(google::protobuf::Message *msg) {
	delete msg;
}

void msg_operate_t::send(tcp_connection_t *conn, google::protobuf::Message& msg) {
	std::string name = msg.GetDescriptor()->full_name();
	net_output_stream_t& stream = conn->get_output_stream();
	msg_header_t header;
	header.name_len = strlen(name.c_str()) + 1;
	header.len = msg.ByteSize();
	header.msg_type = MSG_TYPE_PB;
	header.sid = network_->get_sid();
	header.tid = conn->get_sid();
	stream.write(&header, sizeof(header));
	stream.write(name.c_str(), header.name_len);
	msg_output_stream_t os(stream);
	if (!msg.SerializeToZeroCopyStream(&os)) {
		ERROR("");
		stream.backup((int)os.ByteCount());
		return;
	}
	conn->add_event_write();
}

// void msg_operate_t::send_func(tcp_connection_t *conn, const char *funcname, const char *fmt, va_list vlist, int len) {
// 	net_output_stream_t& stream = conn->get_output_stream();
// 	msg_header_t header;
// 	header.len = len;
// 	header.name_len = strlen(funcname) + 1;
// 	header.msg_type = MSG_TYPE_SCRIPT;
// 	header.sid = network_->get_sid();
// 	header.tid = conn->get_sid();
// 	stream.write(&header, sizeof(header));
// 	stream.write(funcname, header.name_len);
// 	int ilen = sizeof(int);
// 	int dlen = sizeof(double);
// 	int slen = 0;
// 	slen = strlen(funcname) + 1;
// 	stream.write(&slen, ilen);
// 	stream.write(funcname, slen);
// 	slen = strlen(fmt) + 1;
// 	stream.write(&slen, ilen);
// 	stream.write(fmt, slen);
// 	const char *walk = fmt;
// 	while (*walk != '\0') {
// 		switch (*walk) {
// 		case 'i' :
// 			stream.write(&ilen, ilen);
// 			stream.write((int *)vlist, ilen);
// 			va_arg(vlist, int);
// 			break;
// 		case 'd' :
// 			stream.write(&dlen, ilen);
// 			stream.write(&vlist, dlen);
// 			va_arg(vlist, double);
// 			break;
// 		case 's' :
// 			slen = strlen((char *)vlist) + 1;
// 			stream.write(&slen, ilen);
// 			stream.write((char *)vlist, slen);
// 			va_arg(vlist, char *);
// 			break;
// 		default :
// 			ERROR("");
// 			stream.reset();
// 			return;
// 		}
// 		++walk;
// 	}
// 	conn->add_event_write();
// }
// 

 void msg_operate_t::send_func(tcp_connection_t *conn, const char *funcname, const char *fmt, va_list vlist, int len) {
	net_output_stream_t& stream = conn->get_output_stream();
	msg_header_t header;
	header.len = len;
	header.name_len = strlen(funcname) + 1;
	header.msg_type = MSG_TYPE_SCRIPT;
	header.sid = network_->get_sid();
	header.tid = conn->get_sid();
	stream.write(&header, sizeof(header));
	stream.write(funcname, header.name_len);
	int ilen = sizeof(int);
	int dlen = sizeof(double);
	int slen = 0;
	// slen = strlen(funcname) + 1;
	// stream.write(&slen, ilen);
	// stream.write(funcname, slen);
	slen = strlen(fmt) + 1;
	stream.write(&slen, ilen);
	stream.write(fmt, slen);
	const char *walk = fmt;
	while (*walk != '\0') {
		switch (*walk) {
		case 'i' :
			stream.write(&ilen, ilen);
			stream.write((int *)vlist, ilen);
			va_arg(vlist, int);
			break;
		case 'd' :
			stream.write(&dlen, ilen);
			stream.write(&vlist, dlen);
			va_arg(vlist, double);
			break;
		case 's' :
			slen = strlen((char *)vlist) + 1;
			stream.write(&slen, ilen);
			stream.write((char *)vlist, slen);
			va_arg(vlist, char *);
			break;
		default :
			ERROR("");
			stream.reset();
			return;
		}
		++walk;
	}
	conn->add_event_write();
}

bool msg_operate_t::on_message(tcp_connection_t *conn) {
	net_input_stream_t& stream = conn->get_input_stream();
	tcp_network_t *network = conn->get_network();
	while (true) {
		int walk = 0;
		if (stream.size() <= sizeof(msg_header_t)) {
			break;
		}
		msg_header_t header;
		walk += stream.read(&header, sizeof(header));
		if (header.len < 0 || header.len > MSG_MAX_LEN) {
			ERROR("%d", header.len);
			stream.reset();
			// break;
			return false;
		}
		if (header.name_len > MSG_NAME_MAX_LEN) {
			ERROR("%d", header.name_len);
			stream.reset();
			return false;
		}
		if (header.len + header.name_len > stream.size()) {
			stream.backup(walk);
			break;
		}
		stream.read(msg_name_, header.name_len);
		if (header.msg_type == MSG_TYPE_PB) {
			google::protobuf::Message *msg = gen_message(msg_name_);
			if (msg == NULL) {
				ERROR("");
				stream.reset();
				// break;
				return false;
			}
			msg_input_stream_t is(stream, header.len);
			if (!msg->ParseFromZeroCopyStream(&is)) {
				ERROR("");
				stream.reset();
				free_message(msg);
				// break;
				return false;
			}
			network->get_msg_dispatch()->on_message(conn, msg);
			free_message(msg);
		} else if (header.msg_type == MSG_TYPE_SCRIPT) {
			bool ok = network->get_msg_dispatch()->on_script_func(conn, msg_name_);
			if (!ok) {
				ERROR("");
				stream.reset();
				// break;
				return false;
			}
		} 
	}
	stream.finish();
	return true;
}
