#include <stdio.h>
int main() {
   int i = 33;
   int * ii = &i;
   printf("pointer testing: %d", *ii);
   return 0;
}