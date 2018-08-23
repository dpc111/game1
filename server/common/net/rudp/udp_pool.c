#include "udp_pool.h"

void 
udp_chunk_pool_init(udp_chunk_pool *pool) {
	pool->head = NULL;
	pool->tail = NULL;
	pool->size = 0;
}

udp_chunk_t* 
udp_chunk_pool_malloc(udp_chunk_pool_t *pool) {
	udp_chunk_t* chunk = NULL;
	if (pool->head) {
		chunk = pool->head;
		if (!chunk->next) {
			pool->head = NULL;
			pool->tail = NULL;
		} else {
			pool->head = chunk->next;
		}
		--pool->size;
	} else {
		chunk = (udp_chunk_t *)malloc(sizeof(udp_chunk_t));
		udp_chunk_init(chunk);
	}
	chunk->next = NULL;
	return chunk;
}

void 
udp_chunk_pool_free(udp_chunk_pool_t *pool, udp_chunk_t *chunk) {
	if (chunk == NULL) {
		return;
	}
	udp_chunk_init(chunk);
	if (pool->tail == NULL) {
		pool->head = chunk;
		pool->tail = chunk;
		chunk->next = NULL;
	} else {
		pool->tail->next = chunk;
		pool->tail = chunk;
		chunk->next = NULL;
	}
	++pool->size;
	while (pool->size > CHUNK_POOL_MAX_SIZE) {
		udp_chunk_t *head = pool->head;
		pool->head = head->next; 
		free(head);
	}
}

void 
udp_chunk_pool_destroy(udp_chunk_pool *pool) {
	udp_chunk_t *c = pool->head;
	while (c != NULL) {
		pool->head = c->next;
		free(c);
		c = pool->head;
	}
	pool->tail = NULL;
	pool->size = 0;
}