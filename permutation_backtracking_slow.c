#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "avl.h"
#include "trie.h"
typedef struct word{
  char *string;
  struct word *next;
} WORD;

int main(int argc, char **argv){
  avl_node *generatedWords = NULL;
  int start, move, i, j, candidate;
  char *charString = argv[1];
  clock_t t1,t2;   // store start and end time here
  double t;
  t1 = clock();
  int N = strlen(argv[1]);
  int M = strlen(argv[2]);
  if(N < M){
    printf("Template string too long!!\n");
    exit(1);
  }

  TRIE* dictionary = loadDictionary();
  int characters[100];
  int usedChar = 0, blanks = 0;
  char *word = (char *)malloc(sizeof(char) * N);
  char *template = argv[2];
  word = strcpy(word, argv[1]);

  for (int i = 0; i < M; ++i){
    if (template[i] == '_')
      blanks += 1;
    else{
      for (int j = 0; j < N; ++j){
        if (template[i] == word[j]){
          word[j] = -1;
          usedChar += 1;
          break;
        }
      }
    }
  }

  printf("Reduced: %s  %s %d %d\n", word, template, usedChar, blanks);
  char *unusedChar = (char *)malloc(sizeof(char) * (N - usedChar));
  unusedChar[N - usedChar] = '\0';
  i = 0;
  for (int j = 0; j < N; ++j){
    if (word[j] != -1){
      unusedChar[i] = word[j];
      i += 1;
    }
  }

  int unusedLen = strlen(unusedChar);
  printf("unused: %s %d\n", unusedChar, unusedLen);
  int nopts[unusedLen + 2];                 // array of top of stacks
  int option[unusedLen + 2][unusedLen + 2]; // array of stacks of options
  printf("%s\n", charString);
  for (i = 1; i < unusedLen + 1; i++){
    characters[i] = i;
  }
  for (i = 0; i < unusedLen + 2; ++i){
    nopts[i] = 0;
    for (j = 0; j < unusedLen + 2; ++j){
      option[i][j] = 0;
    }
  }
  move = start = 0;
  nopts[start] = 1;
  printf("===GENERATED=WORDS===\n");
  while (nopts[start] > 0){ // while dummy stack is not empty
    if (nopts[move] > 0){
      move++;
      nopts[move] = 0; // initialize position - no initial candidates
      if (move == unusedLen + 1){ // solution found
        char *string = (char *)malloc(sizeof(char) * M + 1);
        strcpy(string, argv[2]);
        // Generated Strings
        // Replace _ with generated
        i = 1;
        for (j = 0; j < M && i < move; ++j){
          if (string[j] == '_'){
            string[j] = (char) unusedChar[option[i][nopts[i]] - 1];
            i += 1;
          }
        }
        if (search_value(&generatedWords, string) == FALSE && searchWordInTrie(dictionary, string)){
          t2 = clock();
          double duration = (double)(t2 - t1) / (double)CLOCKS_PER_SEC;
          printf("%s %0.8fs\n", string, duration);
          insert_value(&generatedWords, string);
        }
      }
      else if (move == 1){ // only case where we'll populate the first position
        for (candidate = unusedLen; candidate >= 1; candidate--){
          nopts[move]++;
          option[move][nopts[move]] = characters[candidate];
        }
      }
      else{ // find candidates
        char *string = (char *)malloc(sizeof(char) * M + 1);
        strcpy(string, argv[2]);
        int firstReplace = 0;
        i = 1;
        for (j = 0; j < M && i < move; ++j){
          if (string[j] == '_'){
            if (i == 1) {
              firstReplace = j;
            }
            string[j] = (char) unusedChar[option[i][nopts[i]] - 1];
            i += 1;
          }
        }
        if (searchForPrefixInTrie(dictionary, string, firstReplace + nopts[move])){
          for (candidate = unusedLen; candidate >= 1; candidate--){
            for (i = move - 1; i >= 1; i--)
              if (characters[candidate] == option[i][nopts[i]])
                break; // check top of stack of previous positions
            if (!(i >= 1)){ // meaning it did not break - candidate is not used by previous positions
              ++nopts[move];
              option[move][nopts[move]] = characters[candidate];
            }
          }
        }
        else{
          move--;        // current position has exhausted candidates so move to previous
          nopts[move]--; // remove current top on this stack
        }
      }
    }else{                // backtracking step
      move--;        // current position has exhausted candidates so move to previous
      nopts[move]--; // remove current top on this stack
    }
  }
  t2 = clock();
  t = (double)(t2-t1) / (double)CLOCKS_PER_SEC;
  printf("time elapsed: %0.8f\n", t);
}