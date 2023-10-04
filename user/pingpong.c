#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char** argv) {
    const char *str_ping = "ping";
    const char *str_pong = "pong";

    int fds1[2]; 
    int fds2[2]; 
    if (pipe(fds1) < 0 || pipe(fds2) < 0) {
        printf("pipe err. exit....\n");
        exit(-1);
    }

    int cpid = fork();
    if (cpid == 0) {
        // child
        close(fds2[0]);
        close(fds1[1]);
        char buf[10];
        buf[4] = '\0';
        int len = read(fds1[0], buf, 10);

        // debug
        // printf("child receive len %d bytes.\n", len);

        if (len > 0) {
            printf("%d: received %s\n", getpid(), buf);
        }
        write(fds2[1], str_pong, strlen(str_pong));

        close(fds1[0]);
        close(fds2[1]);

    } else if (cpid > 0) {
        // parent
        char buf[10];
        buf[4] = '\0';
        close(fds1[0]);
        close(fds2[1]);

        write(fds1[1], str_ping, strlen(str_ping));
        int len = read(fds2[0], buf, 10);

        // printf("parent receive len %d bytes.\n", len);

        if (len > 0) {
            printf("%d: received %s\n", getpid(), buf);
        }
        close(fds1[1]);
        close(fds2[0]);
    }

    exit(0);

}