#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>

// #define UDP_CHUNK_READ_PTR(chunk) (chunk->buff + chunk->read_offset)
// #define UDP_CHUNK_WRITE_PTR(chunk) (chunk->buff + chunk->write_offset)
// #define UDP_CHUNK_READ_SIZE(chunk) (chunk->write_offset - chunk->read_offset)
// #define UDP_CHUNK_WRITE_SIZE(chunk) (UDP_DATA_MAX_LEN - chunk->write_offset)
// #define UDP_CHUNK_WRITE(chunk, buff, size) \
// memcpy(UDP_CHUNK_WRITE_PTR(chunk), buff, size); \
// chunk->write_offset += size; \
// #define UDP_CHUNK_RESET(chuck) \
// chunk->read_offset = 0; \
// chunk->write_offset = 0; \
// chunk->next = NULL; \

// struct udp_chunk_t {
// 	char buff[UDP_DATA_MAX_LEN];

// 	int read_offset;

// 	int write_offset;

// 	struct udp_chunk_t *next;
// };


// struct chunk_t {
// 	struct chunk_t *prev;
// 	struct chunk_t *next;
// 	char buff[UDP_DATA_MAX_LEN];
// 	int size;
// 	int type;
// 	int seq;
// 	int ack;
// };

#if defined(_WIN32)
    typedef     __int8              int8;
    typedef     __int16             int16;
    typedef     __int32             int32;
    typedef     __int64             int64;
#else
    typedef     char                int8;
    typedef     short               int16;
    typedef     int                 int32;
    typedef     long long           int64;
#endif

struct chunk_t {
	int8 size;									// size of buff
	int8 type;
	int32 seq;
	int32 ack;
	char buff[UDP_DATA_MAX_LEN];
	struct chunk_t *prev;
	struct chunk_t *next;
};

#endif