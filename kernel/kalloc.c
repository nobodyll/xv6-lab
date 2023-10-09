// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

struct {
  struct spinlock lock;
  uint pgref[PHYSTOP / PGSIZE];
} pgref;

void incref(uint64 pa) {
  acquire(&pgref.lock);
  pgref.pgref[pa / PGSIZE]++;
  release(&pgref.lock);
}

void decref(uint64 pa){
  acquire(&pgref.lock);
  pgref.pgref[pa / PGSIZE]--;
  release(&pgref.lock);
}

uint getref(uint64 pa) {
  acquire(&pgref.lock);
  uint ref = pgref.pgref[pa / PGSIZE];
  release(&pgref.lock);
  return ref;
}

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  initlock(&pgref.lock, "pgref");

  for (int i = 0; i < (PHYSTOP / PGSIZE); i++) {
    pgref.pgref[i] = 1;
  }

  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // only when the page's ref equal zero. then free the page.
  decref((uint64)pa);
  if (getref((uint64)pa) == 0) {
    // Fill with junk to catch dangling refs.
    memset(pa, 1, PGSIZE);

    r = (struct run *)pa;

    acquire(&kmem.lock);
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
  } else if (getref((uint64)pa) < 0) {
    panic("kfree panic\n");
  }
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  incref((uint64)r);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
