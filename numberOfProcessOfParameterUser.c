/**
 * @file notBackgroundAlarmWork.c
 * @author ZAWARTO
 * @date 20210627
 * @brief C File of a basic pipes programm that prints the number of process of a user introduced by parameter
 *
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define error(a) {perror(a); exit(1);};

int main(int argc, char *argv[])
{
   if(argc != 2){error("Incorrect number of arguments")};
   char user[100];
   sprintf(user, "%s", argv[1]);
   int pfd1[2], pfd2[2], pid;

   if(pipe(pfd1) != 0) error("pipe 1");

   printf("parent: pipe created, channels: READ=%d and WRITE=%d\n", pfd1[0], pfd1[1]);

   switch(pid = fork())
   {
      case -1: error("fork");
      case 0: 
         printf("1st child process created\n");
         if (close(1)==-1) error ("close 1");
         if (dup(pfd1[1]) != 1) error("dup pipe1[1]");
         close(pfd1[0]); close(pfd1[1]);
         execlp("ps", "ps", "aux", NULL);
         error("ps execlp");
   }
 
   printf("parent: ps(%d) process launched\n", pid);
   close(pfd1[1]);
   if(pipe(pfd2) != 0) error("pipe 2");
   printf("parent: pipe 2 created, channels: READ=%d and WRITE=%d\n", pfd2[0], pfd2[1]);
   
   switch(pid = fork())
   {
      case -1: error("fork");
      case 0:
         printf("2nd child process created\n");
         close(pfd2[0]);
         if (close(0)==-1) error ("close 0");
         if (dup(pfd1[0]) != 0) error("dup pipe1[0]");
         close(pfd1[0]);
         if (close(1)==-1) error ("close 1");
         if (dup(pfd2[1]) !=1) error("dup pipe2[1]");
         close(pfd2[1]);
         execlp("grep", "grep", user, NULL);
         error("grep execlp");
   }
    
   printf("parent: grep(%d) process launched\n", pid);
   close(pfd1[0]);
   close(pfd2[1]);

   switch(pid = fork())
   {
      case -1: error("fork");
      case 0: 
	 printf("3rd child process created\n");
         if (close(0)==-1) error ("close 0");
         if (dup(pfd2[0]) !=0) error("dup pipe2[0]");
         execlp("wc", "wc", "-l", NULL);
         error("wc execlp");
   }
   printf("parent: wc(%d) process launched\n", pid);
   close(pfd2[0]);
   while ((pid = wait(NULL)) != -1)
   {
	printf("parent: %d process finished\n", pid);
   }
   return 0;
}
