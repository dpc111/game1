#include "chunk.h"

#define CHUNK_POOL_MAX_SIZE 1024

struct chunk_t* chunk_pool_malloc(struct chunk_pool_t *pool);

void chunk_pool_free(struct chunk_pool_t *pool, struct chunk_t *chunk)
