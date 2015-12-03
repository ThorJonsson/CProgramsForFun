# include <stdio.h>
#include<string.h>
/* Function to print reverse of the passed string */
static int count=1; // To count no. of different permutations.

void swap(char *s, char *p)
{
    char temp=*s;
    *s=*p;
    *p=temp;
}

void reverse_str(char *a, int i, int n)
{
  int present[256]={};
  if(i==n){
        printf("%2d. %s\n",count++,a);
        return;
   }
 
  for (int j = i; j <= n; j++){
    if (present[a[j]]==0) {
      if (i != j){
	swap(&a[i], &a[j]);
	printf("%d %d %s\n",i,j,a);
      }
      reverse_str(a, i+1, n);
      if (i != j){
	swap(&a[i], &a[j]); //backtrack
	printf("%d %d %s\n",i,j,a);
      }
      present[a[j]] = 1;
    }
  }
}
 
int main()
{
   char a[] = "0212";
   printf("permutations of %s are:\n",a);
   reverse_str(a,0,2);
   count=1; printf("\n");//count to be re-initialized.
/*

   char b[] = "011";
   printf("permutations of %s are:\n",a);
   reverse_str(b,0,2);
   count=1; printf("\n");//count to be re-initialized.
 */  
   return 0;
}