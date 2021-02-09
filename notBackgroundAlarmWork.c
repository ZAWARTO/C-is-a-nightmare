  /**
  	* @file notBackgroundAlarmWork.c
  	* @author ZAWARTO
  	* @date 20210209
  	* @brief C File of a basic alarm clock that doesn't work in the background
  	*
    */

  #include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
	#include <stdbool.h>
	#include <string.h>
	#include <unistd.h>

	bool isNumber(int argc, const char * argv[])
	{
    		int size = strlen(argv[1]);
    		char number[size];
   		  strcpy(number, argv[1]);
    		int i;
    		if (number[0] == '-')
    		{
        		for (i = 2; i<argc; i++)
        		{
            			printf("%s\n", argv[i]);
        		}		
        		exit(2);
    		}
    		for (i = 0; i < size; i++)
    		{
        		printf("%c\n",number[i]);
        		if (!isdigit(number[i]))
        		{
            			return false;
        		}
    		}
    		return true;
	}

	int main(int argc, const char * argv[])
	{
    		if (argc < 3) 
    		{
        		printf("Please call the program with this format: ./despertador TIME MESSAGE_1 [MESSAGE_2] ... [MESSAGE_n]");
        		exit(1);                                     
    		}

    		bool b = isNumber(argc, argv);

		    if (b)
		    {
	    		  int i = 1,j = 2,counter = 1;
	    		  for (; i<=atoi(argv[1]); i++)
	    		  {
            			sleep(1);
            			printf("%d Seconds passed\n", counter);
            			printf("%d Seconds left\n", (atoi(argv[1]) - i));
            			counter++;
	    		  }
	    		  for (; j<argc; j++)
        		{
                	printf("%s\n", argv[j]);  
			      }
			      exit(0);
		  }
}
