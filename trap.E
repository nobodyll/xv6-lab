# 0 "kernel/trap.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "kernel/trap.c"
# 1 "kernel/types.h" 1
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;
# 2 "kernel/trap.c" 2
# 1 "kernel/param.h" 1
# 3 "kernel/trap.c" 2
# 1 "kernel/memlayout.h" 1
# 4 "kernel/trap.c" 2
# 1 "kernel/riscv.h" 1



static inline uint64
r_mhartid()
{
  uint64 x;
  asm volatile("csrr %0, mhartid" : "=r" (x) );
  return x;
}
# 20 "kernel/riscv.h"
static inline uint64
r_mstatus()
{
  uint64 x;
  asm volatile("csrr %0, mstatus" : "=r" (x) );
  return x;
}

static inline void
w_mstatus(uint64 x)
{
  asm volatile("csrw mstatus, %0" : : "r" (x));
}




static inline void
w_mepc(uint64 x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}
# 51 "kernel/riscv.h"
static inline uint64
r_sstatus()
{
  uint64 x;
  asm volatile("csrr %0, sstatus" : "=r" (x) );
  return x;
}

static inline void
w_sstatus(uint64 x)
{
  asm volatile("csrw sstatus, %0" : : "r" (x));
}


static inline uint64
r_sip()
{
  uint64 x;
  asm volatile("csrr %0, sip" : "=r" (x) );
  return x;
}

static inline void
w_sip(uint64 x)
{
  asm volatile("csrw sip, %0" : : "r" (x));
}





static inline uint64
r_sie()
{
  uint64 x;
  asm volatile("csrr %0, sie" : "=r" (x) );
  return x;
}

static inline void
w_sie(uint64 x)
{
  asm volatile("csrw sie, %0" : : "r" (x));
}





static inline uint64
r_mie()
{
  uint64 x;
  asm volatile("csrr %0, mie" : "=r" (x) );
  return x;
}

static inline void
w_mie(uint64 x)
{
  asm volatile("csrw mie, %0" : : "r" (x));
}




static inline void
w_sepc(uint64 x)
{
  asm volatile("csrw sepc, %0" : : "r" (x));
}

static inline uint64
r_sepc()
{
  uint64 x;
  asm volatile("csrr %0, sepc" : "=r" (x) );
  return x;
}


static inline uint64
r_medeleg()
{
  uint64 x;
  asm volatile("csrr %0, medeleg" : "=r" (x) );
  return x;
}

static inline void
w_medeleg(uint64 x)
{
  asm volatile("csrw medeleg, %0" : : "r" (x));
}


static inline uint64
r_mideleg()
{
  uint64 x;
  asm volatile("csrr %0, mideleg" : "=r" (x) );
  return x;
}

static inline void
w_mideleg(uint64 x)
{
  asm volatile("csrw mideleg, %0" : : "r" (x));
}



static inline void
w_stvec(uint64 x)
{
  asm volatile("csrw stvec, %0" : : "r" (x));
}

static inline uint64
r_stvec()
{
  uint64 x;
  asm volatile("csrr %0, stvec" : "=r" (x) );
  return x;
}


static inline void
w_mtvec(uint64 x)
{
  asm volatile("csrw mtvec, %0" : : "r" (x));
}


static inline void
w_pmpcfg0(uint64 x)
{
  asm volatile("csrw pmpcfg0, %0" : : "r" (x));
}

static inline void
w_pmpaddr0(uint64 x)
{
  asm volatile("csrw pmpaddr0, %0" : : "r" (x));
}
# 206 "kernel/riscv.h"
static inline void
w_satp(uint64 x)
{
  asm volatile("csrw satp, %0" : : "r" (x));
}

static inline uint64
r_satp()
{
  uint64 x;
  asm volatile("csrr %0, satp" : "=r" (x) );
  return x;
}

