#ifndef NET_INPUT_STREAM_H
#define NET_INPUT_STREAM_H

#include "chunk.h"
#include "net_stream.h"

//struct chunk;
//class net_stream_t;

class net_input_stream_t : public net_stream_t
{
public:
	net_input_stream_t(tcp_connection_t *conn);

	virtual ~net_input_stream_t();

	virtual int read(void *buff, int size);

	virtual int write(const void *buff, int size);

	virtual void reset();

	virtual int size() const { return size_; }

	int read_fd(void *ud, int fd);

	void backup(int size);

private:
	input_queue_t buff_;
	int size_;
};

#endif
