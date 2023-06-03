#include <stdio.h>
#include <stdlib.h>
typedef struct
{
   int giorno;
   int mese;
   int anno; 
}Data;

typedef struct {
    Data data;
    int nConsiglieri;
    int nListe;
} Elezione;

typedef struct {
    char* nome;
    int nVoti;
} Lista;

/* Legge un int da tastiera.
IP etiche Etichetta per l'input.
IK+OR Int letto.
*/
int scanInt(const char etiche[]) {
    int i;
    printf("%s", etiche);
    scanf("%d", &i);
    return i;
} /* scanInt */

void allocateScanElezione(Elezione *ele) {
    ele=malloc(sizeof(Elezione));
    ele->data.giorno=scanInt("Giorno elezione:");
    ele->data.mese=scanInt("Mese elezione:");
    ele->data.anno=scanInt("Anno elezione:");
    ele->nConsiglieri=scanInt("N consiglieri da eleggere:");
    ele->nListe=scanInt("N Liste presentate:");

}
void trovaRisultati(Elezione *ele) {}
void stampaRisultati(const Elezione *ele) {}
int main() {
    Elezione* elez=NULL;
    allocateScanElezione(elez);
    printf("ciao\n");
    free(elez);
    return 0;
}
