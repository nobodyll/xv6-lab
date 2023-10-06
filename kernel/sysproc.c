#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

void printBinary64(uint64 num) {
    int size = sizeof(num) * 8;  // 计算64位无符号整数的位数

    for (int i = size - 1; i >= 0; i--) {
        uint64 bit = (num >> i) & 1ULL;  // 从最高位到最低位逐个获取位的值
        printf("%d", bit);
    }
    if (num & PTE_A) {
      printf("PTE_A");
    } 
    printf("\n");
}

#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  // if (pgaccess(buf, 32, &abits) < 0)
  // Get func arguments.
  // does va is page boundary????
  uint64 va; // Start virtal addrss of the first user page to check. 
  argaddr(0, &va); 
  int n; // Number of pages to check.
  argint(1, &n);  
  uint64 addrAbits; // bitmask 
  argaddr(2, &addrAbits);

  struct proc *p = myproc();
  int bitmask = 0;
  pte_t *pte = 0;

  for (int i = 0; i < n; i++, va += PGSIZE) {
    pte = walk(p->pagetable, va, 0);
    // printBinary64(*pte);
    if (*pte & PTE_A) {
      bitmask |= (1 << i);
      *pte &= ~(PTE_A);
    }
  }

  copyout(p->pagetable, addrAbits, (char*)&bitmask, sizeof(int));

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
