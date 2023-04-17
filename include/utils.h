
#ifndef __UTIL_H__
#define __UTIL_H__ 1

#define _GNU_SOURCE

#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <ctype.h>

#include <unistd.h>


#define ARRAYSIZE(x) (sizeof(x)/sizeof(*x))

#define errExit(...) do {		\
	fprintf(stderr, __VA_ARGS__);	\
	exit(EXIT_FAILURE);		\
} while (0)

#endif
