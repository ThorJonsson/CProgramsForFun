// Púkk - Thor H. Jonsson
// Begin with five players

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Card
{
  // suit of card
  // e.g. clubs (♣), diamonds (♦), hearts (♥) and spades (♠),
  char suit;
  // Number is from 1 to 13, representing respectively:
  // 2,3,4,5,6,7,8,9,10,prince,queen,king,ace
  int number;
}Card;

typedef struct User
{
  // and a score
  int score;
  // empty array with 5 cards
  Card* hand;
  // face
  Card face;
}User;

// This function initalizes a deck of cards
Card* init_deck(void)
{
  int NumberofCards = 52;
  const char suits[] = {'c','d','h','s'};
  int curr;
  Card* DeckofCards = (Card *)malloc(sizeof(Card)*NumberofCards);
  if(DeckofCards == NULL){
    printf("unable to allocate memory \n");
    exit(EXIT_FAILURE);
  }
  
  for(int i = 0; i < 4; i++){
    for(int j = 1; j <= 13; j++){
      curr = j+i*13-1;
      DeckofCards[curr].suit = suits[i];
      DeckofCards[curr].number = j;
    }
  }
  return DeckofCards;
}

// A utility function to swap two cards
void swap_cards(Card *x, Card *y)
{
  Card temp = *x;
  *x = *y;
  *y = temp;
}

// A utility function to print a deck of cards
void print_deck(Card* DeckofCards, int NumberofCards)
{
  for(int i = 0; i < NumberofCards; i++){
      printf("%c, %d \n",DeckofCards[i].suit,DeckofCards[i].number);
  }
}

void print_users(User* UserArr, int NumberofUsers)
{
  for(int j = 0; j < 5; j++){
    printf("User %d has score %d and the following cards",j, UserArr[j].score);
    print_deck(UserArr[j].hand,5);
  }
}

//Implements the Knuth Shuffle also known as Fisher-Yates shuffle
void Knuth_shuffle(Card* DeckofCards, int NumberofCards)
{
  int rand_num;
  time_t t;

  // Intializes random number generator 
   srand((unsigned) time(&t));
  // We go through the deck and in each iteration exchange 
  // the current card for a random one.
  for(int i = 0; i < NumberofCards; i++){
    rand_num = rand() % NumberofCards;
   
    swap_cards(&DeckofCards[rand_num],&DeckofCards[i]);
  }  
}

// initialize faces 
Card* init_faces(int NumberofUsers)
{
  Card *facesArr = (Card *)malloc(sizeof(Card)*NumberofUsers);
  for(int i = 0; i < 4; i++){
      facesArr[i].suit = 'x';
      facesArr[i].number = 13-i;
  }
  // This is for the prince of clubs
  facesArr[4].suit = 'c';
  facesArr[4].number = 10;
  Knuth_shuffle(facesArr,NumberofUsers);
  return facesArr;
}

// Deal cards
void DealCards(User* UserArr,int NumberofUsers, Card* DeckofCards)
{
  // Shuffle the cards before they're dealt
  Knuth_shuffle(DeckofCards, 52);
  // Deal one card to each user at a time
  for(int j = 0; j < 5; j++){
    for(int i = 0; i < NumberofUsers; i++){
      UserArr[i].hand[j] = DeckofCards[i+j*5];
    }
  }
}

// Users must be 5 for now
User* init_users(Card *DeckofCards)
{
  int NumberofUsers = 5;
  facesArr = init_faces(NumberofUsers);
  User *UserArr = (User *)malloc(sizeof(User)*NumberofUsers);
  for(int i = 0; i < NumberofUsers; i++){
    UserArr[i].score = 100;
    // Lets give each user a face from our shuffled array of face cards
    UserArr[i].face = facesArr[i];
    UserArr[i].hand = (Card *)malloc(sizeof(Card)*5);
  }
  DealCards(UserArr, NumberofUsers, DeckofCards);
  
  return UserArr;
}

void free_users(User *UserArr,int NumberofUsers)
{
  for(int i = 0; i < NumberofUsers; i++){
    free(UserArr[i].hand);
  }
  free(UserArr);
}

int main(void)
{
  Card* DeckofCards = init_faces(5);
  print_deck(DeckofCards, 5);  
  printf("\n");
  Knuth_shuffle(DeckofCards, 5);
  print_deck(DeckofCards, 5);
  free_users(UserArr,5);
  free(DeckofCards);
  exit(EXIT_SUCCESS);
}