#include "hash_tree_stack.h"

void init_hash_tree(hash_tree_stack** htstack)
{
	hash_tree_stack* tmp;

	// allocate new hash tree and store on 
	// top of the tree stack
	tmp = malloc(sizeof(struct hash_tree));
	if (tmp == NULL)
		errExit("unable to create new hashtree on top\n");

	tmp->root = NULL;
	tmp->next = *htstack;

	// set head of stack to newly allocated tree
	*htstack = tmp;
}

void destroy_hash_tree(hash_tree_stack * htree)
{
	if (htree)
	{
		tdestroy(htree->root, free);
		htree = htree->next;
	}
}
