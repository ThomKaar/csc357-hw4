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
     
   wfd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
   rfd = open(argv[4], O_RDONLY);
   /*dirp = opendir(argv[2]);*/
   /*direntp = readdir(dirp);*/
   /*header = create_header(argv[2]);
   write_header(header, outfd);  */

   write_entry(argv[2],argv[4], rfd, wfd); 
   
   close(outfd);
   close(rfd);
   return 0;

}
