#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "avl.h"
#include "trie.h"
#include "tools.h"
clock_t startClock;
int wordCount = 0;

void permute(TRIE* root, avl_node** validWords, char* generatedWord, int left, int right, char* template, int templateLength, int blanks){
  // left == right for template strings that are not all _
  // left == blanks otherwise
  if(left == right || left == blanks){
    char *string = mergeTemplate(generatedWord, blanks, template, templateLength, NULL);
    if(searchWordInTrie(root, string) && search_value(validWords, string) == FALSE){
      clock_t currentClock = clock();
      double duration = (double)(currentClock - startClock) / (double)CLOCKS_PER_SEC;
      printf("%s %0.8fs\n", string, duration);
      insert_value(validWords, string);
      wordCount++;
    }else{
      free(string);
    }
  }
  int firstReplace;
  for(int i = left; i < right; i++){
    swap(generatedWord + left, generatedWord + i);
    char *string = mergeTemplate(generatedWord, blanks, template, templateLength, &firstReplace);
    // printf("%s\n", string);
    if (left < templateLength && searchForPrefixInTrie(root, string, firstReplace + left)) {
      permute(root, validWords, generatedWord, left + 1, right, template, templateLength, blanks);
    }
    free(string);
    swap(generatedWord + left,generatedWord + i);
  }
}



int main(int argc, char **argv){
  int characters[100];
  char *charString = argv[1];
  char *templateString = argv[2];
  int charLength = strlen(charString);
  int templateLength = strlen(templateString);
  double totalTime;
  clock_t endClock;
  TRIE* root = loadDictionary();
  avl_node *validWords = NULL;
  startClock = clock();
  if(charLength < templateLength){
    printf("Template string too long!!\n");
    exit(1);
  } else if (argc != 3){
    printf("Please enter strings\n");
    exit(1);
  }
  int usedChar = 0, blanks = 0;
  char *word = (char *)malloc(sizeof(char) * charLength + 1);
  strcpy(word, charString);
  // count number of blanks and turn all characters available in the template to -1;
  for(int i = 0; i < templateLength; i++){
    if (templateString[i] == '_') {
      blanks++;
    }else{
      for (int j = 0; j < templateLength; ++j){
        if (templateString[i] == word[j]){
          word[j] = -1;
          usedChar++;
          break;
        }
      }
    }
  }
  char *unusedChar = (char *)malloc(sizeof(char) * (charLength - usedChar));
  unusedChar[charLength - usedChar] = '\0';
  int j = 0;
  printf("%s\n", word);
  for (int i = 0; i < charLength; ++i){
    if (word[i] != -1){
      unusedChar[j] = word[i];
      j++;
    }
  }
  int unusedLen = strlen(unusedChar);
  printf("Reduced: %s  %s %d %d\n", charString, templateString, usedChar, blanks);
  qsort(unusedChar, unusedLen, sizeof(char), compare);
  printf("unused: %s %d\n", unusedChar, 45);
  printf("===GENERATED WORDS===\n");
  permute(root, &validWords, unusedChar, 0, unusedLen, templateString, templateLength, blanks);
  printf("Number of words found: %i\n", wordCount);
  endClock = clock();
  totalTime=(double)(endClock - startClock) / (double)CLOCKS_PER_SEC;
  printf("time elapsed: %0.8fs\n", totalTime);
}