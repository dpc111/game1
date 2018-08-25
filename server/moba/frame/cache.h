#ifndef CACHE_H
#define CACHE_H

#include "chunk.h"
#include "obj_pool.h"

typedef chunk_t<10240> cache_chunk_t;

typedef std::list<cache_chunk_t *> cache_chunk_list_t;

typedef obj_pool_t<cache_chunk_t> cache_chunk_pool_t;

class cache_t {
public:
	cache_t();

	~cache_t();

	void read(void *buff, int size);

	void write(void *buff, int size);

private:
	cache_chunk_list_t cache_list_;

	int size_;
};

#endif