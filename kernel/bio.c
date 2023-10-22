// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.


#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

struct {
  struct spinlock lock;
  struct buf buf[NBUF];
} bcache;

struct {
  struct spinlock lock[NBUCKET];
  struct buf bucket[NBUCKET];
}hashtbl_buf;

// key : block nmber
uint hash(uint key) {
  if (key == 0) 
    panic("hash panic\n");

  // printf("key = %d, index = %d\n", key, (key % NBUCKET));
  return key % NBUCKET;
}

void
binit(void)
{
  initlock(&bcache.lock, "bcache");

  for (int i = 0; i < NBUCKET; ++i) {
    initlock(&hashtbl_buf.lock[i], "bcache");
    hashtbl_buf.bucket[i].next = &hashtbl_buf.bucket[i];
    hashtbl_buf.bucket[i].prev = &hashtbl_buf.bucket[i];
  }
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
// static struct buf*
static struct buf*
bget(uint dev, uint blockno)
{
  uint index = hash(blockno);
  // Is the block already cached?
  acquire(&hashtbl_buf.lock[index]);
  struct buf *b = hashtbl_buf.bucket[index].next;
  while (b != &hashtbl_buf.bucket[index]) {
    if (b->dev == dev && b->blockno == blockno) {
      b->refcnt++;
      release(&hashtbl_buf.lock[index]);
      acquiresleep(&b->lock);
      return b;
    }
    b = b->next;
  }
  // release(&hashtbl_buf.lock[index]);

  // Not cached.  alloc a new buf and add to bucket.
  acquire(&bcache.lock);
  for (int i = 0; i < NBUF; ++i) {
    if(bcache.buf[i].refcnt == 0) {
      bcache.buf[i].dev = dev;
      bcache.buf[i].blockno = blockno;
      bcache.buf[i].valid = 0;
      bcache.buf[i].refcnt = 1;
      release(&bcache.lock);

      // add buf to bucket.
      // acquire(&hashtbl_buf.lock[index]);
      bcache.buf[i].prev = &hashtbl_buf.bucket[index];
      bcache.buf[i].next = hashtbl_buf.bucket[index].next;
      hashtbl_buf.bucket[index].next->prev = &bcache.buf[i];
      hashtbl_buf.bucket[index].next = &bcache.buf[i];
      release(&hashtbl_buf.lock[index]);

      acquiresleep(&bcache.buf[i].lock);
      return &bcache.buf[i];
    }
  }

  release(&hashtbl_buf.lock[index]);
  release(&bcache.lock);
  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if(!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");
  releasesleep(&b->lock);

  uint index = hash(b->blockno);

  acquire(&hashtbl_buf.lock[index]);
  // Do we need acquire bcache.lock?
  // acquire(&bcache.lock);
  b->refcnt--;
  if (b->refcnt == 0) {
    b->next->prev = b->prev;
    b->prev->next = b->next;
  }
  // release(&bcache.lock);
  release(&hashtbl_buf.lock[index]);
}

void
bpin(struct buf *b) {
  acquire(&bcache.lock);
  b->refcnt++;
  release(&bcache.lock);
}

void
bunpin(struct buf *b) {
  acquire(&bcache.lock);
  b->refcnt--;
  release(&bcache.lock);
}


