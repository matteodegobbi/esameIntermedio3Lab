/*Matteo De Gobbi*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LUNGHEZZA_NOME 10

typedef struct {
    int giorno;
    int mese;
    int anno;
} Data;
typedef struct {
    char* nome;
    int nVoti;
    int nConsAttribuiti;
    double nResto;
} Lista;
typedef struct {
    /*Uso Data* e non Data perche' dalla consegna dobbiamo minimizzare la memoria occupata da Elezione*/
    Data* data;
    int nConsiglieri;
    int nListe;
    Lista* liste;
} Elezione;
/* Funzione che confronta due valori.
IP a Indirizzo del primo valore da confrontare.
IP b Indirizzo del secondo valore da confrontare.
OR Un valore < 0, se a <b;
   0, se a == b;
   Un valore > 0, se a >b.
*/
typedef int (*fCompare)(const void*, const void*);

/*Funzione che compara due Liste (ricevendo un puntatore a un puntatore a una Lista) tramite il nResto
Se $a ha un nResto maggiore viene ritornato 1, se $b ha un nResto maggiore -1, altrimenti 0
IP a,b due Lista** da confrontare tramite il resto
OR risultato del comparison con le regole specificate da fCompare

Attenzione in questo compare non e' sufficiente restituire la differenza dei due nResto perche' il cast
implicito da double a int crea problemi nel caso di valori della differenza minori di 1 in valore assoluto
*/
int compareListePtByResto(const Lista** a, const Lista** b) {
    if (((*a)->nResto) > ((*b)->nResto)) {
        return 1;
    } else if (((*a)->nResto) < ((*b)->nResto)) {
        return -1;
    } else {
        return 0;
    }
} /*compareListePtByResto*/

/* Scambia il contenuto di due variabili, un byte per volta.
IOP a1 Indirizzo del primo elemento da scambiare
IOP a2 Indirizzo del secondo elemento da scambiare
IP nByte  numero di byte occupato da ciascuna variabile
*/
void swap(char* a1, char* a2, int nByte) {
    char sca;
    int i;
    for (i = 0; i < nByte; i++) {
        sca = *a1;
        *a1 = *a2;
        *a2 = sca;
        a1++;
        a2++;
    } /* for */
} /* swap */

/* Usa l'ultimo elemento come pivot, e poi:
~ Sposta gli elementi piu` piccoli a sinistra del pivot.
~ Sposta gli elementi piu` grandi a destra.
~ Sposta il pivot al confine tra i due insiemi suddetti.
IP a Array da partizionare.
IP n Grandezza dell'array $a.
IP nByte Numero di byte occupato da ogni elemento di $a.
IP cmp Funzione per effettuare i confronti.
OR Indice del pivot.
*/
int partition(void* a, int n, int nByte, fCompare cmp) {
    int i, indicePivot = 0;
    char* pAI = a;                             /* Puntatore ad a[i], con i = 0, .., n - 1 */
    char* pASwap = a;                          /* Puntatore all'elemento di $a che sara` scambiato con a[i] */
    char* pPivot = (char*)a + (n - 1) * nByte; /* Puntatore al pivot, che e` a[n - 1] */
    indicePivot = 0;
    for (i = 0; i < n; i++) {
        if (cmp(pAI, pPivot) < 0) {
            swap(pAI, pASwap, nByte);
            pASwap += nByte;
            indicePivot++;
        } /* if */
        pAI += nByte;
    } /* for */
    swap(pPivot, pASwap, nByte);
    return indicePivot;
} /* partition */

/* Ritorna il k-esimo elemento piu` piccolo di un array.
IP a Array da elaborare.
IP n Grandezza di a.
IP nByte Numero di byte di un elemento di a.
IP k indice dell'elemento da ritornare. 0 <= k < n
IP cmp Funzione di confronto tra gli elementi di $a.
OR Indirizzo del k-esimo elemento piu` piccolo di $a.
*/
void* quickSelect(void* a, int n, int k, int nByte, fCompare cmp) {
    int indicePivot = partition(a, n, nByte, cmp);
    if (indicePivot == k)
        return (char*)a + indicePivot * nByte;
    else if (indicePivot < k) /* Cerca l'elemento in a[indicePivot + 1, .., n - 1] */
        return quickSelect((char*)a + (indicePivot + 1) * nByte, n - indicePivot - 1, k - indicePivot - 1,
                           nByte, cmp);
    else /* Cerca l'elemento in a[0, .., indicePivot - 1] */
        return quickSelect(a, indicePivot, k, nByte, cmp);
} /* quickSelect */

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

/*Si occupa di allocare i campi della Lista* $li e di leggere da tastiera i dati necessari per
 inizializzarli
 OP li la Lista* di cui dobbiamo allocare il campo nome e inizializzare i campi nome e nVoti
 */
