/*Written for God by His grace. */

#include "archive_creation.h"

/*This file's purpose is to store helper functions to archive files for mytar. */

/*This function's purpose is to traverse through all children paths given a parent.*/
void traverse_paths(char * pathname){

}

void traverse_to_root(struct stat* sb, struct stat *sb_list, char *path)
{
   int i;
   long prev_i;
   int first;

   first = 1;
   prev_i= 0; /*default value should never be used.*/
   i = 0;
   sb_list[i] = *sb;
   i++;
   while((sb->st_ino != prev_i) || first)
   {
      chdir("../");
      prev_i = sb->st_ino;
      stat("./", sb);
      sb_list[i] = *sb;
      i++;
      
      first = 0; 
      /*This is a waste of an assignment every time except the first time.*/
   }
   create_path(path, sb_list, i-OFF_SET);
   return;
}

void header_set_name(Header* header, char* name){
      strcpy(header->name, name);
}

/*given: parse the current path */

