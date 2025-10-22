#include <openssl/evp.h> // Necesario para las nuevas funciones
#include <openssl/err.h>

//Definición de estructuras y funciones usadas para leer, almacenar y escribir
//en imagenes bmp de 24 bits, sin compresion y con un HEADER V3 Standard

#define MAXLENSTRING 256
#pragma pack(push, 1)
typedef struct
{
    char signature[2];
    int fileSize;
    int reserved;
    int dataOffset;
}BITMAPFILEHEADER;

typedef struct
{
    int sizeHeader;
    int width;
    int height;
    short int planes;
    short int bitCount;
    int compression;
    int imageSize;
    int Xppm;
    int Yppm;
    int colorsUsed;
    int colorsImportant;
}BITMAPINFOHEADERV3X;
#pragma pack(pop)


void checkCLI(int , char );
int scanBMFH(BITMAPFILEHEADER*, char*);
int scanBMIH(BITMAPINFOHEADERV3X*, char*);
void checkBMP(BITMAPINFOHEADERV3X, BITMAPFILEHEADER);
unsigned char **createMatrix(int, int);
void destroyMatrix(unsigned char**, int, int);
void scanColors(unsigned char**, int, int, int ,char*);
char* createNewName(char*, int, int);
void createNewBMP(unsigned char **, BITMAPFILEHEADER , BITMAPINFOHEADERV3X , int , char *, int, int );
unsigned char* linearize_matrix(unsigned char **M, int height, int stride, long *flat_size);
void delinearize_buffer(unsigned char *flat_buffer, int height, int stride, unsigned char **M);

// Ayudante para leer datos cifrados (1D) del archivo
unsigned char* readEncryptedData(BITMAPINFOHEADERV3X headerInfo, int dataOffset, char* imageName);

// Ayudante para escribir datos cifrados (1D) al archivo
void createNewBMP_block(BITMAPFILEHEADER headerFile, BITMAPINFOHEADERV3X headerInfo, unsigned char* encrypted_data, int encrypted_len, char *originalName, int opt, int type);

// Funciones de cifrado por bloque (ECB)
void encrypt_ecb(unsigned char *plaintext, long plaintext_len, unsigned char **ciphertext, int *ciphertext_len, unsigned char *key);
void decrypt_ecb(unsigned char *ciphertext, int ciphertext_len, unsigned char **plaintext, long *plaintext_len, unsigned char *key);

// Funciones de cifrado por bloque (CBC)
void encrypt_cbc(unsigned char *plaintext, long plaintext_len, unsigned char **ciphertext, int *ciphertext_len, unsigned char *key, unsigned char *iv);
void decrypt_cbc(unsigned char *ciphertext, int ciphertext_len, unsigned char **plaintext, long *plaintext_len, unsigned char *key, unsigned char *iv);

// (Incluir el .c)
#include "bmpImage_block.c"
