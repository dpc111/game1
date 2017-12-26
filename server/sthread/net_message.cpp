#include "net_message.h"

void net_message::send(const tcp_connection_t *conn, google::protobuf::Message& msg) {
	net_output_stream_t& stream = conn->get_output_stream();
	msg_header_t header;
	header.len = msg.ByteSize();
	header.sid = sid;
	header.tid = tid;
	header.msgid = msgid;
	stream.write(&header, sizeof(header));
	pb_output_stream_t os(stream);
	if (!msg->SerializeToZeroCopyStream(&os)) {
		ERROR();
		stream.backup((int)os.ByteCount());
		return false;
	}
	conn->incr_write_event();
}