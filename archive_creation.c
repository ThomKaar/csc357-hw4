/*Written for God by His grace. */

#include "archive_creation.h"

/*This file's purpose is to store helper functions to
 archive files for mytar. */

/*This function's purpose is to traverse through all
children paths given a parent.*/
void traverse_paths(char * pathname){

}

void traverse_to_root(struct stat* sb, struct stat *sb_list, char *path)
{
   int i
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

/*given: the path (aquired by getpwd()), a char array of 100, and a char array size 155*/
void prefix_name_split(char*path, char*d_name, char*name, char* prefix){
  int i;
  int offset;

  if(strlen(d_name) > 100)
  {
    offset = strlen(d_name) - 100;
    name = (char *) safe_malloc(sizeof(char*) * 100);
    strcpy(name, (d_name + offset);

    prefix = (char *) safe_malloc(sizeof(char*) * 155);
    strcpy(prefix, path);
    strncat(prefix, d_name, offset);
  }
  else{

  }

  strcpyn(name, d_name, strlen(d_name));
  strcpy(prefix, path);



  while( path[i] != '\0' && i < 100 || i < strlen(path)){
    name[i] = path[i];
  }
  if(path[i] != '\0'){
      while()
  }
}
