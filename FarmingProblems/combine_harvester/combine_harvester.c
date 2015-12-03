/* Thor H. Jonsson 01/12'14
First we turn the field into an NxM array where NxM are the number of char
that the array consists of.
Then we take the array and loop through it looking for ...?
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

// A global variable to keep track of whether each field is OK or not
int works;

// This algorithm should check all the blocks of our array
// It simply works through all the lines and columns and checks whether
// the columns and the lines are intersecting in a legitimate way
void CutCrops(int** ArrCrops,int ArrLength,int ArrWidth)
{
  
  // works = 0 if the field does not suffice the conditions otherwise works = 1
  works = 1;
  int currCrop, compCrop_1, compCrop_2, compCrop_3;

  for(int i = 0; i < ArrLength; i++){  
    
    compCrop_1 = ArrCrops[i][0];
    
    for(int j = 1; j < ArrWidth; j++){
    currCrop = ArrCrops[i][j];
    
    /* If currCrop has a lower value then another block int the same row then 
     all that block has been surfaced with a column
    This column therefore must contain bigger values than currCrop*/
    if(currCrop < compCrop_1){
	compCrop_2 = currCrop;
	for(int l = i; l < ArrLength; l++){
	  currCrop = ArrCrops[l][j];
	
	  /* if the currCrop is higher then the column must have been surfaced 
	with a line so all the values on the line must be bigger then 
currCrop and the ones to the left must also be bigger or equal to compCrop_1 */
	  if(currCrop > compCrop_2){
	      compCrop_3 = currCrop;
	      for(int r = 0; r < ArrWidth; r++){
		currCrop = ArrCrops[l][r];
		if(currCrop < compCrop_3 || (r < j && currCrop < compCrop_2)){
		  works = 0;
		  break;
		}
	      }
	   }
	}
     
    /* if currCrop has a bigger value than compCrop_1 then it is clear that 
    the line has been surfaced with a column. Therefore all values in that 
    column must have a value bigger or equal to currCrop*/
    }else if(currCrop > compCrop_1){
	compCrop_2 = currCrop;
	for(int k = i; k < ArrLength; k++){
	  currCrop = ArrCrops[k][j];
	  if(currCrop < compCrop_2){
	    works = 0;
	    break;
	    
	    /*if currCrop has a larger value than compCrop_2 then the column
	    has been surfaced with a row therefore we know that all
	     values in this row must be larger than currCrop */
	  }else if(currCrop > compCrop_2){
	    compCrop_3 = currCrop;
	      for(int r = 0; r < ArrWidth; r++){
		currCrop = ArrCrops[k][r];
		if(currCrop < compCrop_3){
		  works = 0;
		  break;
		  
		}
	     }
	   }
	 }
      }
    }
  }
  
  if(works == 1){
    printf("YES \n");
  }else{
    printf("NO \n");
  }
}

// A utility function to allocate an array with specific dimensions
int** AllocateField(int lines,int columns)
{
    int **Array = (int **)malloc(sizeof(int*)*lines);
    if(Array == NULL){
      printf("Unable to allocate memory");
      exit(EXIT_FAILURE);
    }
    
    for (int r = 0; r < lines; r++){
	Array[r] = (int *)malloc(sizeof(int)*columns);
	if(Array[r] == NULL){
	  printf("Unable to allocate memory");
	  exit(EXIT_FAILURE);
	}
    }
    return Array;
}

// A utility function to free a **pointer
void FreeField(int** Array, int lines)
{
  for(int i = 0; i < lines; i++){
    free(Array[i]);
  }
  free(Array);
}  


