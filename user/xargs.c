#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("error\n");
    exit(-1);
  }
  char args[10][20];

  // 1. get intput from stdin
  char c;
  int x = 0;
  int y = 0;

  while (read(0, &c, 1) > 0) {
    if (c == '\n') {
      args[x][y] = '\0';
      x++;
      y = 0;
    } else {
      args[x][y] = c;
      y++;
    }
  }

  for (int i = 0; i < x; i++) {
    int pid = fork();
    if (pid == 0) {
      wait(0);
    } else if (pid > 0) {
      char *execArgs[argc + 1];
      for (int i = 0; i < argc - 2; i++) {
        execArgs[i + 1] = argv[i + 2]; // parame 1
      }
      execArgs[0] = argv[1]; // executable file name
      execArgs[argc - 1] = args[i]; // 从标注输入获取的内容
      execArgs[argc] = 0;

      // debug
      // for (int m = 0; m < argc + 1 ; m++) {
      //   printf("execArgs[%d] = %s\n", m, execArgs[m]);
      // }

      exec(argv[1], (char **)&execArgs);
    }
  }
  exit(0);
}