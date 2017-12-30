#ifndef NET_STREAM_H
#define NET_STREAM_H

class tcp_connection_t;

class net_stream_t
{
public:
	net_stream_t(tcp_connection_t *conn) { conn_ = conn; }

	virtual ~net_stream_t() {}

	virtual int read(void *buff, int size) = 0;

	virtual int write(const void *buff, int size) = 0;

	virtual void reset() = 0;

	virtual int size() const = 0;

	tcp_connection_t* get_conn() { return conn_; }

public:
	tcp_connection_t *conn_;
};

#endif