#ifndef STRING_HASHTABLE_H
#define STRING_HASHTABLE_H 1

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
    void *key;
    size_t data;
};

/*
 * string_hashing:
 * is a implementation of
 * FNV hash doing hash on
 * 64-bit machine.
 */
void
string_hashing(struct hash_t *htab)
{
    char *str = htab->key;
    htab->data = 0;

    while (*str)
    {
        htab->data ^= *str++;
        htab->data *= FNV_PRIME;
    }
}


struct list_node
{
    struct hash_t htab;
    struct list_node *next;
};


extern struct list_node* (*createNode)(void *);                // create a new node for linked list

extern void insertAtBeginning(struct list_node*, void *);
extern void insertAtEnd(struct list_node*, void *);

extern void insertBeforeKey(struct list_node*, void *, void *);
extern void insertAfterKey(struct list_node*, void *, void *);

extern void deleteAtBeginning(struct list_node*, void *);
extern void deleteAtEnd(struct list_node*, void *);

extern void deleteBeforeKey(struct list_node*, void *);
extern void deleteAfterKey(struct list_node*, void *);

extern void free_nodes(struct list_node*);


/* https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash */

#endif
