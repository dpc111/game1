#ifndef TIEMSTAMP_H
#define TIEMSTAMP_H

#include <sys/time.h>
#include <time.h>
#include <asm/unistd.h>
#include "type.h"

// int64 timestamp_gettimeofday() {
// 	timeval tv;
// 	gettimeofday(&tv, NULL);
// 	return 1000000 * int64(tv.tv_sec) + int64(tv.tv_usec)
// }

int64 gettime() {
	int64 tm;
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	tm = int64(ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
	return tm;
}

int64 getus() {
	return gettime();
}

int64 getms() {
	return getus() / 1000;
}

void sleepmsg(int64 ms) {
	struct timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = msg % 1000 * 1000;
	if (select(0, NULL, NULL, NULL, &tv) == -1) {
		printf("sleep fail\n");
	}
} 

#endif
