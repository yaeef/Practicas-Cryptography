//BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

//CONSTANTES
#define RSA_KEY_BITS 2048
#define PUBLIC_EXPONENT_VAL 65537

//FUNCIÓN KEYGEN RSA
int key_gen_rsa(int bits, int exponent, const char *priv_key_archive, const char *pub_key_archive)
{
  RSA *rsa = NULL;
  BIGNUM *e = NULL;
  FILE *fp_priv = NULL;
  FILE *fp_pub = NULL;
  int ret = -1;

  //GENERAR EXPONENTE PÚBLICO
  e = BN_new();
  if(!e || !BN_set_word(e, exponent))
  {
    fprintf(stderr, "Error: Generar exponente publico.\n");
    goto fin;
  }

  //CREAR STRUCT RSA
  rsa = RSA_new();
  if(!rsa)
  {
    fprintf(stderr, "Error: Crear estructura de tipo RSA.\n");
    goto fin;
  }

  //GENERAR LLAVES
  if(!RSA_generate_key_ex(rsa, bits, e, NULL))
  {
    fprintf(stderr, "Error: Generar llaves RSA.\n");
    goto fin;
  }
  //PRIVADA
  fp_priv = fopen(priv_key_archive,"wb");
  if(!priv_key_archive)
  {
    fprintf(stderr, "Error: Crear archivo de salida de llave privada.\n");
    goto fin;
  }
  if(!PEM_write_RSAPrivateKey(fp_priv, rsa, NULL, NULL, 0, NULL, NULL))
  {
    fprintf(stderr, "Error: Escribir llave privada.\n");
    goto fin;
  }
  //PÚBLICA
  fp_pub = fopen(pub_key_archive, "wb");
  if(!pub_key_archive)
  {
    fprintf(stderr, "Error: Crear archivo de salida de llave publica.\n");
    goto fin;
  }
  if(!PEM_write_RSA_PUBKEY(fp_pub, rsa))
  {
    fprintf(stderr, "Error: Escribir llave publica.\n");
    goto fin;
  }
  //EXITO
  ret = 0;
  
  //LIBERACIÓN DE RECURSOS OPENSSL
  fin:
    if(fp_priv) fclose(fp_priv);
    if(fp_pub) fclose(fp_pub);
    if(rsa) RSA_free(rsa);
    if(e) BN_free(e);
    return ret;

}

int main(int argc, char **argv)
{
  if(key_gen_rsa(RSA_KEY_BITS, PUBLIC_EXPONENT_VAL, "private_key.pem", "public_key.pem") != 0)
  {
    fprintf(stderr, "Error: Generar llaves. \n");
    return 1;
  }
  return 0;
}

