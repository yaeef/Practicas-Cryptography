//Declaracion de las funciones definidas

//Función que verifica argumentos CLI
void checkCLI(int argc)
{
  if(argc != 4)
  {
    printf("\n\n\t.:Error en numero de argumentos CLI:.\n\n");
    exit(1);
  }
}

//Función para inicializar la estructura affineKey
int initKey(affineKey *key)
{
  if(key != NULL)
  {
    key->a = 0;
    key->a_i = 0;
    key->B = 0;
    key->B_i = 0;
    key->n=0;
    return TRUE;
  }
  return FALSE;
}


//Función para leer parametros de afinneKey desde CLI
int scanKey(affineKey *key, long long a, long long B, long long n)
{
  if(key != NULL)
  {
    key->a = a;
    key->B = B;
    key->n = n;
    return TRUE;
  }
  return FALSE;
}

//Función para obtener a_i| Se recibe como entrada a,n

int eea(affineKey *key)
{

  long long t=0, newt=1;
  long long r=key->n, newr=key->a;

  while(newr != 0)
  {
    long long q = r/newr;
    long long temp = newt;
    newt = t-q*newt;
    t = temp;
    temp = newr;
    newr = r-q*newr;
    r = temp;
  }

  if(r != 1) //mcd(a,n) != 1 -> No existe inverso para todo elemento en Zn
  {
    key->a_i = -1;
    return FALSE;  
  }
  if(t < 0)
    t += key->n;
  key->a_i = t;
  return TRUE;
  
}

int neutralAdditive(affineKey *key)
{
  if(!(0 <= key->B && key->B < key->n)) //B no pertenece a [0,n)
  {
    key->B_i = -1;
    return FALSE;
  }
  else 
  {
    key->B_i = key->n - key->B;
    while(key->B_i < 0)
    {
      key->B_i += key->n;
    }
    return TRUE;
  }
}

void printKey(affineKey *key)
{
  //printf("\n\ta=[%lld]", key->a);
  //printf("\n\ta_i=[%lld]", key->a_i);
  //printf("\n\tB=[%lld]", key->B);
  //printf("\n\tB_i=[%lld]", key->B_i);
  //printf("\n\tn=[%lld]\n\n", key->n);
  printf("%lld %lld", key->a_i, key->B_i);
}
