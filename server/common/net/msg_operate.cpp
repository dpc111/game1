#include "stdio.h"
#include "log.h"
#include "msg_operate.h"
#include "msg_stream.h"
#include "tcp_network.h"
#include "msg_dispatch.h"

#define MSG_MAX_LEN 10240

#pragma pack(push, 1)
struct msg_header_t {
	int len;
	int sid;
	int tid;
	int msgid;
	msg_header_t() : len(0), sid(0), tid(0), msgid(0) {}
}; 
#pragma pack(pop)

msg_operate_t::msg_operate_t(tcp_network_t *network) {
	network_ = network;
}

msg_operate_t::~msg_operate_t() {
}

google::protobuf::Message *msg_operate_t::gen_message(int msgid) {
	const char *name = network_->get_msg_dispatch()->msg_name(msgid);
	if (!name) {
		ERROR("");
		return NULL;
	}
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
	int msgid = network_->get_msg_dispatch()->msg_id(name.c_str());
	net_output_stream_t& stream = conn->get_output_stream();
	msg_header_t header;
	header.len = msg.ByteSize();
	header.sid = network_->get_sid();
	header.tid = conn->get_sid();
	header.msgid = msgid;
	stream.write(&header, sizeof(header));
	msg_output_stream_t os(stream);
	if (!msg.SerializeToZeroCopyStream(&os)) {
		ERROR("");
		stream.backup((int)os.ByteCount());
		return;
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
			ERROR("");
			stream.reset();
			break;
		}
		if (header.len > stream.size()) {
			stream.backup(walk);
			break;
		}
		google::protobuf::Message *msg = gen_message(header.msgid);
		if (msg == NULL) {
			ERROR("");
			stream.reset();
			break;
		}
		msg_input_stream_t is(stream, header.len);
		if (!msg->ParseFromZeroCopyStream(&is)) {
			ERROR("");
			stream.reset();
			free_message(msg);
			break;
		}
		network->get_msg_dispatch()->on_message(conn, header.msgid, msg);
		free_message(msg);
	}
	stream.finish();
}
