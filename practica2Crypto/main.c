#include <stdio.h>
#include <stdlib.h>
#include "eea.h"


int main(int argc, char **argv)
{
  checkCLI(argc);

  affineKey key;
  initKey(&key);
  scanKey(&key, atoll(*(argv+1)),atoll(*(argv+2)),atoll(*(argv+3)));
  eea(&key);
  neutralAdditive(&key);
  printKey(&key);
  return 0;
}


//Falta combrobar gcd(a,n) = 1, en caso que no se pone a_i=-1
//Falta comprobar 0<B<=n, en caso que no se pone B_i = -1
