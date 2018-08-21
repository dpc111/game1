#ifndef CHUNK_H
#define CHUNK_H

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