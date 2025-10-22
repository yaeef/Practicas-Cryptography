#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include "bmpImage_block.h" 

int main(int argc, char **argv)
{
    int opt = getopt(argc, argv, "e:d:");
    checkCLI(argc, opt);
    int type = atoi(*(argv+3)); // 0=ECB, 1=CBC
    unsigned char *key = *(argv+4);
    unsigned char *iv = *(argv+5);

    BITMAPFILEHEADER headerFile;
    BITMAPINFOHEADERV3X headerInfo;

    //Cifrado
    if(opt == 'e')
    {
        scanBMFH(&headerFile, optarg);
        scanBMIH(&headerInfo, optarg);
        checkBMP(headerInfo, headerFile);
        int padding = ((headerInfo.width*3+3)/4)*4; // stride
        unsigned char **M = createMatrix(headerInfo.height, padding);
        scanColors(M, headerInfo.height, headerFile.dataOffset, padding, optarg);

        //Linealizar M
        long original_size;
        unsigned char *flat_data = linearize_matrix(M, headerInfo.height, padding, &original_size);
        
        // Cifrar el búfer 1D
        unsigned char *encrypted_data = NULL;
        int encrypted_len = 0;

        if(type == 0) // ECB
            encrypt_ecb(flat_data, original_size, &encrypted_data, &encrypted_len, key);
        else if(type == 1) // CBC
            encrypt_cbc(flat_data, original_size, &encrypted_data, &encrypted_len, key, iv);
        
        //Actualizar headers con el nuevo tamaño
        headerFile.fileSize = 54 + encrypted_len;
        headerInfo.imageSize = encrypted_len;

        // Escribir el nuevo BMP (headers + búfer 1D cifrado)
        createNewBMP_block(headerFile, headerInfo, encrypted_data, encrypted_len, optarg, opt, type);

        destroyMatrix(M, headerInfo.height, padding);
        free(flat_data);
        free(encrypted_data);
    }
    else if(opt == 'd')
    {
        scanBMFH(&headerFile, optarg);
        scanBMIH(&headerInfo, optarg);
        checkBMP(headerInfo, headerFile);
        int padding = ((headerInfo.width*3+3)/4)*4; 

        //Leer el búfer 1D cifrado del archivo
        unsigned char *encrypted_data = readEncryptedData(headerInfo, headerFile.dataOffset, optarg);
        
        //Descifrar el búfer 1D
        unsigned char *decrypted_data = NULL;
        long decrypted_size = 0;
        
        if(type == 0) // ECB
            decrypt_ecb(encrypted_data, headerInfo.imageSize, &decrypted_data, &decrypted_size, key);
        else if(type == 1) // CBC
            decrypt_cbc(encrypted_data, headerInfo.imageSize, &decrypted_data, &decrypted_size, key, iv);

        //Actualizar headers al tamaño original
        headerFile.fileSize = 54 + decrypted_size;
        headerInfo.imageSize = decrypted_size;

        //Crear Matriz M y delinealizar
        unsigned char **M = createMatrix(headerInfo.height, padding);
        delinearize_buffer(decrypted_data, headerInfo.height, padding, M);

        //Escribir el BMP descifrado (¡usando tu función original!)
        createNewBMP(M, headerFile, headerInfo, padding, optarg, opt, type);
      
        destroyMatrix(M, headerInfo.height, padding);
        free(encrypted_data);
        free(decrypted_data);
    }
    
    return 0;
}
