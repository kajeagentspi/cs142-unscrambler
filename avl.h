#define BALANCED 0
#define LEFT_LEANING 1
#define RIGHT_LEANING 2
#define TRUE 1
#define FALSE 0
typedef struct node_tag{
  int height;
  char *string;
  struct node_tag *parent;
  struct node_tag *left;
  struct node_tag *right;
} avl_node;

int max(int a, int b){
  return (a > b ? a : b);
}

void updateheight(avl_node *temp){
  if (temp != NULL)
    temp->height = max(temp->left == NULL ? -1 : temp->left->height, temp->right == NULL ? -1 : temp->right->height) + 1;
}

void left_rotate(avl_node **rootptr){
  avl_node *temp1;

  if (rootptr != NULL && *rootptr != NULL && (*rootptr)->right != NULL){
    temp1 = (*rootptr)->right;

    (*rootptr)->right = temp1->left;
    if (temp1->left != NULL)
      temp1->left->parent = (*rootptr);

    temp1->parent = (*rootptr)->parent;

    (*rootptr)->parent = temp1;
    temp1->left = (*rootptr);

    if (temp1->parent != NULL){
      if (temp1->parent->right == *rootptr)
        temp1->parent->right = temp1;
      else
        temp1->parent->left = temp1;
    }

    *rootptr = temp1;

    updateheight(temp1->left);
    updateheight(temp1);
  }
}

void right_rotate(avl_node **rootptr){
  avl_node *temp1;

  if (rootptr != NULL && *rootptr != NULL && (*rootptr)->left != NULL){
    temp1 = (*rootptr)->left;

    (*rootptr)->left = temp1->right;
    if (temp1->right != NULL)
      temp1->right->parent = (*rootptr);

    temp1->parent = (*rootptr)->parent;

    (*rootptr)->parent = temp1;
    temp1->right = (*rootptr);

    if (temp1->parent != NULL){
      if (temp1->parent->left == *rootptr)
        temp1->parent->left = temp1;
      else
        temp1->parent->right = temp1;
    }

    *rootptr = temp1;

    updateheight(temp1->right);
    updateheight(temp1);
  }
}

void insert_fixup(avl_node **rootptr, avl_node *temp){
  int current = BALANCED, previous, lh, rh;

  do{
    lh = (temp->left == NULL ? -1 : temp->left->height);
    rh = (temp->right == NULL ? -1 : temp->right->height);

    previous = current;
    current = (lh == rh ? BALANCED : (lh > rh ? LEFT_LEANING : RIGHT_LEANING));

    if (abs(lh - rh) > 1){
      if (current == LEFT_LEANING){
        if (previous == LEFT_LEANING)
          right_rotate(&temp);
        else{
          right_rotate(&temp);
          left_rotate(&(temp->left));
        }
      }
      else{
        if (previous == RIGHT_LEANING)
          left_rotate(&temp);
        else{
          left_rotate(&temp);
          right_rotate(&(temp->right));
        }
      }
    } //end if

    updateheight(temp);

    if (temp->parent == NULL)
      *rootptr = temp;

    temp = temp->parent;

  } while (temp != NULL);
}

void insert_node(avl_node **rootptr, avl_node *temp){
  if (*rootptr == NULL)
    *rootptr = temp;
  else{
    temp->parent = *rootptr;

    if (strcmp((*rootptr)->string, temp->string) > 0)
      insert_node(&((*rootptr)->right), temp);
    else{
      insert_node(&((*rootptr)->left), temp);
    }
  }
}

void insert_value(avl_node **rootptr, char *string){
  avl_node *temp;
  temp = (avl_node *)malloc(sizeof(avl_node));
  temp->string = string;
  temp->height = 0;
  temp->parent = temp->left = temp->right = NULL;
  insert_node(rootptr, temp);
  insert_fixup(rootptr, temp);
}

// search the given value in the tree return the node
int search_value(avl_node **rootptr, char *string){
  avl_node *temp = *rootptr;

  while (temp != NULL){
    if (strcmp(temp->string, string) == 0){
      return TRUE;
    }
    else if (strcmp(temp->string, string) > 0){
      temp = temp->right;
    }
    else{
      temp = temp->left;
    }
  }
  return FALSE;
}

void view(avl_node *root, int tabs){
  int i;
  if (root != NULL){
    view(root->right, tabs + 1);
    for (i = 0; i < tabs; i++)
      printf("\t");
    printf("%s\n", root->string);
    view(root->left, tabs + 1);
  }
}

void viewAVLasList(avl_node *root, int* wordCount){
  int i;
  if(root != NULL){
    viewAVLasList(root->right,wordCount);
    printf("%s\n", root->string);
    *wordCount+=1;
    viewAVLasList(root->left,wordCount);
  }
}