#include <stdio.h>
#include "hash_table.h"

int main(int argc, char *argv[])
{
	struct hash_tree htree;
	struct hash_t *htab;

	init_hash_tree(&htree);
	insert_hash(&htree, "fucker");

	htab = find_hash(&htree, "fucker");
	if (htab)
	{
		printf("%s\n", htab->key);
	}

	destroy_hash_tree(&htree);
}