// C substring function: It returns a pointer to the substring  
char *substring(char *string, int position, int length) 
{
   char *pointer;
   int i;
   pointer = malloc(length+1);
   if (pointer == NULL){
      printf("Unable to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
   //Get the string to the position 
   for (int i = 0 ; i < position -1 ; i++){ 
      string++; 
   }
   //Copy the char's needed
   for (i = 0 ; i < length ; i++){
      pointer[i] = *string;      
      string++;   
   }
 
   pointer[i] = '\0';
 
   return pointer;
}

/* The following are our assumptions about the file
   First line contains only the number of cases: CaseNumber
   1 <= CaseNumber <= 100
   The second line ArrInfo has the dimensions, NxM, of the field.
   1<=NumLin,NumCol<=100
   Then we have the values of the field, Arr_Harvest[i][j]
   1 <= Arr_Harvest[i][j] <= 100 */
void scanfile(FILE *f_ptr)
{
    
    size_t line_size = 0;
    int read;
    char *line_buffer = NULL;
    
    int line_number = 0;
    int **Arr_Harvest = NULL;
    int CaseNumber;
    int NumLin = 0;
    int NumCol = 0;
    int ArrInfo = 1;
    int i = 0;
    int j = 0;
    int CurrCase = 1;
    int t, k, MyIntFromFile;

    while((read = getline(&line_buffer, &line_size, f_ptr)) != -1){	
      
      if(line_number == ArrInfo){
	  
	  if(sscanf(line_buffer,"%d %d",&NumLin, &NumCol) != 2){
	      printf("Something is wrong with line number %d \n",line_number);
	      exit(EXIT_FAILURE);
	  }else{
	      /* ArrInfo controls where the program 
	      should stop feeding our array, send it to
	      the cutCrops function, free it and allocate a new one.*/ 
	      ArrInfo = ArrInfo + NumLin + 1;
	      // Using the info we allocate a new dynamic 2d array
	      Arr_Harvest = AllocateField(NumLin,NumCol);
	   }
	  
	  
      }else if(line_number > 0){
	// Feed the Arr_Harvest of info from getline
	k = 0; 
	while(k < read){
	    // Each number is seperated by the delimiter, ' ' 
	    t = 0; 
	    // Here we convert the char* into a number
	    // Obtain the number of letters in the number, k is the position
	    while(line_buffer[k] != ' ' && line_buffer[k] != '\n'){
		t++;
		k++;
	    }
	    // Extract the substring needed from line_buffer and convert it
	    // to a number then save it to the array.
	    if(line_buffer[k-t] == ' ' || line_buffer[k-t] != '\n'){
	      char *temp = NULL;
	      temp = substring(line_buffer,k-t+1, t);
	      sscanf(temp,"%d",&MyIntFromFile);
	      Arr_Harvest[i][j] = MyIntFromFile;
	      j++;
	      free(temp);
	    }
	    k++;
	}
	
	if(j == NumCol){
	  i++;
	  j=0;
	}
	 
      // Obtain the number of cases
      }else if(line_number == 0){
	if(sscanf(line_buffer, "%d",&CaseNumber) != 1){
	  printf("Something is wrong with line number %d \n",line_number);
	  exit(EXIT_FAILURE);
	}
      }
      line_number++;
      
      // Free the dynamic array if the line of info for next array has been reached
      if(line_number == ArrInfo && line_number != 1){ 
	  i = 0;
	  printf("Case #%d: ",CurrCase);
	  CurrCase++;
	  // Check the array!
	  CutCrops(Arr_Harvest,NumLin,NumCol);
	  FreeField(Arr_Harvest,NumLin);
	  if(CurrCase > CaseNumber){
	    printf("Number of cases exceeded");
	    break;
	  }
      }
    }
// Close file and deallocate memory
    fclose(f_ptr);
    free(line_buffer);
}

// This function generates a proper cut field in a pseudo-chaotic way
// Good for checking the algo
int** RandomField(int lines, int columns)
{
  int** Array = NULL;
  int Trips, x, y, z;
  Array = AllocateField(lines,columns);
  // Put 1 everywhere for uncut
  for(int i = 0; i < lines; i++){
    for(int j = 0; j < columns; j++){
      Array[i][j] = 1;
    }
  }
  
  Trips = (rand() % 19);
  for (int i = 0; i < Trips; i++){
    x = rand() % 2;
    y = 1 + (rand() % 9);
    z = rand() % lines;
    if(x == 0){
      for(int j = 0; j < columns; j++){
	if(Array[z][j] < y){
	  Array[z][j] = y;
	}
      }
    }else{
      for(int j = 0; j < lines; j++){
	if(Array[j][z] < y){
	  Array[j][z] = y;
	}
      }
    }
  }
  return Array;
}

int main (int argc, char *argv[])
{
    
    FILE *f_ptr = fopen(argv[1], "r");
    // In case we don't have a proper file
    if(f_ptr == NULL){
      perror("Error: ");
      exit(EXIT_FAILURE);;
    }
    else{
      scanfile(f_ptr);
      exit(EXIT_SUCCESS);;
    }

}