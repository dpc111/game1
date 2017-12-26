#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#pragma pack(push, 1)
struct msg_header_t {
	int len;
	int sid;
	int tid;
	int msgid;
	msg_header_t() : len(0), sid(0), tid(0), msgid(0) {}
}; 
#pragma pack(pop)

class pb_output_stream_t : public google::protobuf::io::ZeroCopyOutputStream {
public:
	pb_output_stream_t(net_output_stream_t& stream) {
		stream_ = stream;
		offset_ = 0;		
	}

	~pb_output_stream_t() {
	}

	virtual bool Next(void **data, int *size) {
		if (stream.next(data, size)) {
			offset_ += *size;
			return true;
		}
		return false;
	}

	virtual void BackUp(int num) {
		offset_ -= num;
		stream.backup(num);
	}

	virtual google::protobuf::int64 ByteCount() const {
		return offset_;
	}

private:
	net_output_stream_t& stream_;
	int offset_;
}

class net_message_t {
	static void send(const tcp_connection_t *conn, google::protobuf::Message& msg);

	bool on_message_cb(tcp_connection_t *conn);
}

#define