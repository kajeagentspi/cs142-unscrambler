#define SIZE 26
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

typedef struct trie_node{
  struct trie_node* parent;
  struct trie_node* children[SIZE];
  int isEnd;
  int charactersTillLongest;
  int charactersTillNextEnd;
  char character;
} TRIE;

TRIE* createTrieNode(TRIE* parent){
  TRIE* node = (TRIE*)malloc(sizeof(TRIE));
  // successfull malloc
  if(node){
    node -> parent = parent;
    node -> isEnd = FALSE;
    node -> charactersTillLongest = -1;
    node -> charactersTillNextEnd = 999;
    int i;
    for(i = 0; i < SIZE; i++){
      node -> children[i] = NULL;
    }
    
  }
}

// given a word check if the first length numbers can form a word
int searchForPrefixInTrie(TRIE* root, char* word, int prefixLength){
  int index;
  TRIE* iterator = root;
  // printf("%i\n", root -> children[CHAR_TO_INDEX('p')] -> charactersTillLongest);
  int wordLength = strlen(word);
  int i;
  for(i = 0; i < prefixLength; i++){
    index = CHAR_TO_INDEX(word[i]);
    if (iterator -> children[index] == NULL || iterator -> children[index] -> charactersTillLongest < wordLength || iterator -> children[index] -> charactersTillNextEnd > wordLength) {
      return FALSE;
    }
    wordLength--;
    iterator = iterator->children[index];
  }
  return TRUE;
}

void insertWordToTrie(TRIE* root, char* word){
  int length = strlen(word);
  int index;
  int remainingCharacters = length;
  int i;
  TRIE* iterator = root;
  for(i = 0; i < length; i++){
    index = CHAR_TO_INDEX(word[i]);
    if (iterator -> children[index] == NULL) {
      iterator -> children[index] = createTrieNode(iterator);
      iterator -> children[index] -> character = word[i];
    }
    if (iterator -> children[index] -> charactersTillLongest < remainingCharacters) {
      iterator -> children[index] -> charactersTillLongest = remainingCharacters;
    }
    if (iterator -> children[index] -> charactersTillNextEnd > remainingCharacters) {
      iterator -> children[index] -> charactersTillNextEnd = remainingCharacters;
    }
    remainingCharacters--;
    iterator=iterator->children[index];
  }
  iterator->isEnd=TRUE;
}

void deleteWord(TRIE* end, char* word){
  // check if there are children if there is no changes
  int i;
  for(i = 0; i < SIZE; i++){
    if (end -> children[i] != NULL) {
      return;
    }
  }
  int length = strlen(word);
  int counter = 1;
  TRIE* iterator = end;
  int subword = FALSE;
  for(i = 0; i < length; i++){
    if (iterator == NULL) {
      break;
    }else if (iterator -> isEnd == TRUE && i != 0 && subword == FALSE){
      iterator -> charactersTillLongest = counter;
      counter++;
      subword = TRUE;
    }
    if(subword){
      iterator -> charactersTillLongest = counter;
      counter++;
    }else{
      int index = CHAR_TO_INDEX(iterator -> character);
      iterator = iterator -> parent;
      free(iterator -> children[index]);
      iterator -> children[index] = NULL;
    }
  }
}

int searchWordInTrie(TRIE* root, char* word){
  int length = strlen(word);
  int index;
  int i;
  TRIE* iterator = root;
  for(i = 0; i < length; i++){
    index = CHAR_TO_INDEX(word[i]);
    if (iterator -> children[index] == NULL) {
      return FALSE;
    }
    iterator = iterator -> children[index];
  }
  if (iterator -> isEnd) {
    deleteWord(iterator, word);
    return TRUE;
  }else{
    return FALSE;
  }
}


TRIE* loadDictionary(){
  TRIE* root = createTrieNode(NULL);
  root -> charactersTillLongest = 999;
  FILE *fp = fopen("words.txt", "r");
  int loaded=0;
  int discarded=0;
  int i;
  if(fp){
    const size_t line_size = 300;
    char *line = (char *)malloc(line_size);
    char *copy = NULL;
    int alpha;
    while (fgets(line, line_size, fp) != NULL){
      alpha = TRUE;
      line[strcspn(line, "\r\n")] = '\0';
      for (i = 0; line[i]; i++){
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
    return root;
  }else{
    exit(1);
  }
}