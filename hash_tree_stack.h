#ifndef __HASH_TREE_STACK_H_
#define __HASH_TREE_STACK_H_ 1

#include "hash_table.h"


typedef struct hash_tree {
	void *root;
	struct hash_tree *next;
} hash_tree_stack;

void init_hash_tree(hash_tree_stack **);
void destroy_hash_tree(hash_tree_stack **);

#endif
