
#ifndef __UTIL_H__
#define __UTIL_H__ 1

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <search.h>

#define errExit(...) do {		\
	fprintf(stderr, __VA_ARGS__);	\
	exit(EXIT_FAILURE);		\
} while (0)

#endif
