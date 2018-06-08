#ifndef LOG_H
#define LOG_H

#include "stdio.h"

#define ERROR(fmt, ...) printf("\033[31m[%s][%d]", __FILE__, __LINE__); printf(fmt, ##__VA_ARGS__); printf("\033[0m\n")
#define LOG(fmt, ...) printf("\37[37m[%s][%d]", __FILE__, __LINE__); printf(fmt, ##__VA_ARGS__); printf("\033[0m\n")

#endif
