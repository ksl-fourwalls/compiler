#include <stdio.h>
#include "hash_tree_stack.h"

int main(int argc, char *argv[])
{
	hash_tree_stack *htstack = NULL;
	struct hash_t *htab;

	init_hash_tree(&htstack);
	destroy_hash_tree(htstack);
}
