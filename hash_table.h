#ifndef STRING_HASHTABLE_H
#define STRING_HASHTABLE_H 1

#include "include/utils.h"

#define FNV_PRIME   1099511628211
#define FNV_OFFSET  14695981039346656037

/* 
 * hash table uses FNV-1a 
 * for faster performance of compiler
 * and use linked list in order
 * make dynamic sized hash table.
 *
 */
struct hash_t
{
	char *key;
	size_t data;
};

struct hash_tree {
	void *root;
};

// initialize hash treee struct with 
// zeros
void init_hash_tree(struct hash_tree *);

// destroy hash tree free each hash tables
void destroy_hash_tree(struct hash_tree *);

// insert struct into hash tree
void insert_hash(struct hash_tree *, const char*);

// find string in hash table and returns
// correponding hash struct
struct hash_t *find_hash(struct hash_tree *, const char *);

// uses FNV hash to hash a nul terminated
// string
void string_hashing(struct hash_t *);
#endif
