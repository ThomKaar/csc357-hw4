/*Written for God by His infinite grace. */

#include "checkit.h"
#include "unpack_helper.h"
int main(){
   test_octal_to_decimal();
   return 0;
}

void test_octal_to_decimal(){
   test_octal_to_decimal_0();

}

void test_octal_to_decimal_0(){
   char[8] octal;
   long expected;
   octal = "0000015";
   expected = 13;
   checkit_long(octal_to_decimal(octal, 8), expected);  
}
