#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define error(a) {perror(a); exit(1);};

int main(int argc, char *argv[])
{
    if(argc != 2){error("Incorrect number of arguments")};
    int pfd[2], pid;
    char user[100];
    sprintf(user, "%s", argv[1]);
    if (pipe(pfd) == -1) error("pipe");
    printf("parent: pipe created, channels: READ=%d and WRITE=%d\n", pfd[0], pfd[1]);
    switch (pid = fork()) {
        case -1: error("fork");
        case 0: /* 1st child: who */
        printf("1st child process created\n");
        if (close(1) == -1) error("close");
        if (dup(pfd[1]) != 1) error("dup");
        close(pfd[0]); close(pfd[1]);
        execlp("ps", "ps", "aux", NULL);
        error("execlp");
    }
    printf("parent: ps(%d) process launched\n", pid);
    switch (pid = fork())
    {
        case -1: error("fork");
        case 0: /* 2nd child process: wc -l */
        printf("2nd child process created\n");
        if (close(0) == -1) error("close");
        if (dup(pfd[0]) != 0) error("grep");
        close(pfd[0]); close(pfd[1]);
        execlp("grep", "grep", user, NULL);
        error("execlp");
    }
    printf("parent: grep(%d) process launched\n", pid);
    close(pfd[0]); close(pfd[1]);
    while ((pid = wait(NULL)) != -1)
    {
        printf("parent: %d process finished\n", pid);
    }
    return 0;
}
