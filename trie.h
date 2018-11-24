#define SIZE 26
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

typedef struct trie_node{
  struct trie_node *children[SIZE];
  int isEnd;
  char character;
} TRIE;

TRIE* createTrieNode(){
  TRIE* node = (TRIE*)malloc(sizeof(TRIE));
  // successfull malloc
  if(node){
    node->isEnd=FALSE;
    for(int i = 0; i < SIZE; i++){
      node->children[i]=NULL;
    }
    
  }
}

void insertWordToTrie(TRIE* root, char* word){
  int length = strlen(word);
  int index;
  TRIE* iterator = root;
  for(int i = 0; i < length; i++){
    index = CHAR_TO_INDEX(word[i]);
    if (iterator->children[index]==NULL) {
      iterator->character = word[i];
      iterator->children[index] = createTrieNode();
    }
    iterator=iterator->children[index];
  }
  iterator->isEnd=TRUE;
}

int searchWordInTrie(TRIE* root, char* word){
  int length = strlen(word);
  int index;
  TRIE* iterator = root;
  for(int i = 0; i < length; i++){
    index = CHAR_TO_INDEX(word[i]);
    if (iterator->children[index]==NULL) {
      return FALSE;
    }
    iterator = iterator->children[index];
  }
  if (iterator->isEnd) {
    return TRUE;
  }else{
    return FALSE;
  }
}

int searchForPrefixInTrie(TRIE* root, char* prefix, int length){
  int index;
  TRIE* iterator = root;
  int prefixLength = strlen(prefix);
  for(int i = 0; i < length; i++){
    index = CHAR_TO_INDEX(prefix[i]);
    if (iterator->children[index]==NULL) {
      return FALSE;
    }
    iterator = iterator->children[index];
  }
  return TRUE;
}