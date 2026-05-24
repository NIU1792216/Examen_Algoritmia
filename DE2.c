// Nom: Mario Roda Sevilla
// NIU: 1792216
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

#define MAXCONF 60
#define __cplusplus__strings__ 1

typedef struct configuracio{
    char *posicio;
    unsigned profunditat;
} configuracio;

char *reservar_config(unsigned mida_conf);
char *crear_cadena_uniforme(char *caracter, unsigned repeticions);
void canvi_immediat(char *conf_desti, char *conf_inicial, unsigned index_canvi);
void canvi_salt(char *conf_desti, char *conf_inicial, unsigned index_canvi);
void canvi(configuracio *conf_desti, configuracio *conf_inicial, unsigned index_canvi, unsigned tipus_canvi, unsigned *num_conf, unsigned num_posicions);

int main(int argc, char *argv[]){
    // Comprovem que el programa es cridi amb un argument
    if (argc != 2){
        fprintf(stderr,"ERROR: El programa s'ha de cridar amb un argument\n");
        exit(EXIT_FAILURE);
    }
    configuracio configuracions[MAXCONF];
    // Configuracio entrada
    char *conf_inicial=NULL;
    // Nombre de caracters de les configuracions
    unsigned mida_conf;
    // Punter per trobar els errors a l'entrada
    char *error=NULL;
    // Posem l'indicador de l'error a 0
    errno = 0;
    // Convertim els caracters a un nombre i comprovem que a la conversio no hi hagi errors
    int long n = strtol(argv[1], &error, 10);
    // Si no s'ha pogut llegir cap caracter en base decimal, comprovem si son lletres
    if (error == argv[1]){
        // Llegeixo la mida de la cadena introduida i la transformo en unsigned perque no farem configuracions amb mes de 4294967295 posicions
        mida_conf = (unsigned)strlen(argv[1]);
        // Comprovem que tots els caracters siguin D o E o _
        for (unsigned i=0; i<mida_conf; i++){
            if (argv[1][i] != 'D' && argv[1][i] != 'E' && argv[1][i] != '_'){
            }
        }
        // Reservem tant espai com per la cadena introduida (inclou el \0)
        conf_inicial = reservar_config(mida_conf);
        strcpy(conf_inicial, argv[1]);
    }
    else if (errno == ERANGE){
            perror("ERROR: Argument no valid o fora de rang\n");
            printf("Problema a %s\n", error);
            exit(EXIT_FAILURE);
    }
    // He posat un interval maxim per la n, ja que amb n massa gran el programa podria petar
    else if (n < 0 || n > 65){
        fprintf(stderr, "ERROR: L'entrada no pot ser un nombre negatiu ni major de 65");
        exit(EXIT_FAILURE);
    }
    // Si entra aqui es que han entrat un nombre dintre del rang posat
    else {
        mida_conf = (unsigned)(2*n + 1);
        reservar_config(mida_conf+1);
        char *cadena_d = crear_cadena_uniforme("D", n);
        char *cadena_e = crear_cadena_uniforme("E", n);
        // Copiem n Ds a la cadena conf_inicial, afegim un _ i concatenem n Es a la cadena
        conf_inicial = strcat(conf_inicial, cadena_d);
        strcat(conf_inicial, "_");
        conf_inicial = strcat(conf_inicial, cadena_e);
    }

    configuracions[0].posicio = conf_inicial;
    configuracions[0].profunditat = 0;

    printf("Configuració inicial: %s\n",configuracions[0].posicio);

    unsigned numconfactual = 0, numconf = 1;
    char *configuracio_actual=reservar_config(mida_conf);
    while(numconfactual < numconf && numconf < MAXCONF){
    strcpy(configuracio_actual, configuracions[numconfactual].posicio);
    for(unsigned i = 0; i < mida_conf; i++){
        if((configuracio_actual[i] == 'D' && configuracio_actual[i+1] == '_') || (configuracio_actual[i+1] == 'E' && configuracio_actual[i]  == '_')){
            canvi(&configuracions[numconf], &configuracions[numconfactual], i, 1, &numconf, mida_conf);
        }
        if (i < 3){
            if ((configuracio_actual[i] == 'D' && configuracio_actual[i+1] == 'E' && configuracio_actual[i+2] == '_') || (configuracio_actual[i] == '_' && configuracio_actual[i+1] == 'D' && configuracio_actual[i+2] == 'E')){
                canvi(&configuracions[numconf], &configuracions[numconfactual], i, 2, &numconf, mida_conf);
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

// void canvi_immediat(char *conf_desti, char *conf_inicial, unsigned index_canvi){
//     strcpy(conf_desti, conf_inicial);
//     conf_desti[index_canvi] = conf_inicial[index_canvi+1];
//     conf_desti[index_canvi+1] = conf_inicial[index_canvi];
// }
// void canvi_salt(char *conf_desti, char *conf_inicial, unsigned index_canvi){
//     strcpy(conf_desti, conf_inicial);
//     conf_desti[index_canvi] = conf_inicial[index_canvi+2];
//     conf_desti[index_canvi+2] = conf_inicial[index_canvi];
// }
char *reservar_config(unsigned mida_conf){
    char *config=NULL;
    if ((config =(char *)calloc(mida_conf, sizeof(char))) == NULL){
        perror("ERROR: No s'ha pogut reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    return config;
}
char *crear_cadena_uniforme(char *caracter, unsigned repeticions){
    char *cadena=NULL;
    // Reservem i inicialitzem a 0 la cadena amb el nombre de caracters + \0
    cadena = reservar_config(repeticions+1);
    for (unsigned i=0; i<repeticions; i++){
        strcat(cadena,caracter);
    }
    return cadena;
}
void canvi(configuracio *conf_desti, configuracio *conf_inicial, unsigned index_canvi, unsigned tipus_canvi, unsigned *num_conf, unsigned num_posicions){
    // Reservem memoria per la posicio a la configuracio de desti
    conf_desti->posicio = reservar_config(num_posicions);
    
    unsigned index_lletra = index_canvi;
    unsigned index_ = index_canvi + tipus_canvi;

    strcpy(conf_desti->posicio, conf_inicial->posicio);
    conf_desti->posicio[index_canvi] = conf_inicial->posicio[index_canvi+tipus_canvi];
    conf_desti->posicio[index_canvi+tipus_canvi] = conf_inicial->posicio[index_canvi];

    if (conf_inicial->posicio[index_canvi] == '_'){
        index_ = index_canvi;
        index_lletra = index_canvi + tipus_canvi;
    }
    conf_desti->profunditat = conf_inicial->profunditat + 1;
    printf("%s -(%u,%u)-> %s (profunditat = %u)\n",conf_inicial->posicio, index_lletra, index_,  conf_desti->posicio, conf_desti->profunditat);
    (*num_conf)++;
}
