#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include "rsa.h"

int main(int argc, char **argv)
{
  int opt = getopt(argc, argv, "e:d:");   //Leer flag
  checkCLI(argc, opt);
  const unsigned char *input_txt = *(argv+2);
  const unsigned char *key = *(argv+3);
  if(endec(opt, input_txt, key) != 0)
  {
    fprintf(stderr, "Error: La operación de cifrado/decifrado no se llevo a cabo. :()\n");
    return 1;
  }

  return 0;
}
