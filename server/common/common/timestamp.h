#ifndef TIEMSTAMP_H
#define TIEMSTAMP_H

// int64 timestamp_gettimeofday() {
// 	timeval tv;
// 	gettimeofday(&tv, NULL);
// 	return 1000000 * int64(tv.tv_sec) + int64(tv.tv_usec)
// }

int64 gettime();

int64 getus();

int64 getms();

float getfs();

void sleepms(int64 ms);

#endif