static inline void
w_mscratch(uint64 x)
{
  asm volatile("csrw mscratch, %0" : : "r" (x));
}


static inline uint64
r_scause()
{
  uint64 x;
  asm volatile("csrr %0, scause" : "=r" (x) );
  return x;
}


static inline uint64
r_stval()
{
  uint64 x;
  asm volatile("csrr %0, stval" : "=r" (x) );
  return x;
}


static inline void
w_mcounteren(uint64 x)
{
  asm volatile("csrw mcounteren, %0" : : "r" (x));
}

static inline uint64
r_mcounteren()
{
  uint64 x;
  asm volatile("csrr %0, mcounteren" : "=r" (x) );
  return x;
}


static inline uint64
r_time()
{
  uint64 x;
  asm volatile("csrr %0, time" : "=r" (x) );
  return x;
}


static inline void
intr_on()
{
  w_sstatus(r_sstatus() | (1L << 1));
}


static inline void
intr_off()
{
  w_sstatus(r_sstatus() & ~(1L << 1));
}


static inline int
intr_get()
{
  uint64 x = r_sstatus();
  return (x & (1L << 1)) != 0;
}

static inline uint64
r_sp()
{
  uint64 x;
  asm volatile("mv %0, sp" : "=r" (x) );
  return x;
}



static inline uint64
r_tp()
{
  uint64 x;
  asm volatile("mv %0, tp" : "=r" (x) );
  return x;
}

static inline void
w_tp(uint64 x)
{
  asm volatile("mv tp, %0" : : "r" (x));
}

static inline uint64
r_ra()
{
  uint64 x;
  asm volatile("mv %0, ra" : "=r" (x) );
  return x;
}


static inline void
sfence_vma()
{

  asm volatile("sfence.vma zero, zero");
}

typedef uint64 pte_t;
typedef uint64 *pagetable_t;
# 5 "kernel/trap.c" 2
# 1 "kernel/spinlock.h" 1

struct spinlock {
  uint locked;


  char *name;
  struct cpu *cpu;
};
# 6 "kernel/trap.c" 2
# 1 "kernel/proc.h" 1

struct context {
  uint64 ra;
  uint64 sp;


  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};


struct cpu {
  struct proc *proc;
  struct context context;
  int noff;
  int intena;
};

extern struct cpu cpus[8];
# 43 "kernel/proc.h"
struct trapframe {
            uint64 kernel_satp;
            uint64 kernel_sp;
            uint64 kernel_trap;
            uint64 epc;
            uint64 kernel_hartid;
            uint64 ra;
            uint64 sp;
            uint64 gp;
            uint64 tp;
            uint64 t0;
            uint64 t1;
            uint64 t2;
            uint64 s0;
            uint64 s1;
            uint64 a0;
            uint64 a1;
            uint64 a2;
            uint64 a3;
            uint64 a4;
            uint64 a5;
            uint64 a6;
            uint64 a7;
            uint64 s2;
            uint64 s3;
            uint64 s4;
            uint64 s5;
            uint64 s6;
            uint64 s7;
            uint64 s8;
            uint64 s9;
            uint64 s10;
            uint64 s11;
            uint64 t3;
            uint64 t4;
            uint64 t5;
            uint64 t6;
};

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };


struct proc {
  struct spinlock lock;


  enum procstate state;
  void *chan;
  int killed;
  int xstate;
  int pid;


  struct proc *parent;


  uint64 kstack;
  uint64 sz;
  pagetable_t pagetable;
  struct trapframe *trapframe;
  struct context context;
  struct file *ofile[16];
  struct inode *cwd;
  char name[16];

  struct VMA* mmap_vma[16];
};
# 7 "kernel/trap.c" 2
# 1 "kernel/sleeplock.h" 1

struct sleeplock {
  uint locked;
  struct spinlock lk;


