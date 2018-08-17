struct chunk_pool_t {
	struct chunk_t *head;
	struct chunk_t *tail;
	int size = 0;
};

struct chunk_t* 
chunk_pool_malloc(struct chunk_pool_t *pool) {
	chunk_t* chunk = NULL;
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
		chunk = (chunk_t *)malloc(sizeof(chunk_t));
	}
	chunk->next = NULL;
	return chunk;
}

void 
chunk_pool_free(struct chunk_pool_t *pool, struct chunk_t *chunk) {
	if (chunk == NULL) {
		return;
	}
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
		chunk_t *head = pool->head;
		pool->head = head->next; 
		free(head);
	}
}