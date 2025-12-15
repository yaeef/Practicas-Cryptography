#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include "bmpImage.h"

int main(int argc, char **argv)
{
  int opt = getopt(argc, argv, "e:d:");   //Leer flag
  checkCLI(argc, opt);
  int type = atoi(*(argv+3));
  unsigned char *key = *(argv+4);
  unsigned char *iv = *(arv+5);

  BITMAPFILEHEADER headerFile;
  BITMAPINFOHEADERV3X headerInfo;

  scanBMFH(&headerFile, optarg);                  //Lectura primeros 14 bytes del header
  scanBMIH(&headerInfo, optarg);                  //Lectura de los 40 bytes restantes
  checkBMP(headerInfo, headerFile);               //Verificación de estructura de la imagen
  int padding = ((headerInfo.width*3+3)/4)*4;     //Alineación a 4 bytes 
  unsigned char **M = createMatrix(headerInfo.height, padding);     //Creación de matriz de colores
  scanColors(M, headerInfo.height, headerFile.dataOffset, padding, optarg);      //Lectura de bytes de color
  endec(M, headerFile, headerInfo, key, iv, opt, type);
  createNewBMP(M, headerFile, headerInfo, padding, optarg, opt, type);
  destroyMatrix(M, headerInfo.height, padding);
  return 0;
}
