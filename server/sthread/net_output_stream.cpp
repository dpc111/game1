#include <string.h>
#include <stdlib.h>
#include "net_output_stream.h"
#include "tcp_connection.h"
#include "net_pool.h"

#define IOVEC_NUM 16

net_output_stream_t::net_output_stream_t(tcp_connection_t *conn) 
	: net_stream_t(conn)
	, size_(0) {
}

net_output_stream_t::~net_output_stream_t() {
	reset();
}

int net_output_stream_t::write(const void *buff, int size) {
	if (size <= 0) {
		return 0;
	}
	char *ptr = (char *)buff;
	while (true) {
		output_chunk_t *chunk;
		if (buff_.size() == 0 || buff_.back()->write_size() == 0) {
			chunk = output_chunk_alloc();
			chunk->conn_ = conn_;
			buff_.push_back(chunk);
		} 
		else {
			chunk = buff_.back();
		}
		int write_size = size < chunk->write_size() ? size : chunk->write_size();
		memcpy(chunk->write_ptr(), ptr, write_size);
		ptr += write_size;
		size -= write_size;
		size_ += write_size;
		if (size <= 0) {
			break;
		}
	}
	return (char *)buff - ptr;	
}

int net_output_stream_t::write_fd(void *ud, int fd) {
	tcp_connection_t *conn = (tcp_connection_t *)conn;
	iovec vecs[IOVEC_NUM];
	int i = 0;
	for (output_queue_t::iterator it = buff_.begin(); it != buff_.end(); ++it) {
		output_chunk_t *chunk = *it;
		vecs[i].iov_base = chunk->read_ptr();
		vecs[i].iov_len = chunk->read_size();
		i++;
		if (i >= IOVEC_NUM) {
			break;
		}
	}
	int n = ::writev(fd, vecs, i);
	if (n < 0) {
		return n;
	}
	int sz = n;
	while (!buff_.empty()) {
		output_chunk_t *chunk = buff_.front();
		if (sz < chunk->read_size()) {
			chunk->read_offset_ += sz;
			break;
		}
		sz -= chunk->read_size();
		output_chunk_free(chunk);
		buff_.pop_front();
	}
	size_ -= n;
	return n;
}

void net_output_stream_t::backup(int size) {
	for (output_queue_t::reverse_iterator it = buff_.rbegin(); it != buff_.rend(); ++it) {
        output_chunk_t *chunk = *it;
        if(chunk->write_offset_ == 0) {
            continue;
        }
        size_t backup_size = size <= chunk->write_offset_ ? size : chunk->write_offset_;
        chunk->write_offset_ -= backup_size;
        size_ -= backup_size;
        size -= backup_size;
        if (size <= 0) {
            break;
        }
    }
}

void net_output_stream_t::append(output_chunk_t *chunk) {
	buff_.push_back(chunk);
	size_ += chunk->read_size();
}

void net_output_stream_t::reset() {
	while (!buff_.empty()) {
		output_chunk_t *chunk = buff_.front();
		output_chunk_free(chunk);
		buff_.pop_front();
	}
	size_ = 0;
}


