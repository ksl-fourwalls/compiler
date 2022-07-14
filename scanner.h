#ifndef SCANNER_H
#define SCANNER_H 1

#include "include/utils.h"
#include "hash_tree_stack.h"

#define BUFFER_SIZE 1024

struct scanner_info {
	FILE *filp;
	char buffer[BUFFER_SIZE];
};

// intiailize scanner info struct
void init_scanner_info(struct scanner_info*, const char *);

// delete file object
void free_scanner_info(struct scanner_info*);


/**
 * read bytes from file descriptor
 * of scanner_info struct
 */
static uint64_t safe_read(struct scanner_info *info);

/*
 * return next character
 * from the queue buffer
 */
char next_chr(struct scanner_info* info);

struct token* get_token(struct scanner_info *info);
#endif
