/* Thor H. Jonsson 06/12/14
Approach will be similar to Graham's scan algorithm
First we construct a convex hull using Graham's scan algorithm.
Then we draw a line from each of the points on the hull
to all of the other points. Then we use qsort to sort the points by using
a comparison function that orders the points by using their orientation
Finally we measure the area for each of the polygons that correspond to a
point on the convex hull and choose the biggest.*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
typedef struct Point
{
    //x coordinate
    double x;
    //y coordinate
    double y;
    //number in input file
    int inputNum;
    
}Point;

// A global point needed for sorting points with reference to the first point
// Used in compare function of qsort()
Point RefP;

// A utility function to calculate the area of a simple polygon
double PolygonArea(Point *ArrP,int NumofP)
{
  double area = 0;
  int sum = 0;
  for(int i = 0; i < NumofP; i++){
    if(i+1 == NumofP){
      sum = sum + (ArrP[i].x*ArrP[0].y - ArrP[0].x*ArrP[i].y);
    }else{
      sum = sum + (ArrP[i].x*ArrP[i+1].y - ArrP[i+1].x*ArrP[i].y);
    }
  }
    area = (double) abs(sum)/2;
    return area;
}
// A utility function to swap two points
void swap(Point *p1, Point *p2)
{
    Point temp;
    temp.x = p1->x;
    temp.y = p1->y;
    p1->x = p2->x;
    p1->y = p2->y;
    p2->x = temp.x;
    p2->y = temp.y;
}
 
// A utility function to return square of distance between p1 and p2
double dist(Point p1, Point p2)
{
  double dist = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);   
  return dist;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
    // if q.y - p.y is negative then p.y > q.y
    double val = (q.y - p.y)*(r.x - q.x) - (q.x - p.x)*(r.y - q.y);
 
    if (val == 0) {
      return 0;  // colinear
    } else if (val > 0){
      return 1;
    } else {
      return 2;
    }
    
}

/* A function used by library function qsort() to sort an array of
  points with respect to the first point.
  The  comparison  function must return an integer less than, equal to,
  or greater than zero if the first argu-ment is considered to be 
  respectively less than, equal to, or greater than the second.*/
int Compare(const void *vp1, const void *vp2)
{
   Point *p1 = (Point *)vp1;
   Point *p2 = (Point *)vp2;
 
   // Find orientation
   int o = orientation(RefP, *p1, *p2);
   if (o == 0){
     // The point that is closer to the reference point is moved in front
     // in case the points are colinear
      if(dist(RefP, *p2) >= dist(RefP, *p1)){
	  return -1; 
      }else{
	  return 1;
      }
   }else if(o == 2){
      return -1;
   }else{
     return 1;
   }
}


void drawPolygon(Point *ArrP, int NumberofPoints)
{
  FILE *pipe_gp = popen("gnuplot -persist", "w");

  fputs("set xrange [0:110]\n", pipe_gp);
  fputs("set yrange [0:110]\n", pipe_gp);
  fputs("plot '-' u 1:2 w lp\n", pipe_gp);

  for (int i = 0; i < NumberofPoints; i++) {
    fprintf(pipe_gp, "%lf %lf\n", ArrP[i].x, ArrP[i].y);
  }
  fprintf(pipe_gp, "%lf %lf\n", ArrP[0].x, ArrP[0].y);
  fputs("e\n", pipe_gp);
  pclose(pipe_gp);
}

void PrintPolygon(Point *ArrP, int NumberofPoints, Point *InitP, int BooleanDraw)
{
  swap(&(ArrP[0]),InitP);
     // Sort n-1 points with respect to the first point.  A point p1 comes
   // before p2 in sorted ouput if p2 has larger polar angle (in 
   // counterclockwise direction) than p1
   RefP = ArrP[0];
   qsort(&ArrP[1], NumberofPoints-1, sizeof(Point), Compare);
   printf("area:%lf \n", PolygonArea(ArrP,NumberofPoints));
   for(int j = 0; j < NumberofPoints; j++){
     printf("%d ",ArrP[j].inputNum);
   }
   printf("\n");
   if(BooleanDraw == 1){
      drawPolygon(ArrP, NumberofPoints);
   }
}

// This function finds the bottom left point and then uses qsort to order 
// the points s.t. they make up a simple polygon and prints.
double simplePolygon(Point *ArrP, int NumberofPoints, Point *InitP)
{
  double area;
  //Place the Reference point at the beginning of the Array
  swap(&(ArrP[0]), InitP);
 
   // Sort n-1 points with respect to the first point.  A point p1 comes
   // before p2 in sorted ouput if p2 has larger polar angle (in 
   // counterclockwise direction) than p1
   RefP = ArrP[0];
   qsort(&ArrP[1], NumberofPoints-1, sizeof(Point), Compare);
   area = PolygonArea(ArrP,NumberofPoints);
   return area;
   
}

