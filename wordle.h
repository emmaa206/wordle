#ifndef WORDLE_H
#define WORDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAX_CUVINTE 15000
#define LUNGIME_CUVANT 5

// Structura principală a stării jocului
typedef struct {
    char dictionar[MAX_CUVINTE][LUNGIME_CUVANT + 1];
    int nr_cuvinte;

    char tinta[LUNGIME_CUVANT + 1];
    char istoric[6][LUNGIME_CUVANT + 1];
    int culori_istoric[6][LUNGIME_CUVANT];
    int stare_tastatura[26];
    int incercare_curenta;

    char mesaj_eroare[100];
} StareJoc;

// Funcții de logică
void incarca_dictionar(StareJoc *stare, const char *nume_fisier);
void alege_cuvant_tinta(StareJoc *stare);
void reseteaza_joc(StareJoc *stare);
bool valideaza_cuvant(StareJoc *stare, const char *incercare);
void evalueaza_culori(StareJoc *stare, const char *incercare);

// Funcții de interfață (UI)
void curata_ecran();
void afiseaza_grila(const StareJoc *stare);
void afiseaza_tastatura(const StareJoc *stare);
void citeste_incercare(StareJoc *stare);

#endif // WORDLE_H
