/*Written for God by His grace. */
#ifndef DIRECTORY_STRUCT_H
#define DIRECTORY_STRUCT_H

#include <dirent.h>


typdef struct {
   Child_List *entries;
   struct stat* sb;
   struct dirent* dir_ent;
} D_Node;

typdef struct {
  D_Child* front;
  D_Child* back;
  int size;
} Child_List;

typdef struct {
   D_Node* node;
   D_Child* next;
} D_Child;
