struct chunk_pool_t {
	struct udp_chunk_t *head;
	struct udp_chunk_t *tail;
	int size = 0;
};

struct udp_chunk_t* 
chunk_pool_malloc(struct chunk_pool_t *pool) {
	udp_chunk_t* chunk = NULL;
	if (pool_->head) {
		chunk = pool->head;
		if (!chunk->next) {
			pool->head = NULL;
			pool->tail = NULL;
		} else {
			pool->head = chunk->next;
		}
		--pool->size;
	} else {
		udp_chunk_t *chunk = (udp_chunk_t *)malloc(sizeof(udp_chunk_t));
	}
	UDP_CHUNK_RESET(chunk);
	return chunk;
}

void 
chunk_pool_free(struct chunk_pool_t *pool, struct udp_chunk_t *chunk) {
	if (!pool->head) {
		pool->head = chunk;
	}
	if (!pool->tail) {
		pool->tail = chunk;
	}
	pool->tail->next = chunk;
	pool->tail = chunk;
	UDP_CHUNK_RESET(chunk);
	++pool->size;
	while (pool->size > CHUNK_POOL_MAX_SIZE) {
		udp_chunk_t *head = pool->head;
		pool->head = head->next; 
		free(head);
	}
}