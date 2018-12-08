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

void permute(TRIE* root, avl_node** validWords, avl_node** testedSubstrings, char* generatedWord, int left, int right, char* template, int templateLength, int blanks){
  // left == right for template strings that are not all _
  // left == blanks otherwise
  int i;
  if(left == right || left == blanks){
    char *string = mergeTemplate(generatedWord, blanks, template, templateLength, NULL);
    if(searchWordInTrie(root, string) && search_value(validWords, string) == FALSE){
      clock_t currentClock = clock();
      double duration = (double)(currentClock - startClock) / (double)CLOCKS_PER_SEC;
      printf("%s\n", string);
      insert_value(validWords, string);
      wordCount++;
    }else{
      free(string);
    }
  }
  int firstReplace;
  for(i = left; i < right; i++){
    swap(generatedWord + left, generatedWord + i);
    char *string = mergeTemplate(generatedWord, blanks, template, templateLength, &firstReplace);
    if (left < templateLength && searchForPrefixInTrie(root, string, firstReplace + left)) {
      permute(root, validWords, testedSubstrings, generatedWord, left + 1, right, template, templateLength, blanks);
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
  int i, j;
  double totalTime;
  clock_t endClock;
  TRIE* root = loadDictionary();
  avl_node *validWords = NULL;
  avl_node *testedSubstrings = NULL;
  startClock = clock();
  if(charLength < templateLength){
    if (argc == 3) {
      printf("Template string too long!!\n");
    }
    exit(1);
  }
  int usedChar = 0, blanks = 0;
  char *word = (char *)malloc(sizeof(char) * charLength + 1);
  strcpy(word, charString);
  // count number of blanks and turn all characters available in the template to -1;
  for(i = 0; i < templateLength; i++){
    if (templateString[i] == '_') {
      blanks++;
    }else{
      for (j = 0; j < templateLength; ++j){
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
  j = 0;
  for (i = 0; i < charLength; ++i){
    if (word[i] != -1){
      unusedChar[j] = word[i];
      j++;
    }
  }
  int unusedLen = strlen(unusedChar);
  qsort(unusedChar, unusedLen, sizeof(char), compare);
  if (argc == 3) {
    printf("===GENERATED WORDS===\n");
  }
  permute(root, &validWords, &testedSubstrings, unusedChar, 0, unusedLen, templateString, templateLength, blanks);
  endClock = clock();
  totalTime=(double)(endClock - startClock) / (double)CLOCKS_PER_SEC;
  if (argc == 3) {
    printf("Number of words found: %i\n", wordCount);
  }
}