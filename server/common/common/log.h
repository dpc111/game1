#ifndef LOG_H
#define LOG_H

#include "stdio.h"

//#define ERROR() printf("error\n")
#define ERROR(fmt, ...) printf("%s %d ", __FILE__, __LINE__); printf(fmt, ##__VA_ARGS__); printf("\n")

#endif
