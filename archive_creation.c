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

void create_path(char *path, struct stat *sb_list, int i)
{
   int temp;
   DIR *dirp;
   struct dirent *direntp;
   struct stat sb;
   int n;
    
   temp = i;
   while(temp > 0)
   {
      dirp = opendir(path);
      
      while((direntp = readdir(dirp)) != NULL)
      {
         stat(direntp->d_name, &sb);
         lstat(path, &sb);
         printf("dirent i_node is: %ld\t stat i_node is: %ld\n", direntp->d_ino, sb.st_ino);
         if(sb.st_ino == sb_list[temp-1].st_ino && sb.st_dev == sb_list[temp-1].st_dev)
         {
            add_to_path(path, direntp->d_name);
            n = strlen(path);
            path[n] = '/';
