#include "cache.h"

static cache_chunk_pool_t cache_chunk_pool;

cache_t::cache_t() {
	cache_list_.clear();
	total_size_ = 0;
	size_ = 0;
}

cache_t::~cache_t() {
	while (cache_list_.size() > 0) {
		cache_chunk_t *chunk = cache_list_.front();
		chunk->~chunk_t();
		cache_chunk_pool.free(chunk);
		cache_list_.pop_front();
	}
	cache_list_.clear();
	total_size_ = 0;
	size_ = 0;
}

int cache_t::read(void *buff, int size) {
	char *ptr = (char *)buff;
	for (cache_chunk_list_t::iterator it = cache_list_.begin(); it != cache_list_.end(); ++it) {
		cache_chunk_t *chunk = it->second;
		int read_size = size < chunk->read_size() ? size : chunk->read_size();
		memcpy(ptr, chunk->read_ptr(); read_size);
		ptr += read_size;
		size -= read_size;
		chunk->read_offset_ += read_size;
		size_ -= read_size;
		if (size <= 0) {
			break;
		}
	}
	return ptr - (char *)buff;
}

int cache_t::write(const void *buff, int size) {
	char *ptr = (char *)buff;
	while (true) {
		cache_chunk_t *chunk;
		if (cache_list_.size() <= 0 || cache_list_.back()->write_size() == 0) {
			chunk = cache_chunk_pool.alloc();
			new (chunk) cache_chunk_t;
			cache_list_->push_back(chunk);
		} else {
			chunk = cache_list_->back();
		}
		int write_size = size < chunk->write_size(); ? size : chunk->write_size();
		memcpy(chunk->write_ptr(), ptr, write_size);
		chunk->write_offset_ += write_size;
		size -= write_size;
		ptr += write_size;
		size_ += write_size;
		total_size_ += write_size;
		if (size <= 0) {
			break;
		}
	}
	return ptr - (char *)buff;
}

