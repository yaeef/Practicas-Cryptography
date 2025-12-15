//Declaracion de las funciones definidas en el archivo de cabezera
//Para la selección de cifrado y decifrado RSA

//Función que verifica argumentos CLI
void checkCLI(int argc, char flag)       //./compilado -flag input.txt key.pem
{
  if(argc != 4)        
  {
    printf("\n\n\t.:Error en numero de argumentos CLI:.\n\n");
    exit(1);
  }
  if(flag == '?')
  {
    printf("\n\n\t.:Error en flag seleccionada [e,d]:.\n\n");
    exit(1);
  }
}
//Función que carga una llave rsa mediante su path
RSA* load_rsa_key(const char* key_archive, int is_public)
{
  FILE *fp = fopen(key_archive, "rb");
  RSA *rsa = NULL;

  if(!fp)
  {
    fprintf(stderr, "Error: Cargar llave\n");
    return NULL;
  }

  if(is_public)
  {
    rsa = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
    if(!rsa)
    {
      rewind(fp);
      rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    }
  }
  else 
  {
    rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
  }

  fclose(fp);
  return rsa;
}
//Función auxiliar para verificar la correcta carga de llaves
void print_key(RSA *rsa)
{
  BIO *out = BIO_new_fp(stdout, BIO_NOCLOSE);
  RSA_print(out, rsa, 0);
  BIO_free(out);
}
//Función de cifrado RSA con padding OAEP
int rsa_encrypt_oaep(const char *input_txt, const char *public_key)
{
  RSA *rsa = load_rsa_key(public_key, 1);
  if(!rsa) return -1;
  
  FILE *fin = NULL;
  FILE *fout = NULL;
  long file_size;

  unsigned char *in_buf = NULL;
  unsigned char *out_buf = NULL;
  int bytes_cifrados = -1;

  //Obtener tamaño del texto a CIFRAR
  fin = fopen(input_txt, "rb");
  if(!fin)
  {
    fprintf(stderr, "Error: Abrir archivo de entrada para su cifrado\n");
    goto fin;
  }
  fseek(fin, 0, SEEK_END);
  file_size = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  //Verificar tamaño
  if(file_size > MAX_PLAINTEXT_SIZE)
  {
    fprintf(stderr, "Error: Archivo de entrada excede el limite de tamaño.\n");
    goto fin;
  }

  //Buffers de cifrado
  in_buf = (unsigned char *)malloc(file_size);
  out_buf = (unsigned char *)malloc(CIPHERTEXT_SIZE);
  if(!in_buf || !out_buf)
  {
    fprintf(stderr, "Error: crear buffers.\n");
    goto fin;
  }
  if(fread(in_buf, 1, file_size, fin) != file_size)
  {
    fprintf(stderr, "Error: Leer archivo de entrada\n");
    goto fin;
  }
  //cifrado
  bytes_cifrados = RSA_public_encrypt((int)file_size, in_buf, out_buf, rsa, RSA_PKCS1_OAEP_PADDING);
  if(bytes_cifrados < 0)
  {
    fprintf(stderr, "Error: Cifrar bytes con RSA OAEP\n");
    goto fin;
  }

  //escritura CIPHERTEXT
  fout = fopen("ciphertext.enc", "wb");
  if(!fout)
  {
    fprintf(stderr, "Error: Escribir ciphertext.\n");
    goto fin;
  }
  fwrite(out_buf, 1, bytes_cifrados, fout);
  
  fin:
    if(in_buf) free(in_buf);
    if(out_buf) free(out_buf);
    if(fin) fclose(fin);
    if(fout) fclose(fout);
    if(rsa) RSA_free(rsa);
    return bytes_cifrados > 0 ? 0 : -1;
}

//Función de decifrado
int rsa_decrypt_oaep(const char *input_txt, const char *private_key)
{
  RSA *rsa = load_rsa_key(private_key, 0);
  if(!rsa) return -1;
  
  FILE *fin = NULL;
  FILE *fout = NULL;
  long file_size;

  unsigned char *in_buf = NULL;
  unsigned char *out_buf = NULL;
  int bytes_decifrados = -1;

  //Obtener tamaño del texto a DECIFRAR
  fin = fopen(input_txt, "rb");
  if(!fin)
  {
    fprintf(stderr, "Error: Abrir archivo de entrada para su decifrado\n");
    goto fin;
  }
  fseek(fin, 0, SEEK_END);
  file_size = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  //Verificar tamaño
  if(file_size != CIPHERTEXT_SIZE)
  {
    fprintf(stderr, "Error: Archivo de entrada para decifrar no cumple con el tamaño especifico.\n");
    goto fin;
  }

  //Buffers de decifrado
  out_buf = (unsigned char *)malloc(MAX_PLAINTEXT_SIZE);
  in_buf = (unsigned char *)malloc(CIPHERTEXT_SIZE);
  if(!in_buf || !out_buf)
  {
    fprintf(stderr, "Error: crear buffers.\n");
    goto fin;
  }

  //decifrado
  if(fread(in_buf, 1, file_size, fin) != file_size)
  {
    fprintf(stderr, "Error: Leer CIPHERTEXT.\n");
    goto fin;
  }
  bytes_decifrados = RSA_private_decrypt((int)file_size, in_buf, out_buf, rsa, RSA_PKCS1_OAEP_PADDING);
  if(bytes_decifrados < 0)
  {
    fprintf(stderr, "Error: Decifrar bytes con RSA OAEP\n");
    goto fin;
  }

  //escritura PLAINTEXT
  fout = fopen("ciphertext_decrypted.txt", "wb");
  if(!fout)
  {
    fprintf(stderr, "Error: Crear ciphertext_encrypted.txt.\n");
    goto fin;
  }
  fwrite(out_buf, 1, bytes_decifrados, fout);
  
  fin:
    if(in_buf) free(in_buf);
    if(out_buf) free(out_buf);
    if(fin) fclose(fin);
    if(fout) fclose(fout);
    if(rsa) RSA_free(rsa);
    return bytes_decifrados > 0 ? 0 : -1;
}

//Función que orquesta el cifrado o decifrado
int endec(int opt, const unsigned char *input_txt, const unsigned char *key)
{
  if(opt == 101) //CIFRAR
  {
    return rsa_encrypt_oaep(input_txt, key);
  }
  else
  {
    return rsa_decrypt_oaep(input_txt, key);
  }
}
