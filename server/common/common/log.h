#ifndef LOG_H
#define LOG_H

#include "stdio.h"

//#define ERROR() printf("error\n")
#define ERROR(...) printf(##__VA_ARGS__)

#endif
