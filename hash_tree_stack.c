#include "hash_tree_stack.h"

/*
 * hash tree stack works as follows
 *
 * 	---------------- <------ htstack (top)
 * 	|   htree[n]	|
 * 	----------------
 *	|   htree[n-1]	|-------------------------------------------   htree
 * 	----------------					    /-----  ------\
 *	|   htree[n-2]	|					child1 	child2
 * 	-----------------					  /  \	 	 /  \
 *	|    ...	|				      child3 child4  child5 child6
 * 	----------------
 *	|   htree[0]	|
 * 	----------------
 */

// initialize hash tree root and store it into
// stack from linked list.
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

// destroy tree and change top of the
// stack
void destroy_hash_tree(hash_tree_stack **htstack)
{
	if (*htstack)
	{
		tdestroy((*htstack)->root, free);
		*htstack = (*htstack)->next;
	}
}
