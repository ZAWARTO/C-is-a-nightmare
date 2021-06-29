/**
 * @file numberOfProcessOfParameterUser.c
 * @author ZAWARTO
 * @date 20210629
 * @brief C File of a basic pipes programm that prints the number of connected 
 * accounts containing a match to the given pattern introduced as a parameter
 *
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define error(a) {perror(a); exit(1);};

int main(int argc, char *argv[])
{ 

   int pfd1[2], pfd2[2], pid;

   if(pipe(pfd1) != 0) error("pipe");

   printf("parent: pipe created, channels: READ=%d and WRITE=%d\n", pfd1[0], pfd1[1]);

   switch(pid = fork())
   {
      case -1: error("fork");
      case 0: /*1st child: who*/
         printf("1st child process created\n");
         if (close(1)==-1) error ("close");
         if (dup(pfd1[1]) != 1) error("dup");
         close(pfd1[0]); close(pfd1[1]);
         execlp("who", "who", NULL);
         error("who execlp");
 }
   
   printf("parent: who(%d) process launched\n", pid);
   close(pfd1[1]);

   if(pipe(pfd2) != 0) error("pipe");

   printf("parent: pipe created, channels: READ=%d and WRITE=%d\n", pfd2[0], pfd2[1]);
   
   switch(pid = fork())
   {
      case -1: error("fork");
      case 0: /*2nd child: grep acaf00*/
         printf("1st child process created\n");
         close(pfd2[0]);
         if (close(0)==-1) error ("close");
         if (dup(pfd1[0]) != 0) error("dup");
         close(pfd1[0]);
         if (close(1)==-1) error ("close");
         if (dup(pfd2[1]) !=1) error("dup");
         close(pfd2[1]);
         execlp("grep", "grep", argv[1], NULL);
         error("grep execlp");
    }
   
   printf("parent: grep(%d) process launched\n", pid);

   close(pfd1[0]);
   close(pfd2[1]);

   switch(pid = fork())
   {
      case -1: error("fork");
      case 0: /*3rd child: wc -l*/
         if (close(0)==-1) error ("close");
         if (dup(pfd2[0]) !=0) error("dup");

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
