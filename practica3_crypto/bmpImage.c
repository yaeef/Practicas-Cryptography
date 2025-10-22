//Declaracion de las funciones definidas en el archivo de cabezera
//Para el procesamiento de una imagen bmp de 24 bits 

//Función que verifica argumentos CLI
void checkCLI(int argc, char flag)       //./compilado -flag imagen.bmp tipo, key, iv
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


//Funcion para lectura del BITMAPFILEHEADER, i.e, los primeros 14 bytes del header
int scanBMFH(BITMAPFILEHEADER *header, char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");

    if(bmpImage == NULL)
    {
        printf("\n\n\t.:Error en apertura de archivo. Funcion->[scanBMFH()]\n\n");
        return 1;
    }
    fseek(bmpImage, 0, SEEK_SET);                          
    fread(&header->signature, sizeof(char), 2, bmpImage);   
    fread(&header->fileSize, sizeof(int), 1, bmpImage);
    fread(&header->reserved, sizeof(int), 1, bmpImage);
    fread(&header->dataOffset, sizeof(int), 1, bmpImage);      
    fclose(bmpImage);
    return 0;
}
//Funcion para lectura del BITMAPINFOHEADER, i.e, los 40 bytes restantes del header. Uso exclusivo para BMP 3.X estandar
int scanBMIH(BITMAPINFOHEADERV3X *header, char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");

    if(bmpImage == NULL)
    {
        printf("\n\n\t.:Error en apertura de archivo. Funcion->[scanBMIH()]\n\n");
        return 1;
    }

    fseek(bmpImage, 14, SEEK_SET);     
    fread(&header->sizeHeader, sizeof(int), 1, bmpImage);     //LECTURA DE BITMAPINFOHEADER | 40 BYTES
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
//Funcion para verificar que la imagen puede ser procesada.
void checkBMP(BITMAPINFOHEADERV3X headerInfo, BITMAPFILEHEADER headerFile)
{

    if((headerFile.signature[0] != 66) || (headerFile.signature[1] != 77))
    {
        printf("\n\n\t.:Error de archivo. Error->[Firma incorrecta]");
        exit(1);
    }
    if(headerFile.dataOffset != 54)
    {
        printf("\n\n\t.:Error de archivo. Error->[Header incompatible]");
        exit(1);
    }
    if(headerInfo.sizeHeader != 40)
    {
        printf("\n\n\t.:Error de archivo. Error->[Header incompatible]");
        exit(1);
    }
    if(headerInfo.bitCount != 24)
    {
        printf("\n\n\t.:Error de archivo. Error->[La imagen debe de ser de 24 bits]");
        exit(1);
    }
    if(headerInfo.compression != 0)
    {
        printf("\n\n\t.:Error de archivo. Error->[Imagen debe ser sin compresion]");
        exit(1);
    }
}

/*Funcion para crear la matriz dinamica para almacenar los bytes de color*/
unsigned char **createMatrix(int height, int width)
{
    unsigned char **M;
    M = (unsigned char**)malloc(height*sizeof(unsigned char*));
    if(M == NULL)
    {
        printf("\n\n\t.:Error de memoria. Error->[No se pudo crear la matriz]");
        exit(1);
    }
    for(int i=0; i<height; i++)
    {
        *(M + i) = (unsigned char*)calloc(width, sizeof(unsigned char));
        if(*(M + i) == NULL)
        {
            printf("\n\n\t.:Error de memoria. Error->[No se pudo crear la matriz]");
            exit(1);
        }
    }
    return M;
}
//Funcion para liberar memoria usada de forma dinamica por la matriz de colores
void destroyMatrix(unsigned char **M, int height, int width)
{
    for(int i=0; i<height; i++)
        free(*(M + i));
    free(M);
}
//Funcion para obtener los valores rgb de la imagen bmp
void scanColors(unsigned char **M, int height, int dataOffset, int padded ,char *imageName)
{
    FILE *bmpImage = fopen(imageName, "rb");

    if(bmpImage == NULL)
    {
        printf("\n\n\t.:Error en apertura de archivo. Funcion->[scanColors()]\n\n");
        exit(1);
    }
    fseek(bmpImage, dataOffset, SEEK_SET);
    for(int i=0; i < height; i++)
    {
        fread(*(M+i), padded, 1, bmpImage);
    }
    fclose(bmpImage);
}

//Funciones de cifrado y decifrado para CFB de 128 bits
void cifrar_matriz_cfb(unsigned char **M, int height, int stride, unsigned char *key, unsigned char * iv)
{
  EVP_CIPHER_CTX *ctx;
  unsigned char *temp_buffer = (unsigned char*)malloc(stride*sizeof(unsigned char));
  int len;

  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_128_cfb(), NULL, key, iv);

  for(int i=0; i< height; i++)
  {
    EVP_EncryptUpdate(ctx, temp_buffer, &len, M[i], stride);
    memcpy(M[i], temp_buffer, len);
  }

  EVP_EncryptFinal_ex(ctx, temp_buffer, &len);
  EVP_CIPHER_CTX_free(ctx);
  free(temp_buffer);
}

void decifrar_matriz_cfb(unsigned char **M, int height, int stride, unsigned char *key, unsigned char *iv)
{
  EVP_CIPHER_CTX *ctx;
  unsigned char *temp_buffer = (unsigned char*)malloc(stride*sizeof(unsigned char));
  int len;
   ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_128_cfb(), NULL, key, iv);

  for(int i=0; i<height; i++)
  {
    EVP_DecryptUpdate(ctx, temp_buffer, &len, M[i], stride);
    memcpy(M[i], temp_buffer, len);
  }

  EVP_DecryptFinal_ex(ctx, temp_buffer, &len);
  EVP_CIPHER_CTX_free(ctx);
  free(temp_buffer);
}

