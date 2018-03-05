/*Written for God by His grace. */
#include "flags_helper.h"

int flags_has_f(char* argv[])  {
   int flags_len;
   int i;  
   
   flags_len = strlen(argv[1]);
   
   for(i=0; i<flags_len; i++){
      if(argv[FLAGS_INDEX][i] == 'f'){
         return 1;
      }
   }
   return 0;
   
}

int flags_incorrect_length(char *argv[]){
   
   if(strlen(argv[FLAGS_INDEX]) > 3){
      return 1;
   }
   return 0;
}

int flags_contain_c(char *argv[]){
   int i, len;
   len = strlen(argv[FLAGS_INDEX]);
   for(i = 0; i< len; i++){
      if(argv[FLAGS_INDEX][i] == 'c')
      {
         return 1;
      }
   }
   return 0;
}

int flags_contain_x(char *argv[]){
   int i, len;
   len = strlen(argv[FLAGS_INDEX]);
   for(i = 0; i< len; i++){
      if(argv[FLAGS_INDEX][i] == 'x')
      {
         return 1;
      }
   }
   return 0; 
}


int flags_contain_t(char *argv[]){
   int i, len;
   len = strlen(argv[FLAGS_INDEX]);
   for(i = 0; i< len; i++){
      if(argv[FLAGS_INDEX][i] == 't')
      {
         return 1;
      }
   }
   return 0; 
}



int v_s_ornone(char *argv[]){

   int len, i;
   len = strlen(argv[FLAGS_INDEX]);
   if(len < 3){
      return -1;
   }
   for(i = 0; i < len; i++){
      if(argv[FLAGS_INDEX][i] == 'v'){
         return 0;
      }
      if(argv[FLAGS_INDEX][i] == 'S'){
         return 1;
      }
   }
   return -1;
}

