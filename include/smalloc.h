#ifndef _SAME_SIZE_MALLOC
#define _SAME_SIZE_MALLOC 1

#ifndef SSIZE
#define SSIZE 100
#endif

#define TOTAL_SMCHUNK_SIZE sizeof(smalloc_node)


#define errExit(msg) do { \
  perror(msg);            \
  exit(EXIT_FAILURE);     \
} while (0)

/*
 * smalloc: allocator
 * is design for blocks
 * of same size which are
 * used frequently by an application
 */
smalloc_node {
  char userinp[SSIZE];
  smalloc_node *next;
};

struct {
  size_t nunits;                // size total number of allocated units
  smalloc_node* freelist;        // list of free region of memory
  smalloc_node* newchunk;        // new node is used to create a newnode 
                                // from mmaped memory
} memlist = { 0 };


/*
 * allocates memory with
 * mmap and free that
 * memory address into
 * free list for further
 * use
 */
void morecore()
{
  void *mmap_memory;
  void sfree(void *);

  mmap_memory = mmap(0, 0x1000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  if (mmap_memory == MAP_FAILED)
    errExit("malloc");

  memlist.newchunk = mmap_memory+TOTAL_SMCHUNK_SIZE;
  memlist.nunits = 0x1000/TOTAL_SMCHUNK_SIZE-1;

  sfree(mmap_memory);
}

/*
 * smalloc: allocate memory firstly from the
 * freelist or else it will frees and returns next 
 * chunk from the mmaped_memory
 *
 * mmaped_memory
 *
 * freed chunk
 * morecore
 * |         -------------next call malloc frees
 * |         |            next chunk address
 * v         v
 * -------------------------------------------
 * | chunk 1 |  chunk 2 |   ...    |  ...    |
 * -------------------------------------------
 * |         |          |          |         |
 * -------------------------------------------
 *
 */

void *smalloc()
{
  smalloc_node *free_list, *newchunk;
  void sfree(void *);

  for (;;) {
    free_list = memlist.freelist;

    // chunk from free_list
    if (free_list) 
    {
      memlist.freelist = free_list->next;
      return free_list;
    }

    // check number of remaining
    // units in our newchunks
    if (memlist.nunits--)
    {
      newchunk = memlist.newchunk++;
      sfree(newchunk);
      continue;
    }

    morecore();
  }
}

/*
 * adds address to
 * singly same sized
 * free list
 */
void sfree(void *addr)
{
  if (addr != memlist.freelist)
  {
    ((smalloc_node *)addr)->next = memlist.freelist;
    memlist.freelist = addr;
  }
}
#endif

