#ifndef UDP_POOL_H
#define UDP_POOL_H

#include "udp_chunk.h"

#define CHUNK_POOL_MAX_SIZE 1024

struct udp_chunk_pool_t {
	struct udp_chunk_t *head;
	struct udp_chunk_t *tail;
	int size;
};

struct udp_chunk_t* chunk_pool_malloc(struct udp_chunk_pool_t *pool);

void chunk_pool_free(struct udp_chunk_pool_t *pool, struct udp_chunk_t *chunk)

#endif