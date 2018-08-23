#ifndef UDP_POOL_H
#define UDP_POOL_H

#include "udp_chunk.h"

#define CHUNK_POOL_MAX_SIZE 1024

typedef struct udp_chunk_pool {
	udp_chunk_t *head;
	udp_chunk_t *tail;
	int size;
} udp_chunk_pool_t;

#ifdef __cplusplus
extern "C" {
#endif

void udp_chunk_pool_init(udp_chunk_pool *pool);

udp_chunk_t* udp_chunk_pool_malloc(udp_chunk_pool_t *pool);

void udp_chunk_pool_free(udp_chunk_pool_t *pool, udp_chunk_t *chunk);

void udp_chunk_pool_destroy(udp_chunk_pool *pool);

#ifdef __cplusplus
}
#endif

#endif