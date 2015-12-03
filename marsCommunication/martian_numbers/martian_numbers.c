//Thor H. Jonsson
// The following program translates a martian number to a number in the decimal
// system and back to another positional system with any given base

// How to read a martian number:
// To read a martian number the base source of the number is needed. 
// The base source always follows each number
// The number of letters defines the base for the system

#include <stdio.h>
#include <stdlib.h>
// Allows us to use the function strlen which returns the length of a string
#include <string.h>

// 

void scanfile(FILE*);
int TransformToNum(char*,char*);
void TransformFromNum(int, char*);
int main (void)
{
    /*int v;
    v = TransformToNum("9","0123456789");
    TransformFromNum(v,"oF8");
    */// the file we want to read from comes from argv
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
// The following are our assumptions about the file
// First line contains only the number of lines following it
// For the following lines we have three strings
// String 1 is a number in the number system that string 2 defines
// Strings 3 is the new number system,the one which the output will belong to 
void scanfile(FILE *f_ptr)
{
    // Allocate space for storing each line in the file 
    size_t line_size = 0;
    // This pointer points to the memory address where the data will be kept 
    char *line_buffer = NULL;
    // line number is the first line in our file
    int line_number = 0;
    int NumberOfLines = 0;
// The strings in the array suffice the same conditions as the ones in the file
    char *string_array[3];
    int DecNum, i, j, read;
    // getline is supposed to obtain text from our file 
    // When there are no more lines to be read getline returns -1
    while((read = getline(&line_buffer, &line_size, f_ptr)) != -1){	
     // printf("Line is number %d and the buffer is :%s with length %zu read is %d \n",line_number, line_buffer, strlen(line_buffer),read);
      if(line_number == 0){
	  if(sscanf(line_buffer,"%d",&NumberOfLines) != 1){
	    printf("The first line of the file doesn't seem to suffice the necessary conditions");
	    break;
	  }
      }else if(line_number > 0){
	  if(line_number > NumberOfLines){
	    printf("I can do more lines if you would just let me!");
	  }
	  i = 0;
	  j = 0;
	  string_array[i] = malloc(read*sizeof(char));
	  if(string_array[i]==NULL){
	    printf("unable to allocate memory \n");
	    break;
	  }
// k will run through the line_buffer and monitor our construction its substrings
// read is the number of characters in the buffer
		for (int k = 0; k < read-1; k++){
		   // Here I'm watching out from the delimiter, ' ' 
		   if(line_buffer[k] != ' ' && k < read){
		      string_array[i][j] = line_buffer[k];
		      j++;
		      if(k==read-2){
			string_array[i][j] = '\0';
			break;
		      }
		   }else{
		      // end the string
		      string_array[i][j] = '\0';
		      i++;
		      j=0;
		      string_array[i] = malloc((read-k)*sizeof(char));
		      
		      if(string_array[i]==NULL){
			printf("unable to allocate memory \n");
			break;
		      }
		    }
		  } 
	  printf("Case #%d: ", line_number);
	  DecNum = TransformToNum(string_array[0], string_array[1]);
	  TransformFromNum(DecNum, string_array[2]);
   	      for(i = 0; i < 3; i++){ 
		free(string_array[i]);
	      }
      }
	line_number++;
    }
// Close file and deallocate memory
    fclose(f_ptr);
    free(line_buffer);
}

// The method will use known arithmetic functions for the standard decimal system. 
// We will use the fact that by knowing the size of BaseSource 
// and the number of each letter within it we can factorize each number and obtain
// a human value at the same time.
// Example: 253 = 2*10² + 5*10¹ + 3*10⁰
// 2,5,3 are the numbers of the position of those same letters in the string 0123456789
// 10 reflects the length of the string and the number of the power reflects the number of trailing letters
// If we use the 'foo' example the length of the string oF8 is 3,
// o has position 0, F has position 1, 8 has position 2
// Therefore, Foo = 1*3² + 0*3¹ + 0*3⁰ = 9
int TransformToNum(char* MarNum,char* BaseSource)
{
  // i will run through the array that stores MarNum
  int i;
  // j will run through the basesource
  int j;
  // k will run through a for loop that will act as a power operator
  int k;
  // l will let me measure whether the MarNum is ill-defined or not
  int l;
  // This is the number in BaseCible format
  int HumNum = 0;
  // length of the BaseSource
  int BSlen = strlen(BaseSource);
  // length of the martian number
  int MNlen = strlen(MarNum);
  // To calculate BSlen to a certain power, needed as we will factorize the number
  int BSlenPow;
  // To see the change in HumNum after each loop
  // The outermost loop goes through MarNum
  for(i = 0; i < MNlen;i++){ 
  // We go through BaseSource and see if we can find the letters that MarNum consists of
  // We need to know the position of those letters within the BaseSource
    l = 0;
    for(j = 0; j < BSlen; j++){
      if(MarNum[i] == BaseSource[j]){
	// Now we know the position of our letter within the number system
	// We need to know to what power we need to raise the number equal to the length of the number system
	// It is 1 if there are no trailing letters in our MarNum 
	BSlenPow = 1;
	for(k = 0; k < MNlen-i-1; k++){
	  BSlenPow = BSlenPow*BSlen;
	}
	HumNum = HumNum + j*BSlenPow;
	// Stop looking for the position of the letter within the BaseSource
	break;
      }
      else{
	l++;
      }
    }
    // If the letter in MarNum was not found in BaseSource, HumNum = 0, j = strlen(BaseSource)
    if(l == BSlen){
      printf("Number consists of letters that are not contained in BaseSource");
    }
  }
 return HumNum;
}

// We are going to transform from a number to a number within another number system
// We need to know the number of characters in the system
// To determine the length of our new number in this system we will 
// divide the given number by the number of letters in the new number system 
// Then we need to factorize our old number in terms of the length of the new number system
// 124 = 1*10² + 2*10¹ + 4*10⁰ = 
// E.g. given the number 124 with a number system of length 3
// We know that 3² = 9, 3³ = 27, 3⁴ = 81, 3⁵ = 243 and 81<=124<243
// Therefore we need five letters to describe the number in the new system
// By dividing 124 by 3 we obtain
// 124 = 41*3 + 1
// 41 = 13*3 + 2
// 13 = 4*3 + 1
// 4 = 1*3 + 1
// 1 = 0*3 + 1
// Therefore 124 = (((1*3 + 1)*3 + 1)*3 + 2)*3 + 1 = 1*3⁴+1*3³+1*3²+2*3¹+1*3⁰
void TransformFromNum(int HumNum, char* BaseCible)
{
  // This is the base of the number system. 
  // Our method will be to first change the number to a number of a 
  // numeric system with a different base. From there we will then look up
  // the positions of the letters in our BaseCible
  int Base = strlen(BaseCible);
  // To keep track of Base when we multiply it by itself
  int BasePow = 1;
  // To count the powers of Base we need to represent the number. 
  // This will give away the number of letters needed for the new number
  int N = 0;
  while(BasePow <= HumNum){
    BasePow = Base*BasePow;
    N++;
  }
  // Here we will store our new number
  char* MarNum = malloc(N*sizeof(char));
  int remainder;
  MarNum[N] = '\0';
  // First we will factorize the number with regards to the Base
  for(int j = N-1; j>=0;j--){
    remainder = HumNum%Base;
    MarNum[j] = BaseCible[remainder];
    HumNum = HumNum/Base;
  }
  printf("%s \n", MarNum);
}
