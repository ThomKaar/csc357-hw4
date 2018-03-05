/*Written for God by His grace. */

#define _BSD_SOURCE

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "unpack_helper.h"
#include "directory_struct.h"

void print_table(FILE* fp){

}

/* print file info
  -rw------- pnico/pnico 200 2010-11-02 13:49 Testdir/file2*/
void print_file_info(D_Node *node){
  struct tm *time_m;

  printf((S_ISDIR(node->sb->st_mode)) ? "d" : "-");
  printf((node->sb->st_mode & S_IRUSR) ? "r" : "-");
  printf((node->sb->st_mode & S_IWUSR) ? "w" : "-");
  printf((node->sb->st_mode & S_IXUSR) ? "x" : "-");
  printf((node->sb->st_mode & S_IRGRP) ? "r" : "-");
  printf((node->sb->st_mode & S_IWGRP) ? "w" : "-");
  printf((node->sb->st_mode & S_IXGRP) ? "x" : "-");
  printf((node->sb->st_mode & S_IROTH) ? "r" : "-");
  printf((node->sb->st_mode & S_IWOTH) ? "w" : "-");
  printf((node->sb->st_mode & S_IXOTH) ? "x" : "-");

  time_m = localtime(&(node->sb->st_mtime));

  printf(" %s/%s%9llu ", node->uname, node->gname,
                       (unsigned long long)node->sb->st_size);
  printf("%d-%02d-%02d %02d:%02d", time_m->tm_year + 1900, 1+time_m->tm_mon,
            time_m->tm_mday, time_m->tm_hour, time_m->tm_min);
  printf(" %s\n", node->name);
}

void print_filename(D_Node * node){
  printf("%s", node->pname);
}
/*This file is to create all helper functions for the
 * table listing functionality for mytar. */

/*This function goes one more directory deep
 * it assumes name is a directory and changes the current dirctory
 * into that given directory */
/*void dir_down(struct dirent * direntp, char * path){
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
         file_path  = (char *) malloc( (strlen(path)+
                  strlen(direntp->d_name)) * sizeof(char) );
         strcpy(file_path, path);
         strcat(file_path, direntp->d_name);
         printf("Current path is %s", file_path);
         free(file_path);
      }
   }
}*/



int main(int argc, char *argv[]){
   /*char * path;

   path = (char *) malloc(sizeof(char) * (strlen(argv[1])+3));
   strcat(path, "./");
   strcat(path, argv[1]);
   traverse_deep(path);*/
   D_Node* node;
   char buffer[512];
   int fd;

   node = (D_Node*) malloc(sizeof(D_Node));
   fd = open(argv[1], O_RDONLY);

   read(fd, buffer, 512);
   store_one(buffer, node);
   print_file_info(node);

   close(fd);

   return 0;
}
