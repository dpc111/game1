#ifndef NET_INPUT_STREAM_H
#define NET_INPUT_STREAM_H

#include "chunk.h"
#include "net_stream.h"

class net_input_stream_t
{
public:
	net_input_stream_t(tcp_connection_t *conn);

	~net_input_stream_t();

	tcp_connection_t *get_conn() { return conn_; }

	int read(void *buff, int size);

	void reset();

	int size() const { return size_; }

	int read_fd(void *ud, int fd);

	void backup(int size);

private:
	tcp_connection_t 			*conn_;
	input_queue_t 				buff_;
	int 						size_;
};

#endif
