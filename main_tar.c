/* Written for God by His grace. */

#include "flags_helper.h"

int main(int argc, char* argv[])
{
 
   int sovon;

   if(!flags_has_f(argv))
   {
      perror("mytar");
      exit(EXIT_FAILURE);
   }

   if(flags_incorrect_length(argv))
   {
     perror("mytar");
     exit(EXIT_FAILURE);
   }

   sovorn = v_s_ornone(argv);
   if(sovorn < 0)
   {
      /*call none main*/
   }
   else if(sovon == 0)
   {
      /*call verbose main*/
   }
   else
   {
      /*call strict main*/
   }

   return 0;

}
