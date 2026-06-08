# Wordle - Varianta în Limba Română (Proiect C)

Acest proiect reprezintă o implementare a popularului joc de cuvinte **Wordle**, scris în limbajul **C** și conceput pentru a rula direct în terminalul de Linux, folosind biblioteca grafică `ncurses`.

## Funcționalități
* **Interfață grafică în terminal:** Folosește `ncurses` pentru a desena o grilă de joc interactivă și colorată.
* **Vocabular 100% Românesc:** Jocul utilizează un dicționar personalizat (`biblioteca.txt`) format din 1660 de cuvinte uzuale de fix 5 litere din limba română. 
* **Normalizare:** Toate cuvintele sunt procesate folosind majuscule și fără diacritice, exact ca în jocul original.
* **Control :** Jocul procesează doar caracterele valide și permite părăsirea rapidă și curată a aplicației.

## Cerințe de sistem
Pentru a putea compila și juca acest joc, sistemul tău (Linux / Ubuntu / VirtualBox) trebuie să aibă instalate următoarele pachete:
* Compiler-ul `gcc`
* Utilitarul `make`
* Biblioteca `ncurses`
   `ncurses` se poate instala rulând comanda:
   ```bash
   sudo apt-get install libncurses5-dev libncursesw5-dev

## Compilare și Rulare
Proiectul folosește un `Makefile` pentru a automatiza procesul de compilare. 

1. Deschide terminalul în folderul proiectului.
2. Curăță eventualele fișiere compilate anterior:
   ```bash
   make clean
   make
   ./wordle
   
## Cum se joacă?
Regulile sunt identice cu cele ale jocului clasic:
Ai la dispoziție 6 încercări pentru a ghici cuvântul secret de 5 litere.
După ce scrii un cuvânt și apeși tasta ENTER, culorile literelor se vor schimba pentru a-ți oferi indicii:
🟩 Verde: Litera se află în cuvântul secret și este pe poziția corectă.
🟨 Galben: Litera se află în cuvântul secret, dar este pe o poziție greșită.
⬜ Gri / Default: Litera nu se află deloc în cuvântul secret.

## Comenzi tastatură:
[A - Z] : Introducerea literelor (sunt transformate automat în majuscule).
[BACKSPACE] : Ștergerea ultimei litere introduse.
[ENTER] : Confirmarea cuvântului (funcționează doar dacă rândul conține 5 litere).
[Ctrl + D] sau [Ctrl + C] : Ieșire de urgență / Închiderea curată a jocului în orice moment.

## Structura proiectului
Proiectul este organizat modular pentru a separa eficient logica internă a jocului de interfața grafică:

* **`main.c`** - Punctul de intrare în aplicație. Inițializează mediul vizual `ncurses`, definește paleta de culori și coordonează bucla principală a jocului (game loop). De asemenea, gestionează stările de final (Victorie / Game Over) și meniul de reluare a jocului.
* **`logica.c`** - Reprezintă "creierul" aplicației. Include funcțiile pentru prelucrarea datelor: citirea și sortarea dicționarului (`incarca_dictionar`), selectarea aleatorie a cuvântului secret (`alege_cuvant_tinta`), validarea input-ului printr-o căutare binară eficientă (`bsearch`) și algoritmul central care determină corectitudinea literelor (Verde, Galben, Gri) prin `evalueaza_culori`.
* **`ui.c`** - Modulul responsabil exclusiv de interfața grafică (User Interface). Folosește funcții `ncurses` pentru a desena dinamic grila de joc (`afiseaza_grila`), pentru a randa o tastatură virtuală QWERTY interactivă (`afiseaza_tastatura`) și pentru a citi/șterge literele introduse de utilizator în timp real (`citeste_incercare`).
* **`wordle.h`** - Fișierul antet (header) central. Aici sunt grupate bibliotecile necesare, constantele globale (ex: `LUNGIME_CUVANT`) și definiția structurii principale `StareJoc` (care stochează istoricul încercărilor, cuvântul țintă și starea tastaturii). Tot aici sunt declarate prototipurile funcțiilor pentru a asigura comunicarea între fișierele `.c`.
* **`makefile`** - Scriptul care simplifică și automatizează procesul de compilare a codului sursă într-un fișier executabil.
* **`biblioteca.txt`** - Dicționarul personalizat care conține cuvintele valide.
* **`.gitignore`** - Fișier de configurare pentru a preveni încărcarea fișierelor binare / compilate pe repository-ul Git.
