/*Written for God by His grace. */
#ifndef DIRECTORY_STRUCT_H
#define DIRECTORY_STRUCT_H

#include <dirent.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct child_list Child_List;
typedef struct d_child D_Child;

typedef struct {

  char * pname;
  Child_List* entries;
  struct stat* sb;
  struct dirent* dir_ent;
  char uname[32];
  char gname[32];
  char name[256];
  char * filetype;
} D_Node;

struct d_child {
  D_Node* node;
  D_Child* next;
};

struct child_list {
  D_Child* front;
  D_Child* back;
  int size;
};

D_Node* create_tree(char* pathname);

#endif
