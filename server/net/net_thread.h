#ifndef NET_THREAD_H
#define NET_THREAD_H

#include "type.h"
#include "safe_queue.h"
#include "obj_pool.h"
#include "poller.h"
#include "chunk.h"
//#include "tcp_network.h"

#define NUM_READ_IOVEC 16

class tcp_connection_t;
class tcp_network_t;

class net_thread_t
{
public:
	typedef safe_queue_t<input_chunk_t *> input_squeue_t;
	typedef safe_queue_t<output_chunk_t *> output_squeue_t;
	typedef obj_pool_t<input_chunk_t> input_pool_t;
	typedef obj_pool_t<output_chunk_t> output_pool_t;

public:
	net_thread_t(tcp_network_t *network);
	~net_thread_t();

	static int run(void *arg);
	void thread_process();
	void process(timestamp tm = 0);

	tcp_network_t* get_owner() { return network_; }
	poller_t* get_poller() { return poller_; }
	int get_conn_num() { return conn_num_; }

	static void read(int fd, void* ud);
	static void write(int fd, void* ud);

	input_chunk_t* input_pool_alloc();
	void input_pool_free(input_chunk_t *chunk);
	output_chunk_t* output_pool_alloc();
	void output_pool_free(output_chunk_t *chunk);
	input_queue_t& assign_read_fd_buff();

	void on_conn_add(tcp_connection_t *conn);
	void on_conn_remove(tcp_connection_t *conn);

	#include "net_thread.inl"

private:
	tcp_network_t 				*network_;
	poller_t 					*poller_;

	input_pool_t 				input_pool_;
	output_pool_t 				output_pool_;

	input_squeue_t 				input_queue_;
	output_squeue_t 			output_queue_;

	input_queue_t 				read_fd_buff_;

	int 						conn_num_;
};

#endif