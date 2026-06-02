#include "wordle.h"

// Funcție auxiliară pentru bsearch
int cmp_cuvinte(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

void incarca_dictionar(StareJoc *stare, const char *nume_fisier) {
    FILE *f = fopen(nume_fisier, "r");
    if (!f) {
        perror("Eroare la deschiderea fisierului biblioteca.txt");
        exit(EXIT_FAILURE);
    }

    stare->nr_cuvinte = 0;
    char buffer[20];
    while (fgets(buffer, sizeof(buffer), f) && stare->nr_cuvinte < MAX_CUVINTE) {
        // Trim și normalizare
        int j = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (isalpha(buffer[i])) {
                stare->dictionar[stare->nr_cuvinte][j++] = toupper(buffer[i]);
            }
        }
        stare->dictionar[stare->nr_cuvinte][j] = '\0';

        if (strlen(stare->dictionar[stare->nr_cuvinte]) == LUNGIME_CUVANT) {
            stare->nr_cuvinte++;
        }
    }
    fclose(f);

    // Sortăm dicționarul pentru a putea folosi bsearch ulterior
    qsort(stare->dictionar, stare->nr_cuvinte, LUNGIME_CUVANT + 1, cmp_cuvinte);
}

void alege_cuvant_tinta(StareJoc *stare) {
    if (stare->nr_cuvinte == 0) return;
    int index = rand() % stare->nr_cuvinte;
    strcpy(stare->tinta, stare->dictionar[index]);
}

void reseteaza_joc(StareJoc *stare) {
    memset(stare->istoric, 0, sizeof(stare->istoric));
    memset(stare->culori_istoric, 0, sizeof(stare->culori_istoric));
    memset(stare->stare_tastatura, 0, sizeof(stare->stare_tastatura));
    stare->incercare_curenta = 0;
    stare->mesaj_eroare[0] = '\0';
    alege_cuvant_tinta(stare);
}

bool valideaza_cuvant(StareJoc *stare, const char *incercare) {
    if (strlen(incercare) != LUNGIME_CUVANT) {
        strcpy(stare->mesaj_eroare, "Cuvantul trebuie sa aiba exact 5 litere!");
        return false;
    }

    void *rezultat = bsearch(incercare, stare->dictionar, stare->nr_cuvinte,
                             LUNGIME_CUVANT + 1, cmp_cuvinte);

    if (!rezultat) {
        strcpy(stare->mesaj_eroare, "Cuvantul nu se afla in dictionar!");
        return false;
    }

    stare->mesaj_eroare[0] = '\0'; // Ștergem eroarea dacă e valid
    return true;
}

void evalueaza_culori(StareJoc *stare, const char *incercare) {
    int frecventa_tinta[26] = {0};
    int culori_temp[LUNGIME_CUVANT] = {0}; // 0 = Gri/Negru, 1 = Galben, 2 = Verde

    // Inițializare frecvență
    for (int i = 0; i < LUNGIME_CUVANT; i++) {
        frecventa_tinta[stare->tinta[i] - 'A']++;
    }

    // Pasul 1: Identificare Verde
    for (int i = 0; i < LUNGIME_CUVANT; i++) {
        if (incercare[i] == stare->tinta[i]) {
            culori_temp[i] = 2; // Verde
            frecventa_tinta[incercare[i] - 'A']--;
        }
    }

    // Pasul 2: Identificare Galben / Gri
    for (int i = 0; i < LUNGIME_CUVANT; i++) {
        if (culori_temp[i] != 2) {
            if (frecventa_tinta[incercare[i] - 'A'] > 0) {
                culori_temp[i] = 1; // Galben
                frecventa_tinta[incercare[i] - 'A']--;
            } else {
                culori_temp[i] = 0; // Gri/Negru
            }
        }
    }

    // Salvare în istoric și actualizare tastatură
    strcpy(stare->istoric[stare->incercare_curenta], incercare);
    for (int i = 0; i < LUNGIME_CUVANT; i++) {
        stare->culori_istoric[stare->incercare_curenta][i] = culori_temp[i];

        // Ierarhia culorilor pentru taste (Implicit 0 -> Gri 1 -> Galben 2 -> Verde 3)
        int stare_noua = 1; // Gri
        if (culori_temp[i] == 1) stare_noua = 2; // Galben
        if (culori_temp[i] == 2) stare_noua = 3; // Verde

        int index_litera = incercare[i] - 'A';
        if (stare_noua > stare->stare_tastatura[index_litera]) {
            stare->stare_tastatura[index_litera] = stare_noua;
        }
    }

    stare->incercare_curenta++;
}
