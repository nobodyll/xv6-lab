/* tips
p = get a number from left neighbor
print p
loop:
    n = get a number from left neighbor
    if (p does not divide n)
        send n to right neighbor
p = 从左邻居中获取一个数
print p
loop:
    n = 从左邻居中获取一个数
    if (n不能被p整除)
        将n发送给右邻居

*/

#include "kernel/types.h"
#include "user/user.h"

void foo();

int main() {
  int fd[2];
  if (pipe(fd) < 0) {
    printf("pipe error\n");
    exit(-1);
  }

  int pid = fork();
  if (pid > 0) {
    // parent process
    // close pipe's read end;
    close(fd[0]);

    // write 2-32 to a pipe
    for (int i = 2; i < 33; i++) {
      write(fd[1], &i, sizeof(int));
    }

    close(fd[1]);

    wait(0);
  } else if (pid == 0) {
    foo(fd);
  }
  exit(0);
}

void foo(int *fd_parent) {
  // close pipe's write end;
  close(fd_parent[1]);
  int p = 0;

  // p = get a number from left neighbor
  if (read(fd_parent[0], &p, sizeof(int)) > 0) {
    // print p
    printf("prime %d\n", p);
  } else {
    exit(0);
  }

  // create pipe
  int fds[2];
  if (pipe(fds) < 0) {
    printf("pipe error\n");
    exit(-1);
  }

  // loop:
  //     n = get a number from left neighbor
  //     if (p does not divide n)
  //         send n to right neighbor

  int n = 0;
  while (read(fd_parent[0], &n, sizeof(int)) > 0) {
    if (n % p != 0) {
      write(fds[1], &n, sizeof(int));
    }
  }
  close(fd_parent[0]);

  int pid = fork();
  if (pid > 0) {
    // close read end and write end;
    close(fds[0]);
    close(fds[1]);
    wait(0);
  } else if (pid == 0) {
    foo(fds);
    // close readend, the wrie end was close in foo().
    close(fds[0]);
    exit(0);
  }
}