// Tries out different reference points for our polygon
double FindMaxPolygon(Point *ArrP, int NumberofPoints, int BooleanPrint)
{
   
  double MaxArea = 0;
  double currArea = 0;
  Point *MaxRefP = &(ArrP[0]);
   for(int k = 0; k < NumberofPoints; k++){
     currArea = simplePolygon(ArrP,NumberofPoints, &(ArrP[k]));
     if(currArea > MaxArea){
       MaxArea = currArea;
       *MaxRefP = ArrP[k];
     }
   }
   if ( BooleanPrint == 1){
      PrintPolygon(ArrP,NumberofPoints,MaxRefP,1);
   }
   return MaxArea;
}


// This function finds the bottom left point and then uses qsort to order 
// the points s.t. they make up a simple polygon and prints.
double convexHull(Point *ArrP, int NumberofPoints, int BooleanPrint)
{
  int empty;
  //bottommost point
  int Min = 0;
  Point temP = ArrP[0];
  
  // Finds the bottom
  for(int i = 1; i < NumberofPoints; i++){
    if (ArrP[i].y < temP.y || (ArrP[i].y == temP.y && ArrP[i].x < temP.x)){
      temP = ArrP[i];
      Min = i;
    }
  }
  
     // Place the bottom-most point at first position
   swap(&(ArrP[0]), &(ArrP[Min]));
 
   // Sort n-1 points with respect to the first point.  A point p1 comes
   // before p2 in sorted ouput if p2 has larger polar angle (in 
   // counterclockwise direction) than p1
   RefP = ArrP[0];
   qsort(&ArrP[1], NumberofPoints-1, sizeof(Point), Compare);
   Point *HullArr = (Point *)malloc(sizeof(Point)*NumberofPoints);
   for(int j = 0; j < 3; j++){
      HullArr[j] = ArrP[j];
   }
   empty = 3;
   for(int l = 3; l < NumberofPoints; l++){
     // if the third point yields a counterclockwise turn it is put in this stack
	while(orientation(HullArr[empty-2],HullArr[empty-1],ArrP[l]) != 2){
	  empty--;
      }
      HullArr[empty] = ArrP[l];
      empty++;
   }
   
   // Now HullArr contains all the points that make up the convexHull 
   // Lets make a simple polygon tracing from all of them and find max area
  double MaxArea = 0;
  double currArea;
  Point *MaxRefP = &(HullArr[0]);
   for(int k = 0; k < empty; k++){
     currArea = simplePolygon(ArrP,NumberofPoints, &(HullArr[k]));
     if(currArea > MaxArea){
       MaxArea = currArea;
       *MaxRefP = HullArr[k];
     }
   }
   if ( BooleanPrint == 1){
      PrintPolygon(ArrP,NumberofPoints,MaxRefP,1);
   }   free(HullArr);
   return MaxArea;
}

// The very first line contains number of cases
// The info line contains number of points
void scanfile(FILE *f_ptr)
{
  
  Point *ArrP = NULL;
  
  char *line_buffer = NULL;
  size_t line_size = 0;
  int info_line = 1;
  int NumofCases;
  int CaseNumber = 1;
  int NumofP = 0;
  int line_number = 0;
  int read;
  double x,y;
  int i = 0;
  
  while((read = getline(&line_buffer, &line_size, f_ptr)) != -1){
    //info_line is a line that gives me the dimensions of the next input
    if(line_number == info_line){
	
      if(sscanf(line_buffer,"%d", &NumofP) != 1){
	printf("Something is wrong with line number: %d \n", line_number);
	exit(EXIT_FAILURE);
      }
      info_line = info_line + NumofP + 1;
      if(ArrP != NULL){
	free(ArrP);
      }
      // This array will store all the vertices of the simple polygon
      ArrP = (Point *)malloc(sizeof(Point)*NumofP);
      if(ArrP == NULL){
	 printf("unable to allocate memory \n");
	 exit(EXIT_FAILURE);
      }
      
    
      
    }else if(line_number > 0 && i < NumofP){
      // Read the vertices from the file into the array
      if(sscanf(line_buffer,"%lf %lf", &x, &y) != 2){
	printf("Something is wrong with line number: %d \n", line_number);
	exit(EXIT_FAILURE);
      }else{
	ArrP[i].x = x;
	ArrP[i].y = y;
	ArrP[i].inputNum = i;
	i++;
	if(i == NumofP){
	  printf("Case #%d:",CaseNumber);
	  CaseNumber++;
	  // These functions return the area for each method respect
	  //FindMaxPolygon(ArrP,NumofP,1);
	  convexHull(ArrP,NumofP,1);
	  i = 0;
	}
      }
      
    }else if(line_number == 0){
      
      if(sscanf(line_buffer,"%d",&NumofCases) != 1){
	printf("Something is wrong with line number: %d \n",line_number);
	exit(EXIT_FAILURE);
      }
    }
    line_number++;
  }
  fclose(f_ptr);    
  free(line_buffer);
  free(ArrP);
}

