#include "net_message.h"
#include "msg_stream.h"

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
	google::protobuf::Message *msg = NULL;
	// ???
	std::string& name = "test";
	const google::protobuf::Descriptor *des = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(name);
	if (des) {
		const google::protobuf::Message *tmsg = google::protobuf::MessageFactory::generated_factory()->GetPrototype(des);
 		if (tmsg) {
 			msg = tmsg->New(); 
 		}
	}
	if (!msg) {
		ERROR();
	}
	return msg;
}

void msg_operate_t::free_message(google::protobuf::Message *msg) {
	delete msg;
}

void msg_operate_t::send(const tcp_connection_t *conn, google::protobuf::Message& msg) {
	net_output_stream_t& stream = conn->get_output_stream();
	msg_header_t header;
	header.len = msg.ByteSize();
	header.sid = sid;
	header.tid = tid;
	header.msgid = msgid;
	stream.write(&header, sizeof(header));
	msg_output_stream_t os(stream);
	if (!msg->SerializeToZeroCopyStream(&os)) {
		ERROR();
		stream.backup((int)os.ByteCount());
		return false;
	}
	conn->add_event_write();
}

bool msg_operate_t::on_message(tcp_connection_t *conn) {
	net_input_stream_t& stream = conn_->get_input_stream();
	tcp_network_t *network = conn_->get_network();
	while (true) {
		int walk = 0;
		if (stream.size() <= sizeof(msg_header_t)) {
			break;
		}
		msg_header_t header;
		walk += stream.read(&header, sizeof(header));
		if (header.len < 0 || header.len > MSG_MAX_LEN) {
			ERROR();
			stream.reset();
			break;
		}
		if (header.len > stream.size()) {
			stream.backup(walk);
			break;
		}
		google::protobuf::Message *msg = this->gen_message(header.msgid);
		if (msg == NULL) {
			ERROR();
			stream.reset();
			break;
		}
		msg_input_stream_t is(stream, header.len);
		if (!msg->ParseFromZeroCopyStream(&is)) {
			ERROR();
			stream.reset();
			break;
		}
		network->get_msg_dispatch(conn, header.msgid, msg);
	}
	stream.finish();
}