  char *name;
  int pid;
};
# 8 "kernel/trap.c" 2
# 1 "kernel/defs.h" 1
struct buf;
struct context;
struct file;
struct inode;
struct pipe;
struct proc;
struct spinlock;
struct sleeplock;
struct stat;
struct superblock;
struct VMA;


void binit(void);
struct buf* bread(uint, uint);
void brelse(struct buf*);
void bwrite(struct buf*);
void bpin(struct buf*);
void bunpin(struct buf*);


void consoleinit(void);
void consoleintr(int);
void consputc(int);


int exec(char*, char**);


struct file* filealloc(void);
void fileclose(struct file*);
struct file* filedup(struct file*);
void fileinit(void);
int fileread(struct file*, uint64, int n);
int filestat(struct file*, uint64 addr);
int filewrite(struct file*, uint64, int n);


void fsinit(int);
int dirlink(struct inode*, char*, uint);
struct inode* dirlookup(struct inode*, char*, uint*);
struct inode* ialloc(uint, short);
struct inode* idup(struct inode*);
void iinit();
void ilock(struct inode*);
void iput(struct inode*);
void iunlock(struct inode*);
void iunlockput(struct inode*);
void iupdate(struct inode*);
int namecmp(const char*, const char*);
struct inode* namei(char*);
struct inode* nameiparent(char*, char*);
int readi(struct inode*, int, uint64, uint, uint);
void stati(struct inode*, struct stat*);
int writei(struct inode*, int, uint64, uint, uint);
void itrunc(struct inode*);


void ramdiskinit(void);
void ramdiskintr(void);
void ramdiskrw(struct buf*);


void* kalloc(void);
void kfree(void *);
void kinit(void);


void initlog(int, struct superblock*);
void log_write(struct buf*);
void begin_op(void);
void end_op(void);


int pipealloc(struct file**, struct file**);
void pipeclose(struct pipe*, int);
int piperead(struct pipe*, uint64, int);
int pipewrite(struct pipe*, uint64, int);


void printf(char*, ...);
void panic(char*) __attribute__((noreturn));
void printfinit(void);


int cpuid(void);
void exit(int);
int fork(void);
int growproc(int);
void proc_mapstacks(pagetable_t);
pagetable_t proc_pagetable(struct proc *);
void proc_freepagetable(pagetable_t, uint64);
int kill(int);
int killed(struct proc*);
void setkilled(struct proc*);
struct cpu* mycpu(void);
struct cpu* getmycpu(void);
struct proc* myproc();
void procinit(void);
void scheduler(void) __attribute__((noreturn));
void sched(void);
void sleep(void*, struct spinlock*);
void userinit(void);
int wait(uint64);
void wakeup(void*);
void yield(void);
int either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
int either_copyin(void *dst, int user_src, uint64 src, uint64 len);
void procdump(void);


void swtch(struct context*, struct context*);


void acquire(struct spinlock*);
int holding(struct spinlock*);
void initlock(struct spinlock*, char*);
void release(struct spinlock*);
void push_off(void);
void pop_off(void);


void acquiresleep(struct sleeplock*);
void releasesleep(struct sleeplock*);
int holdingsleep(struct sleeplock*);
void initsleeplock(struct sleeplock*, char*);


int memcmp(const void*, const void*, uint);
void* memmove(void*, const void*, uint);
void* memset(void*, int, uint);
char* safestrcpy(char*, const char*, int);
int strlen(const char*);
int strncmp(const char*, const char*, uint);
char* strncpy(char*, const char*, int);


void argint(int, int*);
int argstr(int, char*, int);
void argaddr(int, uint64 *);
int fetchstr(uint64, char*, int);
int fetchaddr(uint64, uint64*);
void syscall();


extern uint ticks;
void trapinit(void);
void trapinithart(void);
extern struct spinlock tickslock;
void usertrapret(void);


void uartinit(void);
void uartintr(void);
void uartputc(int);
void uartputc_sync(int);
int uartgetc(void);


