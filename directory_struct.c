/* Written for God by His grace. */

#include "directory_struct.h"

D_Node * empty_D_Node(){
   D_Node *new;
   new = malloc(sizeof(D_Node));
   new->entries = NULL;
   new->stat = NULL;
   new->direntp = NULL;
}

void add_DIR_child(D_Node * d_node, D_Child* d_child){
   if(d_node->entries == NULL){
      d_node->entries =  d_child;  
   }
   else{
      add_childD_Node(d_node->entries->node, d_child);
   }
}

 
