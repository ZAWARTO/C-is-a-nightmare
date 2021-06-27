/**
 * @file processOfParameterUser.c
 * @author ZAWARTO
 * @date 20210627
 * @brief C File of a basic programm using the unix system calls to access files 
 * and directories that copy the regular files of the first directory introduced 
 * by parameter into the second one but it starts to copy from a number of bytes 
 * introduced in the last parameter, it makes a register of the path/name and 
 * size of the files copied into a file introduced on the third parameter.
 * The files copied have all the execution permits for groups, owner and others 
 * but no write permit for the owner.
 *
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#define BS 512
#define error(a) {perror(a); exit(1);};

int main(int argc, char *argv[]) //argv1 Dir1 argv2 Dir2 argv3 Fich argv4 Pos
{
    DIR *dir1;
    DIR *dir2;
    struct dirent *lectura1;
    struct dirent *lectura2;
    struct stat st;
    int datos1;
    int datos2;
    int fich;
    int sizeFich;
    int dataSizeCounter = 0;
    int size;
    char dataSizeCounterChar[256] = "";
    char path[256] = "";
    char path2[256] = "";
    char temp[256];
    char buf[BS];
    if(argc != 5)
    {
       error("Incorrect number of arguments");
    }
    int Pos = atoi(argv[4]);
    if(Pos < 0)
    {
        error("Position must be 0 or greater")
    }
    
    if((dir1 = opendir(argv[1])) == NULL){error("Failed to open origin dir");}
    if((dir2 = opendir(argv[2])) == NULL){error("Failed to open destiny dir");}

    while((lectura1 = readdir(dir1)) != NULL)
    {
        sprintf(path, "%s/%s", argv[1], lectura1->d_name);
        sprintf(path2, "%s/%s", argv[2], lectura1->d_name);
  
        if (strcmp(lectura1->d_name, "..") != 0 && strcmp(lectura1->d_name, ".") != 0) 
        {
            if (stat(path, &st) == 0) 
            {
                if (S_ISREG(st.st_mode)) //If is a regular file 
                {
                    if((datos1 = open(path, O_RDONLY, 0600)) == -1){error("Failed to open dir1 file");} //Opens the target file of Dir1 to copy inside Dir2
                    size = lseek(datos1,0,SEEK_END);
                    
                    if (size > Pos) //Check if the size of the file is bigger than the position that indicates the point from which to start copying the file
                    {
                        lseek(datos1,Pos,SEEK_SET); //Moves the pointer to the start position of the file1
                        if((datos2 = open(path2, O_CREAT|O_WRONLY, 0511)) == -1){error("Failed to open dir2 file");}
                        lseek(datos2,0,SEEK_SET);
                        int n = 0;
                        while ((n = read(datos1, buf, BS)) > 0) 
                        {
                            n = write(datos2, buf, n);
                            dataSizeCounter = dataSizeCounter + n;
                        }
                        close(datos1);
                        close(datos2);
                        if((fich = open(argv[3], O_WRONLY|O_CREAT, 0600)) == -1){error("Failed to open Fich file");}
                        sizeFich = lseek(fich,0,SEEK_END);
                        lseek(fich, sizeFich, SEEK_SET);
                        memset(buf, 0, BS);
                        strcpy(buf, "File name: ");
                        strcat(buf, path);
                        strcat(buf, " Size: ");
                        sprintf(dataSizeCounterChar,"%d", dataSizeCounter);
                        strcat(buf, dataSizeCounterChar);
                        strcat(buf, " ");
                        write(fich, buf, strlen(buf));
                        memset(buf, 0, strlen(buf));
                    }    
                }        
            }    
        }           
    }
    close(fich);
    closedir(dir1);
    closedir(dir2);
    return 0;
}
