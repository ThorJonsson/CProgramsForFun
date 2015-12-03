#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void strengcpy(char*);
int
       main(void)
       {
	   int Numberoflines = 7;
           FILE *fp;
	   // line is the buffer, essentially a string or an array of the char's that were found in the previous line
           char *line = NULL;
	   // I want to create an array of char *'s 
	   // This declares copy as an array of Numberoflines many char pointers
           char *copy[Numberoflines];
	   size_t len = 0;
	   ssize_t read;
	   
	   
	   int i = 1;
           fp = fopen("capsule.txt", "r");
           if (fp == NULL)
               exit(EXIT_FAILURE);

           while ((read = getline(&line, &len, fp)) != -1) {
		// Each pointer needs to have space allocated, this space needs to be the space the string takes
		if((copy[i] = malloc((strlen(line)+1)*sizeof(char)))==NULL){
		  printf("unable to allocate memory \n");
		  return -1;
		}
		// 
		strengcpy(line);
		printf("This is line : %s\n", line);
	

		//In each line I will point char pointer number i to the address of line
		//sstrcpy(copy[i], line);
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s \n", copy[i]);
		i++;
           }
	    //printf("Lets see if the copy remains \n");
           for(i = 1; i <= Numberoflines; i++){
	     // printf("Copied line of length %zu :\n", strlen(copy[i]));
	      //printf("%s \n", copy[i]);
	   }

           free(line);
           exit(EXIT_SUCCESS);
       }

           void strengcpy(char *regexp)
           {
               

               for (int k = 0; regexp[k] != '\0'; k++){
                  if(regexp[k] == '('){
		    regexp[k] = '[';
		  } else if(regexp[k] == ')'){
		    regexp[k] = ']';
		  }
	       } 
           }


    /* 
    for (i=0 ; i<5; i++) {
        if ((b[i] = malloc(sizeof(char) * COL)) == NULL) {
            printf("unable to allocate memory \n");
            return -1;
        }
    }

    strcpy(b[0], "string1");
    strcpy(b[1], "string2");
    strcpy(b[2], "string3");
    strcpy(b[3], "string4");
    strcpy(b[4], "string5");

    

       b[]              0   1   2   3   4   5   6    7     8   9
    +--------+        +---+---+---+---+---+---+---+------+---+---+
    |      --|------->| s | t | r | i | n | g | 1 | '\0' |   |   |
    +--------+        +---+---+---+---+---+---+---+------+---+---+
    |      --|------->| s | t | r | i | n | g | 2 | '\0' |   |   |
    +--------+        +---+---+---+---+---+---+---+------+---+---+
    |      --|------->| s | t | r | i | n | g | 3 | '\0' |   |   |
    +--------+        +---+---+---+---+---+---+---+------+---+---+
    |      --|------->| s | t | r | i | n | g | 4 | '\0' |   |   |
    +--------+        +---+---+---+---+---+---+---+------+---+---+
    |      --|------->| s | t | r | i | n | g | 5 | '\0' |   |   |
    +--------+        +---+---+---+---+---+---+---+------+---+---+

    

     b should be free'd 
    for (i=0 ; i<5; i++)
        free(b[i]);

    return 0;
}
 */