#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "wordle.h"

int main() {
    srand((unsigned int)time(NULL));

    initscr();             // Pornim modul ncurses
    cbreak();              // Citim caracterele imediat, fără tampon de linie
    noecho();              // Dezactivăm afișarea automată a caracterelor tastate
    keypad(stdscr, TRUE);  // Permitem taste speciale (Săgeți, Backspace, etc.)
    start_color();         // Activăm suportul de culori

    // Definim perechi de culori
    init_pair(1, COLOR_BLACK, COLOR_YELLOW); // Galben
    init_pair(2, COLOR_WHITE, COLOR_GREEN);  // Verde
    init_pair(3, COLOR_WHITE, 8);  // Gri
    init_pair(4, COLOR_WHITE, COLOR_BLACK);   // Standard
    init_pair(5, COLOR_RED, COLOR_BLACK);     // Erori

    StareJoc stare;
    incarca_dictionar(&stare, "biblioteca.txt");
    reseteaza_joc(&stare);

    while (1) {
        curata_ecran();
        afiseaza_grila(&stare);
        afiseaza_tastatura(&stare);

        citeste_incercare(&stare);

        bool terminat = false;

        if (strcmp(stare.istoric[stare.incercare_curenta - 1], stare.tinta) == 0) {
            curata_ecran();
            afiseaza_grila(&stare);
            afiseaza_tastatura(&stare);
            mvprintw(21, 2, "FELICITARI! Ai ghicit cuvantul: %s", stare.tinta);
            terminat = true;
        }
        else if (stare.incercare_curenta == 6) {
            curata_ecran();
            afiseaza_grila(&stare);
            afiseaza_tastatura(&stare);
            mvprintw(21, 2, "GAME OVER! Cuvantul era: %s", stare.tinta);
            terminat = true;
        }

        if (terminat) {
            mvprintw(22, 2, "Vrei sa joci din nou? (Y/N): ");
            refresh();
            int opt = getch();
            if (toupper(opt) == 'Y') {
                reseteaza_joc(&stare);
            } else {
                mvprintw(23, 2, "Mersi de joc! Iesire...");
                refresh();   // Randăm textul pe ecran
                napms(1500); // Pauză de 1500 milisecunde (1.5 secunde)
                break;
            }
        }
    }
    endwin();
    return 0;
}
