#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE_LINE 500
#define MAX_SIZE_WORD 100

//PART 1 VARIABLES
int   searchWordLength; //length of our input string
float totalScore;
float totalCount;

//PART 2 VARIABLES
char wordListArray[MAX_SIZE_LINE][MAX_SIZE_WORD]; //every word in wordList.txt
int maxPositive = 0; //number representation of most positive
int maxNegative = 0; //number representation of most negative
int maxPositiveLocation = 0; //takes the location of word which got most positive reviews
int maxNegativeLocation = 0; //takes the location of word which got most negative reviews

//search word in the moviereview.txt file
//if indicator is -1, it is used for the first task (total score and count)
//if indicator is other than -1,it is count and it is used for the second task (positive and negative reviews)
void searchForWord(FILE* ptr, char* word, int wordLength, int indicator) {
  ptr = fopen("reviews.txt", "r");
//check whether file exists
  if (ptr == NULL) {
    printf("\n%s\n", "Error! Could not open file");
  } else {
    char line[MAX_SIZE_LINE];
    //these two variable for second task
    int positiveRev = 0;
    int negativeRev = 0;
    while (fgets(line, sizeof(line), ptr)) {
      char* token = strtok(line, " ");
      int scoreValue = atoi(token); //get score value at the beginning of a line

      while (token != NULL) {
        //search for word , if our searching word found as a substring of token(word from movieReview line) and if their sizes are equal, we found the exact word
        if (strcasestr(token, word) && strlen(token) == wordLength) {
          //for first task
          if (indicator == -1) {
            totalCount++;
            totalScore += scoreValue;
          }
          //for second task
          else {
            if (scoreValue == 3 || scoreValue == 4)positiveRev++;
            else negativeRev++;
          }
        }
        token = strtok (NULL, " ");

      }

    }
    if (positiveRev > maxPositive) {
      maxPositive = positiveRev;
      maxPositiveLocation = indicator;
    }
    if (negativeRev > maxNegative) {
      maxNegative = negativeRev;
      maxNegativeLocation = indicator;
    }
  }
  fclose(ptr);

}


int main() {
//PART1
//User Input
  printf("%s", "Enter a word: ");
  char  searchWord[20];
  scanf("%s", searchWord);
  searchWordLength = strlen(searchWord);

  FILE *reviewPtr=NULL;
  FILE *wordListPtr;

//search occurences of word in movireview.txt
  searchForWord(reviewPtr, searchWord, searchWordLength, -1);

  printf("%s appears %.1f times \nThe average score for reviews containing the word %s is %.4f \n" , searchWord, totalCount, searchWord, totalScore / totalCount);

  puts("Continuing second task\n");
//PART2

//Open wordlist.txt
  wordListPtr = fopen("listOfWords.txt", "r");

  if (wordListPtr == NULL) {
    printf("\n%s\n", "Error! Could not open file");
  } else {
    char line[MAX_SIZE_LINE];
    int lineCounter = 0;

    // we get every word from wordlist and put them into an array
    while (fgets(line, sizeof(line), wordListPtr)) {
      char* token = strtok(line, "\r");
      while (token != NULL) {
        strcpy(wordListArray[lineCounter], token);
        lineCounter++;
        token = strtok(NULL, "\r");
      }

      //it basically indicates the number of words in wordlist.txt
      int tempCounter = lineCounter;
      //we use linecounter as an indicator, that tells our function it is going to be used for second task
      for (lineCounter = 0; lineCounter < tempCounter; lineCounter++) {
        //we need that to compare with words in moviereviews
        int wordLength = strlen(wordListArray[lineCounter]);
        searchForWord(reviewPtr, wordListArray[lineCounter], wordLength, lineCounter); //search the word inside movieReview.txt
      }

    }

  }
  fclose(wordListPtr);

  printf("Word that got most positive review is %s with %d\n", &wordListArray[maxPositiveLocation][0], maxPositive);
  printf("Word that got most negative review is %s with %d", &wordListArray[maxNegativeLocation][0], maxNegative);

}