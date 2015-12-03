//Exam
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Cord
{
    //color
    char color;
    //number of units
    int NumberOfUnits;
    // inLoop attribute 
    // This attribute is zero if it is not in our loop otherwise 1 
    int inLoop;
    
}Cord;


// This function takes the substring apart and returns a cord
Cord token2Cord(char *token)
{
    Cord myCord;
    char CordCol;
    int Units;
    if(sscanf(token,"%d %c",&Units, &CordCol) != 2){
	printf("Something is wrong with token");
	exit(EXIT_FAILURE);
     }
     myCord.color = CordCol;
     myCord.NumberOfUnits = Units;
     // All cords have yet to go in loop at the beginning.
     myCord.inLoop = 0;
     return myCord;
}
// This function looks at the elements from ArrC and tries to make
// a loop out of it.
// Each loop must end with another color than it begins with.
void LongestAltLoop(Cord *ArrC,int NumberOfCords)
{
    char FirstColor;
    char LastColor;
    // Lets make a loop that always begins with the longest cord
    // no matter what color it is
    int MaxCord = -1;
    int CordIndex;
    Cord *Loop = (Cord *)malloc(sizeof(Cord)*NumberOfCords);
    int Terminate = 0;
    int iteration = 0;
    // This is used to see if there are any cords left to go in the loop
    
     

    // Counts the length of our loop
    int sum = 0; 
    while(Terminate == 0){
	// We run through all the Cords
	// If we don't find the color we need we set Terminate = 1
	// Otherwise we run through ArrC looking for the color with
	// maximum number of units and put it in the loop
	// to mark that cord is already in loop we use the inloop attribute
	for(int i = 0; i < NumberOfCords; i++){
	 
	  // We only examine cords that are not yet in the loop
	  if(ArrC[i].inLoop == 0){
	      // We do this so that we won't pick a Cord that is in the loop already
	      //If this is the first iteration then color doesn't matter
	      if(iteration == 0 && MaxCord == -1){
		MaxCord = i;
	      }
	      if(iteration == 0 && ArrC[i].NumberOfUnits > ArrC[MaxCord].NumberOfUnits){
		  MaxCord = i;
	      //Else we look for the biggest one of those that are not yet in the loop and have a different color from the last one
	      }else if(LastColor != ArrC[i].color && ArrC[i].NumberOfUnits > ArrC[MaxCord].NumberOfUnits){
		  MaxCord = i;
	      }
	  }
	}
	//We give MaxCord a new name
	CordIndex = MaxCord;
	// We make clear that we are going to put it in the loop
	ArrC[MaxCord].inLoop = 1;
	LastColor = ArrC[MaxCord].color;
	if(iteration == 0){
	  FirstColor = ArrC[MaxCord].color;
	}
	
	MaxCord = -1;
	
	// We need to choose a new value for MaxCord
	// We'll simply choose the first value we see that has a different color
	// If we don't find this value we'll terminate
	for(int i = 0;i < NumberOfCords; i++){
	    if(ArrC[i].inLoop == 0 && ArrC[i].color != LastColor){
	      MaxCord = i;
	      break;
	    }
	}
	// This means that there are no cords left 
	if(MaxCord == -1){
	  Terminate = 1;
	}
	
	// Put it in the loop 
	if(iteration == 0 && Terminate == 0){
	  Loop[iteration] = ArrC[CordIndex];

	  sum = sum + ArrC[CordIndex].NumberOfUnits -1;
	  // This is to make sure that we connect the loop correctly
	  // i.e. first color may not equal last color
	}else if(Terminate == 1){
	    if(LastColor != FirstColor){
	      sum = sum + ArrC[CordIndex].NumberOfUnits -1;
	    }
	}else{
	  Loop[iteration] = ArrC[CordIndex];
	  sum = sum + ArrC[CordIndex].NumberOfUnits - 1;
	}

	iteration++;
    }   
    printf("%d\n",sum);
    free(Loop);
}
// This function takes the line_buffer from file and 
// builds a pointer of Cords using strtok()
void StoreBag(char *line_buffer,int NumberOfCords)
{
    Cord *ArrC = (Cord *)malloc(sizeof(Cord)*NumberOfCords);
    char *token;
    int i = 0;
   /* get the first token */
   token = strtok(line_buffer," ");
   
  while(token != NULL && i < NumberOfCords) {
      // We turn the token into a Cord for analysis
     ArrC[i] = token2Cord(token); 
     // We need this to parse to the next token
      token = strtok(NULL," ");
      i++;
   }
   // Now we can send the pointer of cords to an examination function
   LongestAltLoop(ArrC, NumberOfCords);
   free(ArrC);
  
}




// The very first line contains number of cases
// The info line contains number of strings that are either white or black
// In each line we need to extract each string,
// We'll end up by making a pointer of cords to work with 
void scanfile(FILE *f_ptr)
{
  Cord *ArrC = NULL;
  // Used to store output from getline()
  char *line_buffer = NULL;
  size_t line_size = 0;
  int read;
  int caseNumber = 1;
  int caseLimit = 1;
  // We'll use bag_lines to determine whether there's a bag to extract
  // from getline()
  int bag_line = 2;
  int info_line = 1;
  // first line will give us the number of cases, i.e. the caseLimit
  int line_number = 0;
  // This info will always be stored after an info_line
  int NumberOfCords;
  while((read = getline(&line_buffer, &line_size, f_ptr)) != -1){
    //info_line contains info about the number of cords in next bag
    if(line_number == info_line){

      if(sscanf(line_buffer,"%d", &NumberOfCords) != 1){
	printf("Something is wrong with line number: %d \n", line_number);
	exit(EXIT_FAILURE);
      }
      if(caseNumber <= caseLimit){
	info_line = info_line + 2;
      }
      // takes care of the memory leaking
      if(ArrC != NULL){
	free(ArrC);
      }
      // This pointer will store all the cords from the bag
      // its size depends on the info from the infoline
      ArrC = (Cord *)malloc(sizeof(Cord)*NumberOfCords);
      if(ArrC == NULL){
	 printf("unable to allocate memory \n");
	 exit(EXIT_FAILURE);
      } 
      
      
    }else if(line_number == bag_line){
      // Read each substring that represents a cord and store it
	printf("Case #%d: ",caseNumber);
      StoreBag(line_buffer, NumberOfCords);
	bag_line = bag_line + 2;
	caseNumber++;
	
    
      
    }else if(line_number == 0){
      
      if(sscanf(line_buffer,"%d",&caseLimit) != 1){
	printf("Something is wrong with line number: %d \n",line_number);
	exit(EXIT_FAILURE);
      }
    }
    line_number++;
  }
  fclose(f_ptr);    
  free(line_buffer);
  free(ArrC);
}


int main(int argc, char *argv[])
{
    
    FILE *f_ptr = fopen(argv[1], "r");

    // In case we don't have a proper file
    if(f_ptr == NULL){
      perror("Error: ");
      exit(EXIT_FAILURE);
    }
    else{
      scanfile(f_ptr);
      exit(EXIT_SUCCESS);
    }
}