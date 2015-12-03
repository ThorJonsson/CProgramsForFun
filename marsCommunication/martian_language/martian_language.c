// Allows us to work with regular expressions
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MATCHES 1 //The maximum number of matches allowed in a single string
#define MAX_STRINGS 5000

// regex_t* is a pointer to a pattern buffer storage area
void scanfile(FILE*);
int CountMatches(char*, char**, int, int);
 
int main() {
  // the file we want to read from comes from argv
    FILE *f_ptr = fopen("capsule.txt", "r");
    // In case we don't have a proper file
    if(f_ptr == NULL){
      perror("Error: ");
      return(-1);
    }
    else{
      scanfile(f_ptr);
      return 0;
    }
}

// The following assumptions are made about the contents of the file:
// First line has the format StringLength NumberofStrings NumberofRegExp
// For each reg. exp. we will loop through all the strings 
// While looping through the strings we count those that match the RegExp
void scanfile(FILE *f_ptr)
{
    //int MaxStrLen = 15;
    int StringLength, NumberofStrings, NumberofRegExp, TotNumLines, j, counter,read;
    // Allocate space for storing each line in the file 
    size_t line_size = 0;
    // This pointer will be used for getline which allocates the memory needed
    char *line_buffer = NULL;
    char *string_array[MAX_STRINGS];
    // line number is the first line in our file
    int line_number = 0;
    int CaseNumber = 0;
    NumberofStrings = 0;
    TotNumLines = 0;
    // getline is supposed to obtain text from our file 
    // When there are no more lines to be read getline returns -1
    while((read=getline(&line_buffer, &line_size, f_ptr)) != -1){
      
	// Extract info from line 1
	if(line_number == 0){
	  
	  // sscanf() returns the number of items successfully read 
	  if (sscanf(line_buffer, "%d %d %d", &StringLength, &NumberofStrings, &NumberofRegExp) != 3) {
	    printf("didn't get all three values\n");
	    break;
	  } else {
	    TotNumLines = NumberofRegExp + NumberofStrings;
	  } 
	
	  
	} else if(line_number <= NumberofStrings){
	    
// Each pointer needs to have space allocated, this space needs to be the space the string takes
	    string_array[line_number-1] = malloc((strlen(line_buffer)+1)*sizeof(char));
	    if(string_array[line_number-1]==NULL){
	      printf("unable to allocate memory \n");
	      break;
	    }
	    //In each line I will point char pointer number i to the address of line
	    strcpy(string_array[line_number-1], line_buffer);
	    
	} else if(line_number <= TotNumLines){  
	  CaseNumber = CaseNumber + 1;
	  // Begin by making sure that the regular expression is on the correct format  
	  for (j = 0; line_buffer[j] != '\0'; j++){
              if(line_buffer[j] == '('){
		line_buffer[j] = '[';
	      } else if(line_buffer[j] == ')'){
		line_buffer[j] = ']';
	      } else if(j == read-1){
		line_buffer[j] = '\0';
	      }
	  }    
	    // At this point my line_buffer are reg. exp.
	    counter = CountMatches(line_buffer,string_array, NumberofStrings, StringLength);
	    printf("Case #%d: %d\n", CaseNumber, counter); 
	  
	}
	line_number++;
    }
// Close file and deallocate memory
    fclose(f_ptr);
    free(line_buffer);
}

int CountMatches(char* StringExp, char **StringArray, int NumberofStrings, int StringLength) {
  // To store info about the matching of the string with the reg. exp.
  // We need to check if the last number of the char that matches
  int FailCheck, counter;
  counter = 0;
  regex_t RegularExp; //Our compiled expression
  //REG_EXTENDED is so that we can use Extended regular expressions
  FailCheck = regcomp(&RegularExp, StringExp, REG_EXTENDED);
  if (FailCheck != 0) {
    printf("regcomp failed with %d\n", FailCheck);
  }
  regmatch_t matches[MAX_MATCHES]; 
  for(int i = 0; i < NumberofStrings; i++){
  //Compare the string to the expression, if regexec finds a match it returns 0
  //If the length of the string is equal to the last char matched then we count it 
    if (regexec(&RegularExp, StringArray[i], MAX_MATCHES, matches, 0) == 0 && matches[0].rm_eo == StringLength) {
      counter = counter + 1;
    } 
  }
  return counter;
}
