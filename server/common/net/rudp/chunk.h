#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>

#define UDP_CHUNK_READ_PTR(chunk) (chunk->buff + chunk->read_offset)
#define UDP_CHUNK_WRITE_PTR(chunk) (chunk->buff + chunk->write_offset)
#define UDP_CHUNK_READ_SIZE(chunk) (chunk->write_offset - chunk->read_offset)
#define UDP_CHUNK_WRITE_SIZE(chunk) (UDP_DATA_MAX_LEN - chunk->write_offset)
#define UDP_CHUNK_WRITE(chunk, buff, size) \
memcpy(UDP_CHUNK_WRITE_PTR(chunk), buff, size); \
chunk->write_offset += size; \
#define UDP_CHUNK_RESET(chuck) \
chunk->read_offset = 0; \
chunk->write_offset = 0; \
chunk->next = NULL; \

struct udp_chunk_t {
	char buff[UDP_DATA_MAX_LEN];

	int read_offset;

	int write_offset;

	struct udp_chunk_t *next;
};

#endif