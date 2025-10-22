#define FIRST_CHAR 4 

//Declaración de extras para Ceasar Cipher

//Función para comprobar los argumentos CLI
void checkCLI(int argc, char flag)
{
  if(argc != 5)
  {
    printf("\n\n\t.:Error en numero de argumentos CLI:.\n\n");
    exit(1);
  }
  if(flag == '?')
  {
    printf("\n\n\t.:Error en flag seleccionada [e,d]:.\n\n");
  }
}

//Función para obtener el tamaño del archivo
long getFileSize(char *file)
{
  FILE *f = fopen(file, "r");
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fclose(f);
  return size;
}

//Función para crear una cadena de forma dinámica
char* createString(long n)
{
  char *s = (char*)malloc(n*sizeof(char));
  if(s == NULL)
  {
    printf("\n\n\t.:Memoria no encontrada:.\n\n");
    exit(1);
  }
  return s;
}

//Función para liberar memoria dinámica
void deleteString(char *s)
{
  free(s);
}

//Función para vaciar el archivo txt en memoria RAM
void scanFile(char *RAM, long size, char *fileName)
{
  FILE *myFile = fopen(fileName, "r");
  fread(RAM, 1, size, myFile);
  *(RAM + size) = '\0';  //Se agrega caracter de fin de cadena
  fclose(myFile);
}

//Función que hace el corrrimiento de caracteres
void affineCipherE(char *fileRAM, int a, int B)
{
  int i=0;
  char c;
  while(*(fileRAM + i) != '\0')
  {
    c = *(fileRAM +i);
    if(65 <= c && c <= 90)   //MAYUSCULAS
    {
      c -= 65;
      c = (a*c+B) % 26;
      c += 65;
      *(fileRAM + i) = c;
    }
    if(97 <= c && c <= 122)  //Minusculas
    {
      c -= 97;
      c = (a*c+B) % 26;
      c += 97;
      *(fileRAM + i) = c;
    }
    i++;
  }
}
void affineCipherD(char *fileRAM, int a, int B)
{
  int i=0;
  char c;
  while(*(fileRAM + i) != '\0')
  {
    c = *(fileRAM + i);
    if(65 <= c && c <= 90)
    {
      c -= 65;
      c = a*(c+B) % 26;
      c += 65;
      *(fileRAM + i) = c;
    }
    if(97 <= c && c <= 122)
    {
      c -= 97;
      c = a*(c+B) % 26;
      c += 97;
      *(fileRAM + i) = c;
    }
    i++;
  }
}

//Función que evalua en GCD de dos números
int checkGCD(int a, int n)
{
  int result = (a<n) ? a: n;
  while(result > 0)
  {
    if(a % result == 0 && n % result == 0)
      break;
    result--;
  }
  return result;
}
//a=6; n = 9; result=6 
//            result=5
//            result=4
//            result=3

//Función que encuentra el inverso aditivo en aritmética modular
int scanInverseAlpha(int a)
{
  switch (a) 
  {
    case 1: return 1;
    case 3: return 9;
    case 5: return 21;
    case 7: return 15;
    case 9: return 3;
    case 11: return 19;
    case 15: return 7;
    case 17: return 23;
    case 19: return 11;
    case 21: return 5;
    case 23: return 17;
    case 25: return 25;
    default: return -1;
  } 
}
int scanInverseBeta(int b)
{
  return 26 - b;
}

//Función que crea el nuevo nombre del archivo
char *createNewName(char *originalName, int opt) //Falta evitar desborde
{
  int i=0;

  char *newName = createString(256);
  while(*(originalName + i) != '.')
  {
    *(newName +i ) = *(originalName + i);
    i++;
  }
  if(opt == 'd') //Decifrar
  {
    *(newName + i) = '_';
    *(newName + i + 1) = 'd';
    *(newName + i + 2) = '.';
    *(newName + i + 3) = 't';
    *(newName + i + 4) = 'x';
    *(newName + i + 5) = 't';
    *(newName + i + 6) = '\0';
  }
  if(opt == 'e') //Cifrar
  {
    *(newName + i) = '_';
    *(newName + i + 1) = 'e';
    *(newName + i + 2) = '.';
    *(newName + i + 3) = 't';
    *(newName + i + 4) = 'x';
    *(newName + i + 5) = 't';
    *(newName + i + 6) = '\0';
  }
  return newName;
}

//Función que hace el corrimiento de B o  (-B) a cada caracter
void endec(char *fileRAM, int opt, int A, int B)
{
  if(opt == 'e') //Cifrado
  {
    if(checkGCD(A,26) != 1)
      exit(1);
    if(B < 0 || B > 25)
      exit(1);
    affineCipherE(fileRAM, A, B);
  }
  else           //Decifrado
    affineCipherD(fileRAM, scanInverseAlpha(A), scanInverseBeta(B));
}

//Función que crea el nuevo archivo y vacia el contenido de la ram
void createNewFile(char *RAM, char *originalName, int opt, long fileSize)
{
  char *newName = createNewName(originalName, opt);
  FILE *newFile = fopen(newName, "w");
  fwrite(RAM, 1, fileSize, newFile);
  fclose(newFile);
  deleteString(newName);
}


//ac + b mod 26 ~ p
//ac mod 26 ~ (p-b)
//Aac mod 26 ~ A(p-b)
//c mod 26 ~ A(p-b)
//A(p-b) mod 26 ~ c
