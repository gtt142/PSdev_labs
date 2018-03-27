#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int logExpr(int x1, int x2, int x3) {
    return x1 && x2 || x3;
}

int main() {
    int pid1 = 0, pid2 = 0, pid3 = 0;
    int res = 0;

    pid1 = fork();
    if(pid1) {
        wait(NULL);
    }
    pid2 = fork();
    if(pid2) {
        wait(NULL);
    }
    pid3 = fork();
    if(pid3) {
        wait(NULL);
    }

    res = logExpr(pid1, pid2, pid3);
    printf("%d %d %d | %d\n", !!pid1, !!pid2, !!pid3, res);
    
    exit(0);
}
