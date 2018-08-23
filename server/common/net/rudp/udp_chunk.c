#include "udp_chunk.h"

void
udp_chunk_init(udp_chunk_t *c) {
	c->size = 0;
	c->type = 0;
	c->seq = 0;
	c->ack = 0;
	c->prev = NULL;
	c->next = NULL;
}
