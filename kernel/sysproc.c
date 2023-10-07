#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
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

  // call backtrace();
  backtrace();

  argint(0, &n);
  if(n < 0)
    n = 0;
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

// lab4: trap
uint64 sys_sigalarm(void) {
  struct proc *p = myproc();

  // arg1: alarm interval
  int interval = 0;
  argint(0, &interval);
  // printf("interval: %d\n", interval);

  // arg2: alarm handler
  void (*handler)();
  argaddr(1, (uint64*)&handler);
  // printf("handler: %d: %p\n", handler);

  p->alarm_handler = (uint64)handler;
  p->alarm_interval = interval;

  return 0;
}

uint64 sys_sigreturn(void) {
  struct proc *p = myproc();

  if (p->first != 1)
    panic("call sigreturn but not in sighandler.\n");

  // resume the process status from alarmtrapframe.
  memmove(p->trapframe, p->alarm_trapframe, sizeof(struct trapframe));
  p->nticks = 0;

  p->first = 0;
  
  return p->trapframe->a0;
}