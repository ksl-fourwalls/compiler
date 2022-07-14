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


void insert_hash(void *root, const char* str)
{
	struct hash_t* htab ;

	htab = malloc(sizeof(struct hash_t));
	if (!htab)
		errExit("insufficient memory to allocate hash\n");

	htab->key = (char *)str;
	string_hashing(htab); 

	if (*(void **)tsearch((void *) htab, root, compare) == NULL)
		errExit("Unable to store hash in hashtree\n");
}

struct hash_t *search_hash(void *root, const char* str)
{
	struct hash_t htab;

	htab.key = (char *)str;
	string_hashing(&htab);

	return *(void **)tfind((void *) &htab, root, compare);
}

