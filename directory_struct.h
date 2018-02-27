/*Written for God by His grace. */
#ifndef DIRECTORY_STRUCT_H
#define DIRECTORY_STRUCT_H

#include <dirent.h>


typdef struct {
   D_Child *entires;
   struct stat* sb;
   struct dirent* direntp;
} D_Node;

typdef struct {
   D_Node* cur_node;
   D_Node* next_node;
   
} D_Child;
