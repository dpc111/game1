#ifndef UDP_POOL_H
#define UDP_POOL_H

#include "udp_chunk.h"

#define CHUNK_POOL_MAX_SIZE 1024

typedef struct {
	udp_chunk_t *head;
	udp_chunk_t *tail;
	int size;
} udp_chunk_pool_t;

#ifdef __cplusplus
extern "C" {
#endif

udp_chunk_t* chunk_pool_malloc(udp_chunk_pool_t *pool);

void chunk_pool_free(udp_chunk_pool_t *pool, udp_chunk_t *chunk)

#ifdef __cplusplus
}
#endif

#endif