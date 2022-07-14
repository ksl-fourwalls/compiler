#include <stdio.h>

#include "scanner.h"
#include "hash_tree_stack.h"

int main(int argc, char *argv[])
{
	hash_tree_stack *htstack = NULL;
	struct hash_t *htab;

	init_hash_tree(&htstack);
	insert_hash(&htstack->root, "fucker");

	htab = search_hash(&htstack->root, "fucker");
	if (htab)
		printf("0x%lx\n", htab->data);

	// destroy all trees in stack`
	for (; htstack != NULL; destroy_hash_tree(&htstack));

	{
		struct scanner_info info;

		if (argc != 2)
			errExit("File Name Required\n");

		init_scanner_info(&info, argv[1]);

		get_token(&info);

		free_scanner_info(&info);
	}

}