void allocateScanLista(Lista* li) {
    li->nome = malloc(MAX_LUNGHEZZA_NOME + 1);
    assert(li->nome);
    printf("%s\n", "Nome lista (max 10 car):");
    scanf("%s", (li->nome));
    /* questo assert puo'essere utile per controllare di aver inserito dati validi durante il debug*/
    /*assert(strlen(li->nome)<=10);*/
    /*li->nome = realloc(li->nome, strlen(li->nome) + 1);*/ assert(li->nome);
    /*rialloco per occupare meno memoria possibile come richiesto dalla consegna*/

    /*Attenzione se fallisce l'assert viene liberata tutta la memoria ma se si toglie assert bisogna liberare
    il puntatore passato alla realloc e quindi in quel caso non potremmo assegnare a li->nome il risultato
    della realloc prima di controllare se e' NULL*/

    li->nVoti = scanInt("nVoti:\n");
} /*allocateScanLista*/
/*Si occupa di allocare e inizializzare i campi di $ele
OP ele la struttura Elezione di cui dobbiamo allocare i campi data e liste e inizializzare data, liste, nListe
e nConsiglieri
*/
void allocateScanElezione(Elezione* ele) {
    int i;
    ele->data = malloc(sizeof(Data));
    assert(ele->data);
    ele->data->giorno = scanInt("Giorno elezione:\n");
    ele->data->mese = scanInt("Mese elezione:\n");
    ele->data->anno = scanInt("Anno elezione:\n");
    ele->nConsiglieri = scanInt("N consiglieri da eleggere:\n");
    ele->nListe = scanInt("N Liste presentate:\n");
    ele->liste = malloc(sizeof(Lista) * ele->nListe);
    assert(ele->liste);
    for (i = 0; i < ele->nListe; i++) {
        allocateScanLista(ele->liste + i);
        /*printf("info:%d\n",ele->liste+i);*/
    }
}
/*Esegue il print dei campi di $ele disponibili subito dopo la lettura
OV La data della votazione nel formato DD/MM/YYYY, il numero di consiglieri da eleggere e tutte le liste
candidate nel formato LISTA nome. Voti:nVoti.
*/
void printaDopoLettura(const Elezione* ele) {
    int i = 0;
    printf("Data della votazione: %d/%d/%d\n", ele->data->giorno, ele->data->mese, ele->data->anno);
    printf("Numero di consiglieri da eleggere: %d \n", ele->nConsiglieri);

    for (i = 0; i < ele->nListe; i++) {
        printf("LISTA %s. Voti: %d.\n", ele->liste[i].nome, ele->liste[i].nVoti);
    }
}
/*Calcola i risultati delle elezioni con il sistema proporzionale a liste bloccate, questa funzione modifica i
  campi degli elementi di $ele->liste
  IOP ele la struttura Elezione di cui dobbiamo calcolare i risultati, che verranno poi stampati dalla
  funzione stampaRisultati
 */
void trovaRisultati(Elezione* ele) {
    double quorum = 0;
    int nConsiMancanti = 0, nConsiAssegnati = 0, i;
    for (i = 0; i < ele->nListe; i++) {
        quorum += (double)ele->liste[i].nVoti;
    }
    quorum /= ele->nConsiglieri;
    for (i = 0; i < ele->nListe; i++) {
        ele->liste[i].nConsAttribuiti = (int)((double)(ele->liste[i].nVoti) / quorum);
        nConsiAssegnati += ele->liste[i].nConsAttribuiti;
    }
    nConsiMancanti = ele->nConsiglieri - nConsiAssegnati;
    if (nConsiMancanti != 0) { /*Creo solo nel caso sia necessario cosi' risparmio memoria/tempo*/
        Lista** copiaIndirizziListe = malloc(ele->nListe * sizeof(Lista*));
        assert(copiaIndirizziListe);

        for (i = 0; i < ele->nListe; i++) {
            ele->liste[i].nResto = (ele->liste[i].nVoti - quorum * (ele->liste[i].nConsAttribuiti));
            copiaIndirizziListe[i] = &(ele->liste[i]);
        }
        quickSelect(copiaIndirizziListe, ele->nListe, ele->nListe - nConsiMancanti, sizeof(Lista*),
                    (fCompare)compareListePtByResto);
        for (i = ele->nListe - nConsiMancanti; i < ele->nListe; i++) {
            copiaIndirizziListe[i]->nConsAttribuiti++;
        }
        free(copiaIndirizziListe);
    }
}/*trovaRisultati*/

/*Stampa i risultati delle elezioni calcolati dalla funzione trovaRisultati con il formato specificato dalla consegna
  IP ele la Elezione* su cui abbiamo precedentemente calcolato i risultati
*/
void stampaRisultati(const Elezione* ele) {
    int i;
    for (i = 0; i < ele->nListe; i++) {
        printf("LISTA %s. Voti: %d. Numero consiglieri attribuiti: %d\n", ele->liste[i].nome,
               ele->liste[i].nVoti, ele->liste[i].nConsAttribuiti);
    }
} /*stampaRisultati*/
int main(void) {
    int i;
    Elezione* elez = malloc(sizeof(Elezione));
    assert(elez);
    allocateScanElezione(elez);
    printaDopoLettura(elez);
    trovaRisultati(elez);
    stampaRisultati(elez);
    /*Libero la memoria allocata*/
    for (i = 0; i < elez->nListe; i++) {
        free(elez->liste[i].nome);
    }
    free(elez->data);
    free(elez->liste);
    free(elez);
    return 0;
} /*main*/
