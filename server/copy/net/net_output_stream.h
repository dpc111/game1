#ifndef NET_OUTPUT_STREAM_H
#define NET_OUTPUT_STREAM_H

//#include "thread.h"
#include "thread.h"
#include "chunk.h"
#include "net_stream.h"
//#include "tcp_connection.h"

//class net_stream_t;
class tcp_connection_t;
class net_thread_t;

class net_output_stream_t : public net_stream_t
{
public:
	net_output_stream_t(tcp_connection_t *conn);

	virtual ~net_output_stream_t();

	virtual int read(void *buff, int size);

	virtual int write(const void *buff, int size);

	virtual void reset();

	virtual int size() const { return size_; }

	int write_fd(void *ud, int fd);
	
	void backup(int size);

	void append(output_chunk_t *chunk);

private:
	output_queue_t buff_;
	lock_t stream_lock_;
	int size_;
};

#endif