void kvminit(void);
void kvminithart(void);
void kvmmap(pagetable_t, uint64, uint64, uint64, int);
int mappages(pagetable_t, uint64, uint64, uint64, int);
pagetable_t uvmcreate(void);
void uvmfirst(pagetable_t, uchar *, uint);
uint64 uvmalloc(pagetable_t, uint64, uint64, int);
uint64 uvmdealloc(pagetable_t, uint64, uint64);
int uvmcopy(pagetable_t, pagetable_t, uint64);
void uvmfree(pagetable_t, uint64);
void uvmunmap(pagetable_t, uint64, uint64, int);
void uvmclear(pagetable_t, uint64);
pte_t * walk(pagetable_t, uint64, int);
uint64 walkaddr(pagetable_t, uint64);
int copyout(pagetable_t, uint64, char *, uint64);
int copyin(pagetable_t, char *, uint64, uint64);
int copyinstr(pagetable_t, char *, uint64, uint64);


void plicinit(void);
void plicinithart(void);
int plic_claim(void);
void plic_complete(int);


void virtio_disk_init(void);
void virtio_disk_rw(struct buf *, int);
void virtio_disk_intr(void);




struct VMA* vma_alloc();
void vmainit();
int mmap_trap(pagetable_t pagetable, uint64 va);
void vma_free(struct VMA* vma);
# 9 "kernel/trap.c" 2
# 1 "kernel/fs.h" 1
# 14 "kernel/fs.h"
struct superblock {
  uint magic;
  uint size;
  uint nblocks;
  uint ninodes;
  uint nlog;
  uint logstart;
  uint inodestart;
  uint bmapstart;
};
# 32 "kernel/fs.h"
struct dinode {
  short type;
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[12 +1];
};
# 56 "kernel/fs.h"
struct dirent {
  ushort inum;
  char name[14];
};
# 10 "kernel/trap.c" 2
# 1 "kernel/file.h" 1
struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE, FD_DEVICE } type;
  int ref;
  char readable;
  char writable;
  struct pipe *pipe;
  struct inode *ip;
  uint off;
  short major;
};






struct inode {
  uint dev;
  uint inum;
  int ref;
  struct sleeplock lock;
  int valid;

  short type;
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[12 +1];
};


struct devsw {
  int (*read)(int, uint64, int);
  int (*write)(int, uint64, int);
};

extern struct devsw devsw[];
# 11 "kernel/trap.c" 2
# 1 "kernel/vma.h" 1
struct VMA {
  int index;
  int used;
  void *addr;
  int length;
  int prot;
  int flags;
  struct file *file;
  int fd;
};
# 12 "kernel/trap.c" 2
# 1 "kernel/fcntl.h" 1
# 13 "kernel/trap.c" 2

struct spinlock tickslock;
uint ticks;

extern char trampoline[], uservec[], userret[];


void kernelvec();

extern int devintr();

void
trapinit(void)
{
  initlock(&tickslock, "time");
}


void
trapinithart(void)
{
  w_stvec((uint64)kernelvec);
}





void
usertrap(void)
{
  int which_dev = 0;

  if((r_sstatus() & (1L << 8)) != 0)
    panic("usertrap: not from user mode");



  w_stvec((uint64)kernelvec);

  struct proc *p = myproc();


  p->trapframe->epc = r_sepc();

  if(r_scause() == 8){


    if(killed(p))
      exit(-1);



    p->trapframe->epc += 4;



    intr_on();

    syscall();
  } else if((which_dev = devintr()) != 0){

  } else if ((r_scause() == 13) || (r_scause() == 15)) {
    uint64 va = r_stval();
    if (mmap_trap(myproc()->pagetable, va) == -1)
      p->killed = 1;

  } else {
    printf("usertrap(): unexpected scause %p pid=%d\n", r_scause(), p->pid);
    printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
    setkilled(p);
  }

  if(killed(p))
    exit(-1);


  if(which_dev == 2)
    yield();

  usertrapret();
}




