
#include <stdlib.h>
#include <malloc.h>

#include "polynom.h"

typedef struct _Monom {

    CoeffType coeff;
    int degree;

} Monom;


typedef struct _PolynomListEntry {

    struct _PolynomListEntry *next;
    Monom *monom;

} PolynomListEntry;


typedef struct _Polynom {

    int degree;
    int entryCount;
    PolynomListEntry *list;

} Polynom;


static Monom * CreateMonom (int degree, CoeffType coeff) {

};

static PolynomListEntry *CreatePolynomListEntry (int degree, CoeffType coeff) {

    PolynomListEntry * entry = (PolynomListEntry *) malloc (sizeof(PolynomListEntry));
    if (!entry) {
        return NULL;
        }

    entry->monom = CreateMonom (degree, coeff);
    if (!entry->monom) {
        free (entry);
        return NULL;
        }

    return entry;
}


static BOOL InitPolynom (Polynom *pol) {

    // ...

    pol->degree = 0;
    pol->entryCount = 0;
    pol->list = NULL;

    return TRUE;
}


static void InsertMononToPolynom (Polynom *pol, int degree, CoeffType coeff) {

}


static void FreePolynom (Polynom *pol) {


    return;
}


Polynom * CreatePolynom (void) {

    Polynom * pol = (Polynom *) malloc (sizeof (Polynom));

    if (!InitPolynom (pol)) {
        free (pol);
        return NULL;
        }

    return pol;
}


void DeletePolynom (Polynom *pol) {

    FreePolynom (pol);

    return;
}




Polynom * AddPolynoms (Polynom *pol1, Polynom *pol2) {

    Polynom * res = CreatePolynom ();

    return res;
}


void IncreasePolynom (Polynom *dst, Polynom *pol) {


}
