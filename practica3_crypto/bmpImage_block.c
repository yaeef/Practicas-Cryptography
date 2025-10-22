#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void checkCLI(int argc, char flag)
{
 if(argc != 6)
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
int scanBMFH(BITMAPFILEHEADER *header, char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");
    if(bmpImage == NULL) return 1;
    fseek(bmpImage, 0, SEEK_SET);
    fread(&header->signature, sizeof(char), 2, bmpImage);
    fread(&header->fileSize, sizeof(int), 1, bmpImage);
    fread(&header->reserved, sizeof(int), 1, bmpImage);
    fread(&header->dataOffset, sizeof(int), 1, bmpImage);
    fclose(bmpImage);
    return 0;
}
int scanBMIH(BITMAPINFOHEADERV3X *header, char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");
    if(bmpImage == NULL) return 1;
    fseek(bmpImage, 14, SEEK_SET);
    fread(&header->sizeHeader, sizeof(int), 1, bmpImage);
    fread(&header->width, sizeof(int), 1, bmpImage);
    fread(&header->height, sizeof(int), 1, bmpImage);
    fread(&header->planes, sizeof(short int), 1, bmpImage);
    fread(&header->bitCount, sizeof(short int), 1, bmpImage);
    fread(&header->compression, sizeof(int), 1, bmpImage);
    fread(&header->imageSize, sizeof(int), 1, bmpImage);
    fread(&header->Xppm, sizeof(int), 1, bmpImage);
    fread(&header->Yppm, sizeof(int), 1, bmpImage);
    fread(&header->colorsUsed, sizeof(int), 1, bmpImage);
    fread(&header->colorsImportant, sizeof(int), 1, bmpImage);
    fclose(bmpImage);
    return 0;
}
void checkBMP(BITMAPINFOHEADERV3X headerInfo, BITMAPFILEHEADER headerFile)
{
    if((headerFile.signature[0] != 66) || (headerFile.signature[1] != 77)) exit(1);
    if(headerFile.dataOffset != 54) exit(1);
    if(headerInfo.sizeHeader != 40) exit(1);
    if(headerInfo.bitCount != 24) exit(1);
    if(headerInfo.compression != 0) exit(1);
}
unsigned char **createMatrix(int height, int width)
{
    unsigned char **M;
    M = (unsigned char**)malloc(height*sizeof(unsigned char*));
    if(M == NULL) exit(1);
    for(int i=0; i<height; i++)
    {
        *(M + i) = (unsigned char*)calloc(width, sizeof(unsigned char));
        if(*(M + i) == NULL) exit(1);
    }
    return M;
}
void destroyMatrix(unsigned char **M, int height, int width)
{
    for(int i=0; i<height; i++)
        free(*(M + i));
    free(M);
}
void scanColors(unsigned char **M, int height, int dataOffset, int padded ,char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");
    if(bmpImage == NULL) exit(1);
    fseek(bmpImage, dataOffset, SEEK_SET);
    for(int i=0; i < height; i++)
    {
        fread(*(M+i), padded, 1, bmpImage);
    }
    fclose(bmpImage);
}
char* createNewName(char *originalName, int opt, int type)
{
    int i=0;
    char *newName = (char*)malloc(sizeof(char)*MAXLENSTRING);
    if(newName == NULL) exit(1);
    while(*(originalName +i) != '.')
    {
      *(newName + i) = *(originalName + i);
      i++;
    }
    if(opt == 'd')
    {
        newName[i] = '_'; newName[i+1] = 'd';
        if(type == 0){ newName[i+2] = 'E'; newName[i+3] = 'C'; newName[i+4] = 'B'; }
        else if(type ==1){ newName[i+2] = 'C'; newName[i+3] = 'B'; newName[i+4] = 'C'; }
        else if(type ==2){ newName[i+2] = 'C'; newName[i+3] = 'F'; newName[i+4] = 'B'; }
        else{ newName[i+2] = 'O'; newName[i+3] = 'F'; newName[i+4] = 'B'; }
        newName[i+5] = '.'; newName[i+6] = 'b'; newName[i+7] = 'm'; newName[i+8] = 'p'; newName[i+9] = '\0';
    }
    else
    {
        newName[i] = '_'; newName[i+1] = 'e';
        if(type == 0){ newName[i+2] = 'E'; newName[i+3] = 'C'; newName[i+4] = 'B'; }
        else if(type ==1){ newName[i+2] = 'C'; newName[i+3] = 'B'; newName[i+4] = 'C'; }
        else if(type ==2){ newName[i+2] = 'C'; newName[i+3] = 'F'; newName[i+4] = 'B'; }
        else{ newName[i+2] = 'O'; newName[i+3] = 'F'; newName[i+4] = 'B'; }
        newName[i+5] = '.'; newName[i+6] = 'b'; newName[i+7] = 'm'; newName[i+8] = 'p'; newName[i+9] = '\0';
    }
    return newName;
}
void createNewBMP(unsigned char **M, BITMAPFILEHEADER headerFile, BITMAPINFOHEADERV3X headerInfo, int padding, char *originalName, int opt, int type)
{
  char *newName = createNewName(originalName, opt, type);
  FILE *newImage = fopen(newName, "wb");
  fwrite(&headerFile, sizeof(BITMAPFILEHEADER), 1, newImage);
  fwrite(&headerInfo, sizeof(BITMAPINFOHEADERV3X), 1, newImage);
  for(int i=0; i<headerInfo.height; i++)
    fwrite(M[i], padding, 1, newImage);
  fclose(newImage);
  free(newName);
}


//Funciones para cifrado por bloque
unsigned char* linearize_matrix(unsigned char **M, int height, int stride, long *flat_size)
{
    *flat_size = (long)height * stride;
    unsigned char *flat_buffer = (unsigned char *)malloc(*flat_size);
    
    for (int i = 0; i < height; i++) {
        memcpy(flat_buffer + (i * stride), M[i], stride);
    }
    return flat_buffer;
}

void delinearize_buffer(unsigned char *flat_buffer, int height, int stride, unsigned char **M)
{
    for (int i = 0; i < height; i++) {
        memcpy(M[i], flat_buffer + (i * stride), stride);
    }
}

unsigned char* readEncryptedData(BITMAPINFOHEADERV3X headerInfo, int dataOffset, char* imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");
    fseek(bmpImage, dataOffset, SEEK_SET);
    
    // Leemos el tamaño de la imagen (cifrada) desde el header
    int encrypted_len = headerInfo.imageSize;
    unsigned char *encrypted_data = (unsigned char *)malloc(encrypted_len);

    fread(encrypted_data, 1, encrypted_len, bmpImage);
    fclose(bmpImage);
    return encrypted_data;
}

// Nueva función para escribir el búfer 1D cifrado
void createNewBMP_block(BITMAPFILEHEADER headerFile, BITMAPINFOHEADERV3X headerInfo, unsigned char* encrypted_data, int encrypted_len, char *originalName, int opt, int type)
{
    char *newName = createNewName(originalName, opt, type);
    FILE *newImage = fopen(newName, "wb");
    
    // Escribe los headers (que ya deben estar actualizados)
    fwrite(&headerFile, sizeof(BITMAPFILEHEADER), 1, newImage);
    fwrite(&headerInfo, sizeof(BITMAPINFOHEADERV3X), 1, newImage);
    
    // Escribe el búfer 1D cifrado de una sola vez
    fwrite(encrypted_data, 1, encrypted_len, newImage);
    
    fclose(newImage);
    free(newName);
}


// --- Funciones de Cifrado/Descifrado (ECB) ---
// Usamos AES-128 para ser consistentes con tus funciones CFB/OFB
void encrypt_ecb(unsigned char *plaintext, long plaintext_len, unsigned char **ciphertext, int *ciphertext_len, unsigned char *key)
{
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    *ciphertext_len = 0;

    *ciphertext = (unsigned char *)malloc(plaintext_len + EVP_MAX_BLOCK_LENGTH);
    ctx = EVP_CIPHER_CTX_new();

    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);
    EVP_EncryptUpdate(ctx, *ciphertext, &len, plaintext, plaintext_len);
    *ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, *ciphertext + len, &len);
    *ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
}

