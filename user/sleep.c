#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: sleep num\n");
    }

    int num = atoi(argv[1]);
    // for debug 
    // printf("num = %d\n", num);

    if (sleep(num) == 0) {
        printf("nothing happens for a little while\n");
    }

    exit(0);
}