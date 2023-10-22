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
void new_freerange(void *pa_start, void *pa_end);
void new_kfree_for_kfrange(void *pa, int hartid);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

struct {
  struct spinlock lock[NCPU];
  struct run *freelist[NCPU];
  // for debug
  uint64 count[NCPU];
} new_kmem;

void 
new_kinit()
{
  for (int i = 0; i < NCPU; ++i) {
    initlock(&new_kmem.lock[i], "kmem");
  }
  new_freerange(end, (void*)PHYSTOP);
}

// physical memory freerange [pa_start , pa_end] ->  [end, PHYSTOP]
// (PHYSTOP - PGROUNDUP(end)) / NCPU

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

void
new_freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(int i = 0; p + PGSIZE <= (char*)pa_end;++i, p += PGSIZE) {
    new_kfree_for_kfrange(p, i % NCPU);
  }
  // for (int i = 0; i < NCPU; ++i) {
  //   printf("hartid: %d, count = %d\n", i, new_kmem.count[i]);
  // }
  // panic("1");
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
// void
// kfree(void *pa)
// {
//   struct run *r;

//   if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
//     panic("kfree");

//   // Fill with junk to catch dangling refs.
//   memset(pa, 1, PGSIZE);

//   r = (struct run*)pa;

//   acquire(&kmem.lock);
//   r->next = kmem.freelist;
//   kmem.freelist = r;
//   release(&kmem.lock);
// }

void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  int reopen = 0;
  if (intr_get() != 0) {
    // need reopen interrupt again
    reopen = 1;
  } 
  intr_off();

  int hartid = cpuid();

  r = (struct run*)pa;
  acquire(&new_kmem.lock[hartid]);
  r->next = new_kmem.freelist[hartid];
  new_kmem.freelist[hartid] = r;
  release(&new_kmem.lock[hartid]);

  if (reopen) {
    intr_on();
  }
}

void
new_kfree_for_kfrange(void *pa, int hartid) 
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  // acquire(&new_kmem.lock[hartid]);
  r->next = new_kmem.freelist[hartid];
  new_kmem.freelist[hartid] = r;
  new_kmem.count[hartid]++;
  // release(&new_kmem.lock[hartid]);
}


// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
// void *
// kalloc(void)
// {
//   struct run *r;

//   acquire(&kmem.lock);
//   r = kmem.freelist;
//   if(r)
//     kmem.freelist = r->next;
//   release(&kmem.lock);

//   if(r)
//     memset((char*)r, 5, PGSIZE); // fill with junk
//   return (void*)r;
// }

void *
kalloc(void)
{
  int reopen = 0;
  if (intr_get() != 0) {
    // need reopen interrupt again
    reopen = 1;
  } 
  intr_off();

  int hartid = cpuid();

  acquire(&new_kmem.lock[hartid]);
  struct run *r = new_kmem.freelist[hartid];
  if(r) {
    new_kmem.freelist[hartid] = r->next;
    // new_kmem.count[hartid]--;
    release(&new_kmem.lock[hartid]);
  } else {
    release(&new_kmem.lock[hartid]);
    // need steal mem from other core.
    int target = (hartid + 1) % NCPU;
    for (int j = 0; j < NCPU - 1; ++j) {
      acquire(&new_kmem.lock[target]);
      if (new_kmem.freelist[target] != 0) {
        r = new_kmem.freelist[target];
        new_kmem.freelist[target] = r->next;
        release(&new_kmem.lock[target]);
        break;
      }
      release(&new_kmem.lock[target]);
      target = (target + 1) % NCPU;
    }
  }

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk

  if (reopen) {
    intr_on();
  }

  return (void*)r;
}