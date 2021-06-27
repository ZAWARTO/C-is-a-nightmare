/**
 * @file timeTesterOfCommands.c
 * @author ZAWARTO
 * @date 20210627
 * @brief C File of a basic programm that uses child process to execute the commands 
 * introduced by parameter and a sleep process with a time introduced by
 * parameter too to test the running time of a command
 *
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define error(a){perror(a);exit(1);}

int main(int argc, char *argv[]) //argv1 timeLimit argv2..argv11 commands
{

    if (argc > 12)
    {
        error("Incorrect number of arguments, the maximum number of programs to test is 10");
    }
    
    if (argc < 3)
    {
        error("Incorrect number of arguments, you have to test one program at least");
    }
    
    int timeLimit = atoi(argv[1]);

    if(timeLimit < 0)
    {
        error("The time limit must be 0 or greater")
    }

    int programs = argc - 2;
    int forced = 0;
    int finished = 0;
    int i = 0;
    int j = 2;
    int pid;
    int pidTemp;
    int pidFather = 0;
    int status;

    time_t t1, t2;

    t1 = time(NULL);

    while(i<programs) 
    {
        pid = 0;
        pidTemp = 0;
        status = -1;
        t2 = time(NULL);
        pid = fork();
        switch(pid)
        {
            case -1:
                error("fork")
            case 0:
                execlp(argv[j], argv[j], NULL);
                error("execlp programm");
            default:
                pidTemp = fork();
                switch(pidTemp)
                {
                    case -1:
                        error("fork")
                    case 0:
                        execlp("sleep", "sleep", argv[1], NULL);
                        error("execlp sleep");
                    default:
                        pidFather = wait(NULL);
                        if(pidFather == pidTemp)
                        {
                            kill(pid, SIGKILL);
                            wait(NULL);
                            status = 0;
                            break;
                        }
                        else if(pidFather == pid)
                        {
                            kill(pidTemp, SIGKILL);
                            wait(NULL);
                            status = 1;
                            break;
                        }
                        else 
                        {
                            printf("%d\n", pidFather);
                            status = -1;
                            break;
                        }
                }
        }
        t2 = time(NULL) - t2;
        
        if(status == 0)
        {
            forced++;
        }
        else if(status == 1)
        {
            finished++;
        }
        else
        {
            printf("Error on the %d program\n", i);
        }
        i++;
        j++;
    }

    t1 = time(NULL) - t1;

    printf("Total time: %li \n", t1);
    printf("%d programs have been forced to finish taking more than %d seg and %d programs have been finished under the time limit\n", forced, timeLimit, finished);

    return 0;
}
