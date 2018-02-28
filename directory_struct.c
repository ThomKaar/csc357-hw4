/* Written for God by His grace. */

#include "directory_struct.h"

D_Node * new_d_node(){
   D_Node *new;
   new = malloc(sizeof(D_Node));
   new->entries = NULL;
   new->stat = NULL;
   new->dir_ent = NULL;
}

void set_stat(D_Node *dir_node, struct stat* sb){
  dir_node->stat = sb;
}

void set_dirent(D_Node *dir_node, struct dirent* dp){
  dir_node->dir_ent = dp;
}

D_Child* new_child(D_Node* d_node){
  D_Child* child = (D_Child) malloc(sizeof(D_Child));
  child->node = d_node;
  next = NULL;
}

Child_List* new_list(D_Node* d_node){
  Child_List* list = (Child_List) malloc(sizeof(Child_List));
  D_Child* child = new_child(d_node);
  list->front = child;
  list->back = child;
  list->size = 1;
}

void add_child(Child_List* list, D_Node* d_node){
  D_Child* new_child = new_child(d_node);
  list->back->next = new_child;
  list->back = new_child;
  list->size = list->size++;
}

D_node* create_tree(char* pathname){
  /* Initialize variables */
  DIR* dp;
  struct stat* stat_buf;
  struct dirent* dir_ent;
  D_Node* dir_node;

  dp = opendir(pathname);
  lstat(pathname, stat_buf);

  /* Make root D_node */
  dir_node = new_d_node();
  dir_ent = readdir(dp);
  dir_node->sb = stat_buf;
  dir_node->dir_ent = dp;
  /* Make root D_node */

  while(dirent){
    D_Node* d_node;
    struct stat* stat_buf;

    lstat(dirent->d_name, stat_buf);
    d_node = new_d_node();
    d_node->sb = stat_buf;
    d_node->dir_ent = NULL;
    if(S_ISDIR(d_node->stat->mode_t)){
      struct dirent* dir_ent;

      create_tree(dirent->d_name);

    }

    dirent = readdir(dp);
  }
}
/* Tommy's old code, delete when passed this point
void add_DIR_child(D_Node * d_node, D_Child* d_child){
   if(d_node->entries == NULL){
      d_node->entries =  d_child;
   }
   else{
      add_childD_Node(d_node->entries->node, d_child);
   }
}*/
