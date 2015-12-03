/*Given an input string, reverse the string word by word.

For example,
Given s = "the sky is blue",
return "blue is sky the".*/
typedef struct String
{
  
  
char* getSubString(char* sentence, int first, int last)
{
  int subStringLength = last-first+2;
  char *substring = (char *)malloc(sizeof(char)*subStringLength);
  for(int i = 0; i < subStringLength; i++){
    substring[i] = sentence[first + i];
  }
  substring[last + 2] = '\0';
  return substring;
}
int countWords(char* sentence){
  int i = 0; 
  int NumberOfWords = 1; 
  // Number of words is one more than white spaces in a sentence
  while(sentence[i] != '\0'){
    if(sentence[i] == ' '){
	NumberOfWords++;
    }
  }
  return NumberOfWords;
}
char** Sentence2Arr(char* sentence, int NumberOfWords)
{   
  char** ArrOfStrings = (char **)malloc(sizeof(char *)*NumberOfWords);
  for(int i = 0; i < NumberOfWords; i++){
    
  
}