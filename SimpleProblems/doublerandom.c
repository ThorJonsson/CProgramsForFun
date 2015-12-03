
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Gives a double random number on a given interval
double GiveMeDoubleRandom(int LengthOfInterval)
{
  time_t t; 
  srand((unsigned) time(&t));
  return ((double)rand()/(double)RAND_MAX)*LengthOfInterval;
}


int main()
{
  printf("%f \n",GiveMeDoubleRandom(10));
}