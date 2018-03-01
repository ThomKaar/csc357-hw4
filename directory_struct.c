/* Written for God by His grace. */

#include "directory_struct.h"

struct stat *safe_stat(const char *path, struct stat *buf){
   if (!lstat(path, buf))
   {
      perror("safe stat");
      exit(EXIT_FAILURE);
   }
   return buf;
}

void *safe_malloc(size_t size){
   void *new = malloc(size);
   if(new == NULL)
   {
      perror("safemalloc()");
      exit(EXIT_FAILURE);
   }
   return new;
}

int safe_strlen(char *str){
  if(str == NULL){
    return 0;
  }
  return strlen(str);
}

char *mycwd(){
  char* directory;
  directory = (char*) safe_malloc(sizeof(char) * 155);
  if(getcwd(directory, 155)){
    perror("mycwd");
    exit(EXIT_FAILURE);
  }
  return directory;
}

D_Child* new_child(D_Node* d_node){
  D_Child* child = (D_Child*) safe_malloc(sizeof(D_Child));
  child->node = d_node;
  child->next = NULL;
  return child;
}

Child_List* new_list(){
  Child_List* list = (Child_List*) safe_malloc(sizeof(Child_List));
  list->front = NULL;
  list->back = NULL;
  list->size = 0;
  return list;
}

void list_add_child(Child_List* list, D_Node* d_node){
  D_Child* child;
  child = new_child(d_node);
  if(list->size == 0){
    list->front = child;
    list->back = child;
    list->size = 1;
  }else{
    list->back->next = child;
    list->back = child;
    list->size = list->size + 1;
  }
}

D_Node * new_d_node(){
   D_Node *new;
   new = safe_malloc(sizeof(D_Node));
   new->entries = new_list();
   new->sb = NULL;
   new->dir_ent = NULL;
   return new;
}

void set_stat(D_Node *dir_node, struct stat* stat_buf){
  dir_node->sb = stat_buf;
}

void set_dirent(D_Node *dir_node, struct dirent* dp){
  dir_node->dir_ent = dp;
}

void set_name(D_Node *dir_node, char *dir_name){
  char* new_name = (char*) safe_malloc(sizeof(char)
                                          * (safe_strlen(dir_name) + 1));
  strcpy(new_name, dir_name);
  dir_node->name = new_name;
}

void add_child(D_Node *dir_node, D_Node *d_node){
  list_add_child(dir_node->entries, d_node);
}

void print_node(D_Node* d_node){
  printf("%s\n", d_node->name);
}

D_Node* create_tree(char* pathname){
  /* Initialize variables */
  DIR* dp;
  struct stat* stat_buf;
  struct dirent* dir_entry;
  D_Node* dir_node;

  dp = opendir(pathname);
  stat_buf = NULL;
  safe_stat(pathname, stat_buf);

  /* Make root D_node */
  dir_node = new_d_node();
  set_stat(dir_node, stat_buf);
  /* Make the root/parent D_node */

  while((dir_entry = readdir(dp))){
    D_Node* d_node;
    struct stat* s_buf;
    s_buf = NULL;

    /* IF the directories are not named "." or "..", exectute the remainder */
    if(!strcmp(dir_entry->d_name, ".") || !strcmp(dir_entry->d_name, "..")){
      continue;
    }

    d_node = new_d_node();
    safe_stat(dir_entry->d_name, s_buf);
    set_stat(d_node, s_buf);
    set_dirent(d_node, dir_entry);
    set_name(d_node, dir_entry->d_name);
    print_node(d_node);
    if(S_ISDIR(d_node->sb->st_mode)){
      /*struct dirent* new_dir_ent;
      new_dir_ent = NULL;*/
      printf("===================\n");
      create_tree(dir_entry->d_name);
      printf("===================\n");
    }

    /* add this d_node we just created to the D_Node from outside the loop */
    add_child(dir_node, d_node);
  }

  return dir_node;
}
