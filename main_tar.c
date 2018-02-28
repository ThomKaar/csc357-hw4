/* Written for God by His grace. */

#include "flags_helper.h"
#include "archive_creation.h"

int main(int argc, char* argv[]){

   /*DIR *dirp;*/
   /*struct dirent *direntp;*/
   int rfd;
   int wfd;

   if(!flags_has_f(argv)){
      perror("mytar");
      exit(EXIT_FAILURE);
   }

   if(flags_incorrect_length(argv)){
     perror("mytar");
     exit(EXIT_FAILURE);
   }
     
   if((wfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
      printf("Can't open the write file\n");
      exit(EXIT_FAILURE);
   }

   if((rfd = open(argv[3], O_RDONLY)) < 0){
      printf("Can't open the read file\n");
      close(wfd);
      exit(EXIT_FAILURE);
   }

   /*dirp = opendir(argv[2]);*/
   /*direntp = readdir(dirp);*/
   /*header = create_header(argv[2]);
   write_header(header, outfd);  */

   write_entry(argv[3],argv[2], rfd, wfd); 
  
   close(wfd);
   close(rfd);
   return 0;

}
