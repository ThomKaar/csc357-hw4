/* Written for God by His grace. */

#include "flags_helper.h"
#include "archive_creation.h"

int main(int argc, char* argv[]){

   DIR *dirp;
   struct dirent *direntp;
   Header * header;
   int outfd;

   if(!flags_has_f(argv)){
      perror("mytar");
      exit(EXIT_FAILURE);
   }

   if(flags_incorrect_length(argv)){
     perror("mytar");
     exit(EXIT_FAILURE);
   }
     
   outfd = open("single_header.out", O_WRONLY | O_CREAT | O_TRUNC, 0666);
   dirp = opendir("./");
   direntp = readdir(dirp);
   header = create_header("./", direntp);
   write_header(header, outfd); 
   
   return 0;

}
