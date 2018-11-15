#ifndef CACHE_H
#define CACHE_H

#include "chunk.h"
#include "obj_pool.h"
#include "thread_private_pool.h"

typedef chunk_t<10240> cache_chunk_t;

typedef std::list<cache_chunk_t *> cache_chunk_list_t;

typedef thread_private_pool_t<cache_chunk_t> cache_chunk_pool_t;

class cache_t {
public:
	cache_t();

	~cache_t();

	int read(void *buff, int size);

	int write(const void *buff, int size);

private:
	cache_chunk_list_t cache_list_;

	int total_size_;

	int size_;
};

#endif