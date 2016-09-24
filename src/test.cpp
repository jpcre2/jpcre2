
#define PCRE2_CODE_UNIT_WIDTH 8

#include <iostream>
#include <string.h>
#include <pcre2.h>

int main(){
   char * a = 0;
   std::cout<<&a;
   free(a);
   
   return 0;
   }