// function that writes a pointer of points to a file
void write2file(Point *ArrP, int NumberofPoints)
{
  int NumberOfCases = 1;
  FILE* fp;
  fp = fopen("agriculture.txt","w");
  if(fp == NULL){
    printf("can't create output file");
    exit(EXIT_FAILURE);
  }
  fprintf(fp,"%d\n",NumberOfCases);
  fprintf(fp,"%d\n",NumberofPoints);
  for(int i = 0; i < NumberofPoints; i++){
    fprintf(fp,"%lf %lf\n",ArrP[i].x,ArrP[i].y);
  }
  fclose(fp);
}

void initRandomSequence()
{
  time_t t; 
  srand((unsigned) time(&t));
}

// Gives a double random number on a given interval
double GiveMeDoubleRandom(int LengthOfInterval)
{
  return ((double)rand()/(double)RAND_MAX)*LengthOfInterval;
}


// function that creates a pointer with a given number of random points
Point* randPoints(int NumberofPoints)
{
  int LengthOfInterval = 100;
  Point* ArrP = (Point *)malloc(sizeof(Point)*NumberofPoints);
  initRandomSequence();
  for(int i = 0; i < NumberofPoints; i++){
    ArrP[i].x = GiveMeDoubleRandom(LengthOfInterval); 
    ArrP[i].y = GiveMeDoubleRandom(LengthOfInterval);
    ArrP[i].inputNum = i;
  }
  return ArrP;
}
// A utility function to print a pointer of points
void PrintPointArr(Point *ArrP,int NumberofPoints)
{
    for(int i = 0; i < NumberofPoints; i++){
      printf("%d  : %lf , %lf\n",ArrP[i].inputNum, ArrP[i].x, ArrP[i].y);
    }
}

Point* CopyPointer(Point *OriginalArrP,int NumberofPoints)
{
  Point* ArrCpy = (Point *)malloc(sizeof(Point)*NumberofPoints);
  for(int i = 0; i < NumberofPoints; i++){
    ArrCpy[i].x = OriginalArrP[i].x;
    ArrCpy[i].y = OriginalArrP[i].y;
    ArrCpy[i].inputNum = OriginalArrP[i].inputNum;
  }
  return ArrCpy;
}  

// function that compares the convex hull method of building a polygon
// with the brute force method of building a polygon. 
void compareMethods()
{
  FILE *pipe_gp = popen("gnuplot -persist","w");
  double area1, area2, avgAreaDiff;
  Point* ArrP;
  fputs("set xrange [0:20]\n", pipe_gp);
  fputs("set yrange [-10:1600]\n", pipe_gp);
  fputs("plot '-' u 1:2 w lp\n", pipe_gp);
  int MaxNumOfP = 20;
  int k = 0;
  int NumberOfIterations = 1000;
  for(int i = 3; i < MaxNumOfP; i++){
    avgAreaDiff = 0;
    for(int j = 0; j < NumberOfIterations; j++){
      if(ArrP == NULL){
	printf("Array could not be allocated");
	exit(EXIT_FAILURE);
      }
      ArrP = randPoints(i);
      printf("Begin");
      PrintPointArr(ArrP,i);
      area1 = convexHull(ArrP,i,0);
      PrintPointArr(ArrP,i);
      area2 = FindMaxPolygon(ArrP,i,0);
      PrintPointArr(ArrP,i);
      if(k == 0 && (area1 - area2) < 0){
	convexHull(ArrP,i,1);
	FindMaxPolygon(ArrP,i,1);
	k = 1;
	printf("I will stop now");
	exit(EXIT_SUCCESS);
      }
	
      avgAreaDiff = avgAreaDiff + (area1 - area2);
      
      free(ArrP);
    }
    avgAreaDiff = avgAreaDiff/(double)NumberOfIterations;
    printf("%lf\n", avgAreaDiff);
    fprintf(pipe_gp, "%d %lf\n", i, avgAreaDiff);
  }
  fputs("e\n", pipe_gp);
  pclose(pipe_gp);
}

void WriteRand2File(int NumberofPoints)
{
  write2file(randPoints(NumberofPoints), NumberofPoints);
}

// Driver program to test above functions
int main(int argc, char *argv[])
{
     // WriteRand2File(100);
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