// Nom:
// NIU:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCONF 60

typedef struct configuracio{
  char posicio[6];
  unsigned profunditat;
} configuracio;

int main(){
  configuracio configuracions[MAXCONF];
  unsigned i;

  strcpy(configuracions[0].posicio, "DD_EE");
  configuracions[0].profunditat = 0;

  printf("Configuració inicial: %s\n",configuracions[0].posicio);

  unsigned numconfactual = 0, numconf = 1;
  char configuracio_actual[6];
  while(numconfactual < numconf && numconf < MAXCONF){
    trcpy(configuracio_actual, configuracions[numconfactual].posicio);
    for(i = 0; i < 4; i++){
      // Situació 1: canvi "D_" per "_D"
      if(configuracio_actual[i] == 'D' && configuracio_actual[i+1] == '_'){
        strcpy(configuracions[numconf].posicio,configuracio_actual);
        configuracions[numconf].posicio[i] = configuracio_actual[i+1];
        configuracions[numconf].posicio[i+1] = configuracio_actual[i];
        configuracions[numconf].profunditat = configuracions[numconfactual].profunditat + 1;
        printf("%s -(%u,%u)-> %s (profunditat = %u)\n",configuracio_actual, i, i+1,  configuracions[numconf].posicio, configuracions[numconf].profunditat);
        numconf ++;
      }
    }
    numconfactual ++;
  }
  if (numconf == MAXCONF){
    printf("Hem assolit el màxim de configuracions (%d) i pot ser no hem vist totes les possibilitats.", MAXCONF);
  }
  printf("\n");
  return 0;
}
