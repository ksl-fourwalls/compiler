#ifndef __SAME_SIZE_MALLOC
#define __SAME_SIZE_MALLOC 1

#define SETSIZE(x) memlist.size = x

/*
 * smalloc: allocator
 * is design for blocks
 * of same size which are
 * used frequently by an application
 */
struct node {
  void *data;
  struct node *next;
};

struct {
  unsigned size;                // size of one block
  size_t nunits;                // size total number of allocated units

  struct node* freelist;        // list of free region of memory

  struct node* newnode;         // new node is used to create a newnode 
                                // from mmaped memory

  struct node* mmap_list;       // pointer to mmap allocated block
} 
memlist = { 0 };

/*
 * allocate large chunk of memory
 *
 * which contains small chunks of equal size which
 * speed up our process of allocating memory without
 * caring about what size of each is every time and
 * we do this haveing one freelist and one mmap_list
 * which contains poin
 *  -----------------------------------------------------
 *  | chunk 1   | chunk 2    |    chunk 3  |  chunk 4   |
 *  |-----------|------------|-------------|------------|
 *  | chunk 5   | chunk 6    |    chunk 7  |   ...      |
 *  |-----------|------------|-------------|------------|
 *  |           |            |             |            |
 *  |-----------|------------|-------------|------------|
 *  |           |            |             |            |
 *  |-----------|------------|-------------|------------|
 *  | ...       | ...        |    ...      |   ...      |
 *  -----------------------------------------------------
 *
 */


/*
 * fire we check in free list
 * whether any of 
 */
void *
smalloc(size_t size)
{
  struct node* mtmp;
  void *morecore();
 
  mtmp = memlist.freelist;
  while (mtmp != NULL)
  {
    memlist.freelist = mtmp->next;
    return mtmp;
  }

  while (memlist.nunits)
  {
    memlist.nunits -= memlist.size;
    memlist.data += memlist.size;

    return memlist.data;
  }
  return morecore();
}

void *sfree(void *freedMem)
{
  struct node* mtmp;
  mtmp = memlist.freelist;

  mtmp = &((struct node *)freedMem)->next;
  memlist.freelist = mtmp;
}

void *morecore()
{
  struct node* mtmp;
  void *addr;

  memlist.nunits += (0x1000+memlist.size) / (memlist.size);

  addr = mmap(0, 0x1000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  if (addr == MAP_FAILURE)
  {
    fprintf(stderr, "MAP_FAILURE\n");
    exit(EXIT_FAILURE);
  }

  mtmp = memlist.mmap_list.next;

  memlist.mmap_list.data = addr+sizeof(struct node);
  memlist.mmap_list.next = addr;

  ((struct node *)addr)->next = mtmp;

  return memlist.mmap_list.data;
}


#endif
