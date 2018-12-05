void swap(char *x, char *y) { 
  char temp; 
  temp = *x; 
  *x = *y; 
  *y = temp; 
}

char* mergeTemplate(char* generated, int blanks, char* template, int templateLength, int* firstReplace){
  char *generatedWord = (char *)malloc(sizeof(char) * templateLength + 1);
  strcpy(generatedWord, template);
  int blankCounter = 0;
  for(int i = 0; i < templateLength || blankCounter < blanks; i++){
    if (generatedWord[i] = '_') {
      if (firstReplace && blankCounter == 0) {
        *firstReplace = i;
      }
      generatedWord[i] = generated[blankCounter];
      blankCounter++;
    }
  }
  return generatedWord;
}

int compare (const void * a, const void * b) {
  return ( *(char*)a - *(char*)b );
}