//Definición de funciones usadas para la práctica del 
//algoritmo extendido de euclides

#define TRUE 1
#define FALSE 0

typedef struct
{
  long long a;
  long long a_i;
  long long B;
  long long B_i;
  long long n;
}affineKey;


void checkCLI(int );
int initKey(affineKey* );
int scanKey(affineKey*, long long , long long , long long );
int eea(affineKey*);
int neutralAdditive(affineKey*);
void printKey(affineKey*);



#include "eea.c"
