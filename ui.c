#include "wordle.h"
#include <ncurses.h>

void curata_ecran() {
    clear(); // Funcția nativă din ncurses pentru curățare eficientă
}

void afiseaza_grila(const StareJoc *stare) {
    int start_x = 22; // Punctul de aliniere comun pentru tot jocul

    mvprintw(2, start_x + 1, "=== WORDLE CTI ===");

    for (int r = 0; r < 6; r++) {
        move(5 + (r * 2), start_x);

        for (int c = 0; c < LUNGIME_CUVANT; c++) {
            if (r < stare->incercare_curenta) {
                int cod_culoare = stare->culori_istoric[r][c];
                char litera = stare->istoric[r][c];

                if (cod_culoare == 2) attron(COLOR_PAIR(2));
                else if (cod_culoare == 1) attron(COLOR_PAIR(1));
                else attron(COLOR_PAIR(3));

                printw(" %c ", litera);

                if (cod_culoare == 2) attroff(COLOR_PAIR(2));
                else if (cod_culoare == 1) attroff(COLOR_PAIR(1));
                else attroff(COLOR_PAIR(3));
            } else {
                attron(COLOR_PAIR(4));
                printw(" _ ");
                attroff(COLOR_PAIR(4));
            }
            printw(" ");
        }
    }
}

void afiseaza_tastatura(const StareJoc *stare) {
    const char *randuri_qwerty[] = {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
    };

    int start_y = 18;
    int start_x = 22;

    int offset_x[] = {start_x, start_x + 1, start_x + 3};

    for (int r = 0; r < 3; r++) {
        move(start_y + r, offset_x[r]);

        size_t lungime_rand = strlen(randuri_qwerty[r]);
        for (size_t i = 0; i < lungime_rand; i++) {
            char tasta = randuri_qwerty[r][i];
            int stare_tasta = stare->stare_tastatura[tasta - 'A'];

            if (stare_tasta == 3) attron(COLOR_PAIR(2));
            else if (stare_tasta == 2) attron(COLOR_PAIR(1));
            else if (stare_tasta == 1) attron(COLOR_PAIR(3));
            else attron(COLOR_PAIR(4));

            printw("%c", tasta);

            if (stare_tasta == 3) attroff(COLOR_PAIR(2));
            else if (stare_tasta == 2) attroff(COLOR_PAIR(1));
            else if (stare_tasta == 1) attroff(COLOR_PAIR(3));
            else attroff(COLOR_PAIR(4));

            printw(" ");
        }
    }
}

void citeste_incercare(StareJoc *stare) {
    char incercare_normalizata[LUNGIME_CUVANT + 1];
    int lungime_curenta = 0;

    // Curățăm zona de text de sub tastatură
    move(21, 2); clrtoeol();
    move(22, 2); clrtoeol();

    if (strlen(stare->mesaj_eroare) > 0) {
        attron(COLOR_PAIR(5)); // Culoare roșie pentru erori
        mvprintw(21, 2, "%s", stare->mesaj_eroare);
        attroff(COLOR_PAIR(5));
    }

    mvprintw(22, 2, "Introdu un cuvant (ENTER pentru validare): ");
    refresh();

    // Citire caracter cu caracter în stil ncurses
    while (1) {
        int ch = getch();

        // Dacă apasă Enter (cod ASCII 10 sau KEY_ENTER)
        if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {
            incercare_normalizata[lungime_curenta] = '\0';

            if (valideaza_cuvant(stare, incercare_normalizata)) {
                evalueaza_culori(stare, incercare_normalizata);
                break;
            } else {
                // Redesenăm ecranul pentru a afișa eroarea
                curata_ecran();
                afiseaza_grila(stare);
                afiseaza_tastatura(stare);

                move(21, 2); clrtoeol();
                attron(COLOR_PAIR(5));
                mvprintw(21, 2, "%s", stare->mesaj_eroare);
                attroff(COLOR_PAIR(5));

                move(22, 2); clrtoeol();
                mvprintw(22, 2, "Introdu un cuvant (ENTER pentru validare): ");

                // Reinițializăm buffer-ul local
                lungime_curenta = 0;
                move(22, 45);
                clrtoeol();
            }
        }
        // Backspace pentru ștergerea caracterelor
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (lungime_curenta > 0) {
                lungime_curenta--;
                int curr_y, curr_x;
                getyx(stdscr, curr_y, curr_x);
                mvprintw(curr_y, curr_x - 1, " "); // Ștergem vizual de pe ecran
                move(curr_y, curr_x - 1);
            }
        }
        // Introducere litere standard
        else if (isalpha(ch) && lungime_curenta < LUNGIME_CUVANT) {
            ch = toupper(ch);
            incercare_normalizata[lungime_curenta++] = ch;
            printw("%c", ch); // Afișăm pe ecran instantaneu
        }
        refresh();
    }
}
