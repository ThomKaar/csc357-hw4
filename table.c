/*Written for God by His grace. */

#define _BSD_SOURCE

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*This file is to create all helper functions for the table listing functionality for mytar. */

/*This function goes one more directory deep
 * it assumes name is a directory and changes the current dirctory
 * into that given directory */
void dir_down(struct dirent * direntp, char * path){
   chdir(direntp->d_name);  
   path = (char*) realloc(path, (strlen(path) + strlen(direntp->d_name)));
   strcat(path, direntp->d_name);
   printf("Current path is: %s", path);
}

void traverse_deep(char *path){
   DIR* dirp;
   struct dirent* direntp;
   struct stat* sb;
   char * file_path;

   sb = (struct stat *) malloc(sizeof(struct stat));
   dirp = opendir(path);
   while((direntp = readdir(dirp)) != NULL){
      lstat(direntp->d_name, sb);
      if(S_ISDIR(sb->st_mode)){
         dir_down(direntp, path);
         traverse_deep(path);
         path -= strlen(direntp->d_name);
      }
      else{
         file_path  = (char *) malloc( (strlen(path)+strlen(direntp->d_name)) * sizeof(char) );
         strcpy(file_path, path);
         strcat(file_path, direntp->d_name);
         printf("Current path is %s", file_path);
         free(file_path);
      }
   }
}

int main(int argc, char *argv[]){
   char * path;

   path = (char *) malloc(sizeof(char) * (strlen(argv[1])+3));
   strcat(path, "./");
   strcat(path, argv[1]);
   traverse_deep(path);
}





