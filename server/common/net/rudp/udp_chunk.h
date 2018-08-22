#ifndef UPP_CHUNK_H
#define UPP_CHUNK_H

#define UDP_DATA_MAX_LEN 512                    // Internet(非局域网)上的标准MTU值为576字节 (控制在548字节 (576-8-20)以内)

#include <stdlib.h>

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

typedef struct udp_chunk_t {
	int8 size;									// size of buff
	int8 type;
	int32 seq;
	int32 ack;
	char buff[UDP_DATA_MAX_LEN];
	udp_chunk_t *prev;
	udp_chunk_t *next;
} udp_chunk_t;

#endif