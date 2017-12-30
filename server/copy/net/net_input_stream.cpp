#include <string.h>
#include <stdlib.h>
#include "chunk.h"
#include "tcp_connection.h"
#include "net_thread.h"

net_input_stream_t::net_input_stream_t(tcp_connection_t *conn) : net_stream_t(conn) {
	size_ = 0;
}

net_input_stream_t::~net_input_stream_t() {
	reset();
}

int net_input_stream_t::read(void *buff, int size) {
	char *ptr = (char *)buff;
	for (input_queue_t::iterator it = buff_.begin(); it != buff_.end(); ) {
		input_chunk_t *chunk = *it;
		int read_size = size < chunk->read_size() ? size : chunk->read_size(); 
		memcpy(ptr, chunk->read_ptr(), read_size);
		ptr += read_size;
		size -= read_size;
		chunk->read_offset_ += read_size;
		size_ -= read_size;
		if (chunk->read_size() == 0) {
			if (buff_.size() == 1) {
				chunk->read_offset_ = 0;
				chunk->write_offset_ = 0;
				break;
			} else {
				input_queue_t::iterator it1 = it;
				++it;
				buff_.erase(it1);
				conn_->get_thread()->input_pool_free(chunk);
			}
		}
		if (size <= 0) {
			break;
		}
	}
	return ptr - (char *)buff;
}

int net_input_stream_t::write(const void *buff, int size) {
	return 0;
}

void net_input_stream_t::reset() {
	net_thread_t *thread = conn_->get_thread();
	while (!buff_.empty()) {
		input_chunk_t *chunk = buff_.front();
		thread->input_pool_free(chunk);
		buff_.pop_front();
	}
	size_ = 0;
}

void net_input_stream_t::backup(int size) {
	
}

int net_input_stream_t::read_fd(void *ud, int fd) {
	tcp_connection_t *conn = (tcp_connection_t *)ud;
	net_thread_t *thread = conn->get_thread();
	input_queue_t& read_fd_buff = thread->assign_read_fd_buff();
	iovec vecs[NUM_READ_IOVEC];
	int i = 0;
	for (input_queue_t::iterator it = read_fd_buff.begin(); it != read_fd_buff.end(); ) {
		input_chunk_t *chunk = *it;
		vecs[i].iov_base = chunk->write_ptr();
		vecs[i].iov_len = chunk->write_size();
		i++;
		if (i == NUM_READ_IOVEC) {
			break;
		}
	}
	int n = readv(fd, vecs, i);
	if (n <= 0) {
		reset();
		return n;
	}
	int read_size = n;
	while (read_size > 0) {
		if (read_fd_buff.empty()) {
			return -1;
		}
		input_chunk_t *chunk = read_fd_buff.front();
		read_fd_buff.pop_front();
		buff_.push_back(chunk);
		if (read_size < chunk->total_size()) {
			chunk->write_offset_ += read_size;
			read_size = 0;
			break;
		} else {
			chunk->write_offset_ += chunk->total_size();
			read_size -= chunk->total_size();
		}
	}
	size_ += n;
	return n;
}
