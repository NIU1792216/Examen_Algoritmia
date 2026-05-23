// Nom: Mario Roda Sevilla
// NIU: 1792216
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCONF 60

typedef struct configuracio{
    char posicio[6];
    unsigned profunditat;
} configuracio;

void canvi_immediat(char *conf_desti, char *conf_inicial, unsigned index_canvi);
void canvi_salt(char *conf_desti, char *conf_inicial, unsigned index_canvi);
void canvi(configuracio *conf_desti, configuracio *conf_inicial, unsigned index_canvi, unsigned tipus_canvi, unsigned *num_conf);

int main(){
    configuracio configuracions[MAXCONF];

    strcpy(configuracions[0].posicio, "DD_EE");
    configuracions[0].profunditat = 0;

    printf("Configuració inicial: %s\n",configuracions[0].posicio);

    unsigned numconfactual = 0, numconf = 1;
    char configuracio_actual[6];
    while(numconfactual < numconf && numconf < MAXCONF){
    strcpy(configuracio_actual, configuracions[numconfactual].posicio);
    for(unsigned i = 0; i < 4; i++){
        if((configuracio_actual[i] == 'D' && configuracio_actual[i+1] == '_') || (configuracio_actual[i+1] == 'E' && configuracio_actual[i]  == '_')){
            canvi(&configuracions[numconf], &configuracions[numconfactual], i, 1, &numconf);
        }
        if (i < 3){
            if ((configuracio_actual[i] == 'D' && configuracio_actual[i+1] == 'E' && configuracio_actual[i+2] == '_') || (configuracio_actual[i] == '_' && configuracio_actual[i+1] == 'D' && configuracio_actual[i+2] == 'E')){
                canvi(&configuracions[numconf], &configuracions[numconfactual], i, 2, &numconf);
            }
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

void canvi_immediat(char *conf_desti, char *conf_inicial, unsigned index_canvi){
    strcpy(conf_desti, conf_inicial);
    conf_desti[index_canvi] = conf_inicial[index_canvi+1];
    conf_desti[index_canvi+1] = conf_inicial[index_canvi];
}
void canvi_salt(char *conf_desti, char *conf_inicial, unsigned index_canvi){
    strcpy(conf_desti, conf_inicial);
    conf_desti[index_canvi] = conf_inicial[index_canvi+2];
    conf_desti[index_canvi+2] = conf_inicial[index_canvi];
}
void canvi(configuracio *conf_desti, configuracio *conf_inicial, unsigned index_canvi, unsigned tipus_canvi, unsigned *num_conf){
    unsigned index_lletra = index_canvi;
    unsigned index_ = index_canvi + tipus_canvi;
    if (tipus_canvi == 1){
        canvi_immediat(conf_desti->posicio, conf_inicial->posicio, index_canvi);
    }
    if (tipus_canvi == 2){
        canvi_salt(conf_desti->posicio, conf_inicial->posicio, index_canvi);
    }
    if (conf_inicial->posicio[index_canvi] == '_'){
        index_ = index_canvi;
        index_lletra = index_canvi + tipus_canvi;
    }
    conf_desti->profunditat = conf_inicial->profunditat + 1;
    printf("%s -(%u,%u)-> %s (profunditat = %u)\n",conf_inicial->posicio, index_lletra, index_,  conf_desti->posicio, conf_desti->profunditat);
    (*num_conf)++;
}