//Definición de funciones y constantes usadas para cifrar y decifrar con RSA usando OPENSSL

#define RSA_KEY_BITS 2048
#define MAX_PLAINTEXT_SIZE 214 //OAEP (256 - 42 BYTES)
#define CIPHERTEXT_SIZE (RSA_KEY_BITS / 8)

void checkCLI(int , char );
RSA* load_rsa_key(const char*, int);
void print_key(RSA*);
int rsa_encrypt_oaep(const char*, const char *);
int rsa_decrypt_oaep(const char*, const char *);
int endec(int, const unsigned char*, const unsigned char*);

#include "rsa.c"
