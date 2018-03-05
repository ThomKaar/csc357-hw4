/* Written for God by His grace. */

#include "flags_helper.h"
#include "archive_creation.h"
#include "unpack_helper.h"
#include "directory_struct.h"
#include "table.h"

/*argv[0] -> ./mytar
 * argv[1] -> flags
 * argv[2] -> tarfile
 * argv[3]+ -> path */
int main(int argc, char* argv[]){

   DIR *dirp;
   /*struct dirent *direntp;*/
   int rfd;
   int wfd;
   int path_files_left;
   int current_path_index;

   if(!flags_has_f(argv)){
      perror("mytar");
      exit(EXIT_FAILURE);
   }

   if(flags_incorrect_length(argv)){
     perror("mytar");
     exit(EXIT_FAILURE);
   }

   if(argc < 3)
   {
      perror("mytar");
      exit(EXIT_FAILURE);
   }
   if(flags_contain_c(argv)){

      if((wfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
         printf("Can't open the write file\n");
         exit(EXIT_FAILURE);

      }
      path_files_left = 1;
      current_path_index = 3;
      if( argc  > 4){
         path_files_left = argc - 3;
         printf("number of path_files is: %d\n", path_files_left);
      }
      while(path_files_left > 0){
         if((rfd = open(argv[current_path_index], O_RDONLY)) < 0){
            printf("Can't open the read file\n");
            path_files_left--;
            current_path_index++;
            continue;
         }
         dirp  = opendir(argv[current_path_index]);
         write_entry(argv[current_path_index], argv[2], rfd, wfd);
         if(dirp == NULL){
            path_files_left--;
            current_path_index++;
            close(rfd);
            continue;
         }
         write_entries(dirp, argv[current_path_index], argv[2], rfd, wfd);
         path_files_left--;
         current_path_index++;
         }
         write_last_two_blocks(wfd);
         close(wfd);
         close(rfd);
         return 0;
   }
   else if(flags_contain_x(argv)){
      store_all(argv[2]);
   }
   else if(flags_contain_t(argv)){
      /*Put the table main here. */
      int fd;
      char c;
      fd = open(argv[2], O_RDONLY);

      if(flags_contain_v(argv)){
         c  = 'v';
      }
      else{
         c = 's';
      }
      print_table(fd, c);

      

      close(fd);
   }
   return 0;

}
