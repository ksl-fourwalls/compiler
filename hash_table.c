#include "hash_table.h"

/*
 * string_hashing:
 * is a implementation of
 * FNV hash doing hash on
 * 64-bit machine.
 */
void string_hashing(struct hash_t * new_hash)
{
	char* str = new_hash->key;

	new_hash->data = 0;
	for (; *str != '\0'; str++)
	{
		new_hash->data ^= *str;
		new_hash->data *= FNV_PRIME;
	}
}

static int compare(const void *pa, const void *pb)
{
	if (((struct hash_t*)pa)->data < ((struct hash_t *)pb)->data)
		return -1;
	if (((struct hash_t*)pa)->data > ((struct hash_t *)pb)->data)
		return 1;
	return 0;
}


void insert_hash(struct hash_tree* htree, const char* str)
{
	struct hash_t* htab ;

	htab = malloc(sizeof(struct hash_t));
	if (!htab)
		errExit("insufficient memory to allocate hash\n");

	htab->key = (char *)str;
	string_hashing(htab); 

	if (tsearch((void *) htab, &htree->root, compare) == NULL)
		errExit("Unable to insert hash %lx\n", htab->data);
}

void init_hash_tree(struct hash_tree* htree) 
{ 
	memset(htree, 0, sizeof(struct hash_tree));
}

void destroy_hash_tree(struct hash_tree* htree)
{
	tdestroy(htree->root, free);
}

struct hash_t *find_hash(struct hash_tree *htree, const char* str)
{
	struct hash_t htab = {
		.key = (char *)str
	};
	string_hashing(&htab);
	return *(void **)tfind((void *)&htab, &htree->root, compare);
}