void
usertrapret(void)
{
  struct proc *p = myproc();




  intr_off();


  uint64 trampoline_uservec = ((1L << (9 + 9 + 9 + 12 - 1)) - 4096) + (uservec - trampoline);
  w_stvec(trampoline_uservec);



  p->trapframe->kernel_satp = r_satp();
  p->trapframe->kernel_sp = p->kstack + 4096;
  p->trapframe->kernel_trap = (uint64)usertrap;
  p->trapframe->kernel_hartid = r_tp();





  unsigned long x = r_sstatus();
  x &= ~(1L << 8);
  x |= (1L << 5);
  w_sstatus(x);


  w_sepc(p->trapframe->epc);


  uint64 satp = ((8L << 60) | (((uint64)p->pagetable) >> 12));




  uint64 trampoline_userret = ((1L << (9 + 9 + 9 + 12 - 1)) - 4096) + (userret - trampoline);
  ((void (*)(uint64))trampoline_userret)(satp);
}



void
kerneltrap()
{
  int which_dev = 0;
  uint64 sepc = r_sepc();
  uint64 sstatus = r_sstatus();
  uint64 scause = r_scause();

  if((sstatus & (1L << 8)) == 0)
    panic("kerneltrap: not from supervisor mode");
  if(intr_get() != 0)
    panic("kerneltrap: interrupts enabled");

  if((which_dev = devintr()) == 0){
    printf("scause %p\n", scause);
    printf("sepc=%p stval=%p\n", r_sepc(), r_stval());
    panic("kerneltrap");
  }


  if(which_dev == 2 && myproc() != 0 && myproc()->state == RUNNING)
    yield();



  w_sepc(sepc);
  w_sstatus(sstatus);
}

void
clockintr()
{
  acquire(&tickslock);
  ticks++;
  wakeup(&ticks);
  release(&tickslock);
}






int
devintr()
{
  uint64 scause = r_scause();

  if((scause & 0x8000000000000000L) &&
     (scause & 0xff) == 9){



    int irq = plic_claim();

    if(irq == 10){
      uartintr();
    } else if(irq == 1){
      virtio_disk_intr();
    } else if(irq){
      printf("unexpected interrupt irq=%d\n", irq);
    }




    if(irq)
      plic_complete(irq);

    return 1;
  } else if(scause == 0x8000000000000001L){



    if(cpuid() == 0){
      clockintr();
    }



    w_sip(r_sip() & ~2);

    return 2;
  } else {
    return 0;
  }
}

int mmap_trap(pagetable_t pagetable, uint64 va) {
  if (va > (1L << (9 + 9 + 9 + 12 - 1)))
    return -1;

  va = (((va)) & ~(4096 -1));

  struct proc *p = myproc();
  struct VMA *vma = 0;
  int valid = 0;

  for (int i = 0; i < 16; ++i) {
    if (p->mmap_vma[i] != 0) {
      if (va >= (uint64)p->mmap_vma[i]->addr &&
          va <=
              (uint64)((uint64)p->mmap_vma[i]->addr + p->mmap_vma[i]->length)) {
        vma = p->mmap_vma[i];
        valid = 1;
        break;
      }
    }
  }

  if (valid == 0)
    return -1;

  void *mem = kalloc();
  if (mem == 0)
    return -1;

  int off = (va - (uint64)vma->addr);


  readi(vma->file->ip, 0, (uint64)mem, off, 4096);

  int prot = vma->prot;
  int perm = 0;
  if (prot & PROT_EXEC) { perm |= (1L << 3); }
  if (prot & PROT_READ) { perm |= (1L << 1); }
  if (prot & PROT_WRITE) { perm |= (1L << 2); }

  mappages(pagetable, va, 4096, (uint64)mem, perm);

  return 0;
}
