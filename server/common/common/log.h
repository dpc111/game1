#ifndef LOG_H
#define LOG_H

#include "stdio.h"

//#define ERROR() printf("error\n")
#define ERROR(fmt, ...) printf(fmt, ##__VA_ARGS__); printf("\n")

#endif