void decrypt_ecb(unsigned char *ciphertext, int ciphertext_len, unsigned char **plaintext, long *plaintext_len, unsigned char *key)
{
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    *plaintext_len = 0;

    *plaintext = (unsigned char *)malloc(ciphertext_len);
    ctx = EVP_CIPHER_CTX_new();
    
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);
    EVP_DecryptUpdate(ctx, *plaintext, &len, ciphertext, ciphertext_len);
    *plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, *plaintext + len, &len);
    *plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
}

// --- Funciones de Cifrado/Descifrado (CBC) ---
void encrypt_cbc(unsigned char *plaintext, long plaintext_len, unsigned char **ciphertext, int *ciphertext_len, unsigned char *key, unsigned char *iv)
{
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    *ciphertext_len = 0;

    *ciphertext = (unsigned char *)malloc(plaintext_len + EVP_MAX_BLOCK_LENGTH);
    ctx = EVP_CIPHER_CTX_new();

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, *ciphertext, &len, plaintext, plaintext_len);
    *ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, *ciphertext + len, &len);
    *ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
}

void decrypt_cbc(unsigned char *ciphertext, int ciphertext_len, unsigned char **plaintext, long *plaintext_len, unsigned char *key, unsigned char *iv)
{
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    *plaintext_len = 0;

    *plaintext = (unsigned char *)malloc(ciphertext_len);
    ctx = EVP_CIPHER_CTX_new();

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, *plaintext, &len, ciphertext, ciphertext_len);
    *plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, *plaintext + len, &len);
    *plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
}