//Funciones de cifrado y decifrado para CFB de 128 bits
void cifrar_matriz_ofb(unsigned char **M, int height, int stride, unsigned char *key, unsigned char * iv)
{
  EVP_CIPHER_CTX *ctx;
  unsigned char *temp_buffer = (unsigned char*)malloc(stride*sizeof(unsigned char));
  int len;

  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_128_ofb(), NULL, key, iv);

  for(int i=0; i< height; i++)
  {
    EVP_EncryptUpdate(ctx, temp_buffer, &len, M[i], stride);
    memcpy(M[i], temp_buffer, len);
  }

  EVP_EncryptFinal_ex(ctx, temp_buffer, &len);
  EVP_CIPHER_CTX_free(ctx);
  free(temp_buffer);
}

void decifrar_matriz_ofb(unsigned char **M, int height, int stride, unsigned char *key, unsigned char *iv)
{
  EVP_CIPHER_CTX *ctx;
  unsigned char *temp_buffer = (unsigned char*)malloc(stride*sizeof(unsigned char));
  int len;
   ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_128_ofb(), NULL, key, iv);

  for(int i=0; i<height; i++)
  {
    EVP_DecryptUpdate(ctx, temp_buffer, &len, M[i], stride);
    memcpy(M[i], temp_buffer, len);
  }

  EVP_DecryptFinal_ex(ctx, temp_buffer, &len);
  EVP_CIPHER_CTX_free(ctx);
  free(temp_buffer);
}

void endec(unsigned char **M, BITMAPFILEHEADER fileHeader, BITMAPINFOHEADERV3X infoHeader, unsigned char *key, unsigned char *iv, int opt, int type)
{
  if(type == 2) //CFB
  {
    int stride = ((infoHeader.width*3+3)/4)*4;
    if(opt == 'e')
      cifrar_matriz_cfb(M, infoHeader.height, stride, key, iv);
    else
      decifrar_matriz_cfb(M, infoHeader.height, stride, key, iv);
  }
  if(type == 3) //OFB
  {
    int stride = ((infoHeader.width*3+3)/4)*4;
    if(opt == 'e')
      cifrar_matriz_ofb(M, infoHeader.height, stride, key, iv);
    else
      decifrar_matriz_ofb(M, infoHeader.height, stride, key, iv);
  }
}

//Funcion que ngenera el nuevo nombre der archivo procesado
char* createNewName(char *originalName, int opt, int type)
{
    int i=0;
    char *newName = (char*)malloc(sizeof(char)*MAXLENSTRING);
    if(newName == NULL)
    {
        printf("\n\n\t.:Error en creacion de nombre nuevo. Funcion->[createNewName()]\n\n");
        exit(1);
    }
    while(*(originalName +i) != '.')
    {
      *(newName + i) = *(originalName + i);
      i++;
    }
    if(opt == 'd')   //Nuevo nombre para decifrado
    {
        newName[i] = '_';
        newName[i+1] = 'd';
        if(type == 0)
        {
          newName[i+2] = 'E';
          newName[i+3] = 'C';
          newName[i+4] = 'B';
        }
        else if(type ==1)
        {
          newName[i+2] = 'C';
          newName[i+3] = 'B';
          newName[i+4] = 'C';
        }
        else if(type ==2)
        {
          newName[i+2] = 'C';
          newName[i+3] = 'F';
          newName[i+4] = 'B';
        }
        else
        {
          newName[i+2] = 'O';
          newName[i+3] = 'F';
          newName[i+4] = 'B';
        }
        
        newName[i+5] = '.';
        newName[i+6] = 'b';
        newName[i+7] = 'm';
        newName[i+8] = 'p';
        newName[i+9] = '\0';
    }
    else         //Nuevo nombre para cifrado 
    {
        newName[i] = '_';
        newName[i+1] = 'e';
        if(type == 0)
        {
          newName[i+2] = 'E';
          newName[i+3] = 'C';
          newName[i+4] = 'B';
        }
        else if(type ==1)
        {
          newName[i+2] = 'C';
          newName[i+3] = 'B';
          newName[i+4] = 'C';
        }
        else if(type ==2)
        {
          newName[i+2] = 'C';
          newName[i+3] = 'F';
          newName[i+4] = 'B';
        }
        else
        {
          newName[i+2] = 'O';
          newName[i+3] = 'F';
          newName[i+4] = 'B';
        }
        newName[i+5] = '.';
        newName[i+6] = 'b';
        newName[i+7] = 'm';
        newName[i+8] = 'p';
        newName[i+9] = '\0';
    }
    return newName;
}

//Función que escribe la nueva imagen, tanto el header como la matriz de colores
void createNewBMP(unsigned char **M, BITMAPFILEHEADER headerFile, BITMAPINFOHEADERV3X headerInfo, int padding, char *originalName, int opt, int type)
{
  char *newName = createNewName(originalName, opt, type);
  FILE *newImage = fopen(newName, "wb"); //Falta comprobación de creación del archivo
  fwrite(&headerFile, sizeof(BITMAPFILEHEADER), 1, newImage);
  fwrite(&headerInfo, sizeof(BITMAPINFOHEADERV3X), 1, newImage);
  for(int i=0; i<headerInfo.height; i++)
    fwrite(M[i], padding, 1, newImage);
  fclose(newImage);
  free(newName);
}
