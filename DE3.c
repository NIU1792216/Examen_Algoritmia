// Nom: Mario Roda Sevilla
// NIU: 1792216
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

typedef struct configuracio{
    char *posicio;
    unsigned profunditat;
} configuracio;
// Definim la estructura d'un element de la cua
typedef struct ElementCua{
    struct ElementCua *seguent;
    configuracio *conf;
} ElementCua;
// Definim la estructura de la cua
typedef struct Cua{
    ElementCua *inici;
    ElementCua *final;
} Cua;

char *reservar_config(unsigned mida_conf);
char *crear_cadena_uniforme(char caracter, unsigned repeticions);
void canvi_immediat(char *conf_desti, char *conf_inicial, unsigned index_canvi);
void canvi_salt(char *conf_desti, char *conf_inicial, unsigned index_canvi);
void canvi(const configuracio *conf_inicial, unsigned index_canvi, unsigned tipus_canvi, unsigned mida_conf, Cua *cua);
void encua(Cua *cua, configuracio *conf);
configuracio *desencua(Cua *cua);
void alliberar_cua(Cua *cua);

int main(int argc, char *argv[]){
    // Comprovem que el programa es cridi amb un argument
    if (argc != 2){
        fprintf(stderr,"ERROR: El programa s'ha de cridar amb un argument\n"); //NOLINT
        exit(EXIT_FAILURE);
    }
    // Configuracio entrada
    configuracio *conf_inicial=NULL;
    if ((conf_inicial = (configuracio *)malloc(sizeof(configuracio))) == NULL){
        perror("ERROR: No s'ha pogut reservar memoria\n");
        exit(EXIT_FAILURE);
    }
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
        // Afegim el caracter \0
        mida_conf += 1;
        // Comprovem que tots els caracters siguin D o E o _
        for (unsigned i=0; i<mida_conf; i++){
            if (argv[1][i] != 'D' && argv[1][i] != 'E' && argv[1][i] != '_'){
                fprintf(stderr, "ERROR: Els arguments d'entrada han de ser o un enter positiu o una cadena dels caracters E, D i _\n"); //NOLINT
                exit(EXIT_FAILURE);
            }
        }
        // Reservem tant espai com per la cadena introduida + \0
        conf_inicial->posicio = reservar_config(mida_conf);
        strlcpy(conf_inicial->posicio, argv[1], mida_conf);
    }
    else if (errno == ERANGE){
            perror("ERROR: Argument no valid o fora de rang\n");
            printf("Problema a %s\n", error);
            exit(EXIT_FAILURE);
    }
    // He posat un interval maxim per la n, ja que amb n massa gran el programa podria petar
    else if (n < 0 || n > 65){
        fprintf(stderr, "ERROR: L'entrada no pot ser un nombre negatiu ni major de 65"); //NOLINT
        exit(EXIT_FAILURE);
    }
    // Si entra aqui es que han entrat un nombre dintre del rang posat
    else {
        // La configuracio te n D, n E, 1 _ i 1 \0
        mida_conf = (unsigned)(2*n + 2);
        // Reservem memoria pel num de caracters + \0
        conf_inicial->posicio = reservar_config(mida_conf);
        char *cadena_d = crear_cadena_uniforme('D', n);
        char *cadena_e = crear_cadena_uniforme('E', n);
        // Copiem n Ds a la cadena conf_inicial, afegim un _ i concatenem n Es a la cadena
        sprintf(conf_inicial->posicio, "%s_%s", cadena_d, cadena_e); //NOLINT
        free(cadena_d);
        free(cadena_e);
    }
    Cua cua={NULL, NULL};
    conf_inicial->profunditat = 0;
    encua(&cua, conf_inicial);

    printf("Configuració inicial: %s\n", conf_inicial->posicio);

    while(cua.inici != NULL){
        configuracio *configuracio_actual = desencua(&cua);
        char *pos_actual = configuracio_actual->posicio;
        for(unsigned i = 0; i<(mida_conf-1); i++){
            if((pos_actual[i] == 'D' && pos_actual[i+1] == '_') || (pos_actual[i+1] == 'E' && pos_actual[i]  == '_')){
                canvi(configuracio_actual, i, 1, mida_conf, &cua);
            }
            if (i < (mida_conf-2)){
                if ((pos_actual[i] == 'D' && pos_actual[i+1] == 'E' && pos_actual[i+2] == '_') || (pos_actual[i] == '_' && pos_actual[i+1] == 'D' && pos_actual[i+2] == 'E')){
                    canvi(configuracio_actual, i, 2, mida_conf, &cua);
                }
            }
        }
        free(configuracio_actual->posicio);
        free(configuracio_actual);
    }
    printf("\n");
    // Alliberem memoria de la cua
    alliberar_cua(&cua);
    return 0;
}
char *reservar_config(unsigned mida_conf){
    char *config=NULL;
    if ((config =(char *)calloc(mida_conf, sizeof(char))) == NULL){ //NOLINT
        perror("ERROR: No s'ha pogut reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    return config;
}
char *crear_cadena_uniforme(char caracter, unsigned repeticions){
    char *cadena= NULL;
    // Reservem i inicialitzem a 0 la cadena amb el nombre de caracters + \0
    cadena = reservar_config(repeticions+1);
    for (unsigned i=0; i<repeticions; i++){
        cadena[i] = caracter;
    }
    cadena[repeticions] = '\0';
    return cadena;
}
void canvi(const configuracio *conf_inicial, unsigned index_canvi, unsigned tipus_canvi, unsigned mida_conf, Cua *cua){
    configuracio *conf_desti=NULL;
    if ((conf_desti = (configuracio *)malloc(sizeof(configuracio))) == NULL){
        perror("ERROR: No s'ha pogut reservar memoria\n");
    }
    // Reservem memoria per la posicio a la configuracio de desti
    conf_desti->posicio = reservar_config(mida_conf);
    /* Actuem com si la lletra fos el primer index i _ el segon.
       L'index de _ estara a tipus_canvi posicions de distancia de la "lletra".
       Mes endavant comprovem si el _ esta a on deiem que esta la lletra i canviem els index.*/
    unsigned index_lletra = index_canvi;
    unsigned index_ = index_canvi + tipus_canvi;

    strlcpy(conf_desti->posicio, conf_inicial->posicio, mida_conf);
    conf_desti->posicio[index_canvi] = conf_inicial->posicio[index_canvi+tipus_canvi];
    conf_desti->posicio[index_canvi+tipus_canvi] = conf_inicial->posicio[index_canvi];

    if (conf_inicial->posicio[index_canvi] == '_'){
        index_ = index_canvi;
        index_lletra = index_canvi + tipus_canvi;
    }
    conf_desti->profunditat = conf_inicial->profunditat + 1;
    printf("%s -(%u,%u)-> %s (profunditat = %u)\n",conf_inicial->posicio, index_lletra, index_,  conf_desti->posicio, conf_desti->profunditat);
    // Encuem la configuracio
    encua(cua, conf_desti);
}
void encua(Cua *cua, configuracio *conf){
    ElementCua *element=NULL;
    if ((element = (ElementCua *)malloc(sizeof(ElementCua))) == NULL){
        perror("ERROR: No s'ha pogut reservar memoria\n");
        exit(EXIT_FAILURE);
    }
    element->conf = conf;
    element->seguent = NULL;
    // Si la cua es buida, els dos apuntadors apunten al mateix element
    if (cua->inici == NULL){
        cua->inici = element;
        cua->final = element;
        return;
    }
    cua->final->seguent = element;
    cua->final = element;
}
configuracio *desencua(Cua *cua){
    if (cua->final == NULL){
        return NULL;
    }
    ElementCua *element = cua->inici;
    configuracio *conf = element->conf;
    cua->inici = element->seguent;
    free(element);
    // Si hem tret l'ultim element, el final de la cua es NULL
    if (cua->inici == NULL){
        cua->final = NULL;
    }
    return conf;
}
void alliberar_cua(Cua *cua){
    configuracio *configuracio_actual=desencua(cua);
    while (configuracio_actual != NULL){
        free(configuracio_actual->posicio);
        free(configuracio_actual);
        configuracio_actual = desencua(cua);
    }
}