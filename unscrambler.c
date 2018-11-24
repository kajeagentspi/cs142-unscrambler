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

TRIE* loadDictionary(){
  TRIE* root = createTrieNode();
  FILE *fp = fopen("words.txt", "r");
  int loaded=0;
  int discarded=0;
  if(fp){
    const size_t line_size = 300;
    char *line = (char *)malloc(line_size);
    char *copy = NULL;
    int alpha;
    while (fgets(line, line_size, fp) != NULL){
      alpha = TRUE;
      line[strcspn(line, "\r\n")] = '\0';
      for (int i = 0; line[i]; i++){
        if (!isalpha(line[i])) {
          alpha = FALSE;
          discarded++;
          break;
        }
        line[i] = tolower(line[i]);
      }
      if(alpha){
        loaded++;
        copy = (char*)malloc(line_size);
        strcpy(copy,line);
        insertWordToTrie(root, copy);
      }
    }
    fclose(fp);
    printf("Words Loaded: %i\n", loaded);
    printf("Words Discarded: %i\n", discarded);
    return root;
  }else{
    exit(1);
  }
}

void swap(char *x, char *y) { 
  char temp; 
  temp = *x; 
  *x = *y; 
  *y = temp; 
} 

void permute(TRIE* root, avl_node** generatedWords, char* word, int left, int right, char* template, int templateLength, int blanks){
  // blankCounter = counter of blanks
  // replaceIndex current index in word that will replace _
  int blankCounter,j,replaceIndex,firstReplace;
  if(left==right || left == blanks){
    char *string = (char *)malloc(sizeof(char) * templateLength + 1);
    strcpy(string, template);
    blankCounter = 0;
    replaceIndex = 0;
    for(j=0;j<templateLength || blankCounter<blanks; ++j){
      if(string[j] == '_'){
        string[j]=word[replaceIndex];
        replaceIndex+=1;
        blankCounter+=1;
      }
    }
    if(searchWordInTrie(root,string) && search_value(generatedWords,string)==FALSE){
      insert_value(generatedWords,string);
    }else{
      free(string);
    }
  }
  for(int i=left;i<right;i++){
    swap(word+left,word+i);
    char *string = (char *)malloc(sizeof(char) * templateLength + 1);
    strcpy(string, template);
    blankCounter = 0;
    replaceIndex = 0;
    firstReplace = 0;
    for(j=0;j<templateLength || blankCounter<blanks; ++j){
      if(string[j] == '_'){
        if(blankCounter == 0){
          firstReplace = j;
        }
        string[j]=word[replaceIndex];
        replaceIndex+=1;
        blankCounter+=1;
      }
    }
    
    if (left<templateLength && searchForPrefixInTrie(root,string,firstReplace+left)) {
      permute(root, generatedWords, word, left+1, right, template, templateLength, blanks);
    }
    free(string);
    swap(word+left,word+i);
  }
}

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
  TRIE* root = loadDictionary();
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
  permute(root, &generatedWords, unusedChar, 0, unusedLen, template, M, blanks);
  int wordCount=0;
  viewAVLasList(generatedWords,&wordCount);
  printf("Number of words found: %i\n", wordCount);
  t2 = clock();
  t=(double)(t2-t1)/(double)CLOCKS_PER_SEC;
  printf("time elapsed: %0.2f\n", t);
}