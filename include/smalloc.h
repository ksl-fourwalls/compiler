#ifndef _SAME_SIZE_MALLOC
#define _SAME_SIZE_MALLOC 1

#ifndef SSIZE
#define SSIZE 100
#endif

#include <assert.h>
#include <sys/mman.h>

#define TOTAL_SMCHUNK_SIZE sizeof(struct smalloc_node)


/*
 * smalloc: allocator
 * is design for blocks
 * of same size which are
 * used frequently by an application
 */
struct smalloc_node {
  char userinp[SSIZE];
  struct smalloc_node *next;
};

struct {
  size_t nunits;                // size total number of allocated units
  struct smalloc_node* freelist;        // list of free region of memory
  struct smalloc_node* newchunk;        // new node is used to create a newnode 
                                // from mmaped memory
} memlist = { 0 };

static void morecore();
static void *smalloc();
static void sfree(void *);

/*
 * allocates memory with
 * mmap and free that
 * memory address into
 * free list for further
 * use
 */
static void morecore()
{
  void *mmap_memory;

  mmap_memory = mmap(0, 0x1000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  assert(mmap_memory != MAP_FAILED);

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

static void *smalloc()
{
  struct smalloc_node *free_list, *newchunk;

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
 
   Free list memory
   diagram 

   --------------------------------------------
   | data  | data | data | data | data | data |
   |-------|------|------|------|------|------|
   | next  | next | next | next | next | next |
   --------------------------------------------
      |       |      ^      |       ^     |
      |       |      |      |       |     |
      --------|-------      ---------     |
      ^       |             ^             |
      ---------             ---------------
 */
static void sfree(void *addr)
{
  // avoid double free
  if (addr != memlist.freelist)
  {
    ((struct smalloc_node *)addr)->next = memlist.freelist;
    memlist.freelist = addr;
  }
}
#endif

