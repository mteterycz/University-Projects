/**
 * "Reversi"
 *
 * Program zaliczeniowy 1 do zajęć labolatoryjnych ze Wstępu do programowania.
 *
 * Kompilacja poleceniem:
 * gcc @opcje reversi.c -o reversi
 *
 * Program umożliwia grę w "Reversi".
 *
 * Program, w pętli:
 * 1. pisze tekst zachęty wskazujący, do którego gracza należy ruch i jakie legalne ruchy może on wykonać;
 * 2. czyta polecenie aktualnego gracza;
 * 3. jeśli wczytał polecenie przerwania gry (=), kończy pracę;
 * 4. jeśli wczytał polecenie rezygnacji z ruchu (-), wraca na początek pętli;
 * 5. jeśli wczytał polecenie wykonania ruchu (np. c3), to wykonuje ten ruch i wraca na początek pętli.
 *
 * autor: Maciej Teterycz <mt459557@students.mimuw.edu.pl>
 * wersja: 1.0
 * data: 28 listopada 2023
 */

#include <stdio.h>

// rozmiar planszy
#define KOLUMNY 8
#define WIERSZE 8

// zwraca indeks planszy (a-h) dla odpowiadającego mu indeksu w tablicy (0-7)
char zamiana_na_litere(int x) {
    switch(x) {
        case 0:
            return 'a';
        case 1:
            return 'b';
        case 2:
            return 'c';
        case 3:
            return 'd';
        case 4:
            return 'e';
        case 5:
            return 'f';
        case 6:
            return 'g';
        case 7:
            return 'h';
    }
    return 0;
}

// zwraca indeks tablicy (0-7) dla odpowiadającego mu indeksu na planszy (a-h)
int zamiana_na_cyfre(int x) {
    switch(x) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
    }
    return 0;
}

// Sprawdzenie dla x-tego wiersza i y-tej kolumny czy ruch na tym polu jest legalny.
// Jeśli okaże się, że jest legalny to wyświetla to pole.
void legalne_ruchy(int y, int x, char tablica[][KOLUMNY], char gracz, char przeciwnik) {
    // Jeśli na polu stoi jakiś pionek, to ruch nie jest legalny.
    if (tablica[y][x] == ' ') {
        // Jeśli wiemy, że ruch jest legalny (dalej_szukamy == 0) to nie ma sensu dalej sprawdzać.
        int dalej_szukamy = 1;
        // Sprawdzamy po kolei w każdym kierunku czy aktualny ruch jest legalny:
        // w górę (dla 1 i 2 wiersza nie ma sensu)
        if (dalej_szukamy && y > 1 && tablica[y-1][x] == przeciwnik) {
            int y_temp = y - 2;
            while ( y_temp > 0 && tablica[y_temp][x] == przeciwnik) {
                --y_temp;
            }
            if (tablica[y_temp][x] == gracz) {
                printf(" %c%d", zamiana_na_litere(x), y + 1);
                dalej_szukamy = 0;
            }
        }
        // po skosie w prawo-góra (dla 1 i 2 wiersza i dla kolumn g i h nie ma sensu)
        if (dalej_szukamy && y > 1 && x < KOLUMNY - 2 && tablica[y-1][x+1] == przeciwnik) {
            int x_temp = x + 2;
            int y_temp = y - 2;
            while (x_temp < KOLUMNY - 1 && y_temp > 0 && tablica[y_temp][x_temp] == przeciwnik) {
                ++x_temp;
                --y_temp;
            }
            if (tablica[y_temp][x_temp] == gracz) {
                printf(" %c%d", zamiana_na_litere(x), y + 1);
                dalej_szukamy = 0;
            }
        }
        // w prawo (dla kolumn g i h nie ma sensu)
        if (dalej_szukamy && x < KOLUMNY - 2 && tablica[y][x+1] == przeciwnik) {
            int x_temp = x + 2;
            while (x_temp < KOLUMNY - 1 && tablica[y][x_temp] == przeciwnik) {
                ++x_temp;
            }
            if (tablica[y][x_temp] == gracz) {
                printf(" %c%d", zamiana_na_litere(x), y + 1);
                dalej_szukamy = 0;
            }
        }
        // po skosie w prawo-dół (dla 7 i 8 wiersza i kolumn g i h nie ma sensu)
        if (dalej_szukamy && y < WIERSZE - 2 && x < KOLUMNY - 2 && tablica[y+1][x+1] == przeciwnik) {
            int x_temp = x + 2;
            int y_temp = y + 2;
            while (x_temp  < KOLUMNY - 1 && y_temp < WIERSZE - 1 && tablica[y_temp][x_temp] == przeciwnik) {
                ++x_temp;
                ++y_temp;
            }
            if (tablica[y_temp][x_temp] == gracz) {
                printf(" %c%d", zamiana_na_litere(x), y + 1);
                dalej_szukamy = 0;
            }
        }
        // w dół (dla 7 i 8 wiersza nie ma sensu)
        if (dalej_szukamy && y < WIERSZE - 2 && tablica[y+1][x] == przeciwnik) {
            int y_temp = y + 2;
            while (y_temp  < WIERSZE - 1 && tablica[y_temp][x] == przeciwnik) {
                ++y_temp;
            }
            if (tablica[y_temp][x] == gracz) {
                printf(" %c%d", zamiana_na_litere(x), y + 1);
                dalej_szukamy = 0;
            }
        }
        // po skosie w lewo-dół (dla kolumn a i b oraz wierszy 7 i 8 nie ma sensu)
        if (dalej_szukamy && x > 1 && y < WIERSZE - 2 && tablica[y+1][x-1] == przeciwnik) {
            int x_temp = x - 2;
            int y_temp = y + 2;
            while (x_temp > 0 && y_temp  < WIERSZE - 1 && tablica[y_temp][x_temp] == przeciwnik) {
                --x_temp;
                ++y_temp;
            }
            if (tablica[y_temp][x_temp] == gracz) {
                printf(" %c%d", zamiana_na_litere(x), y + 1);
                dalej_szukamy = 0;
            }
        }
        // w lewo (dla kolumny a i b nie ma sensu)
        if (dalej_szukamy && x > 1 && tablica[y][x-1] == przeciwnik) {
            int x_temp = x - 2;
            while (x_temp > 0 && tablica[y][x_temp] == przeciwnik) {
                --x_temp;
            }
            if (tablica[y][x_temp] == gracz) {
                printf(" %c%d", zamiana_na_litere(x), y + 1);
                dalej_szukamy = 0;
            }
        }
        // po skosie w lewo-góra (dla kolumn a i b oraz wierszy 1 i 2 nie ma sensu)
        if (dalej_szukamy && y > 1 && x > 1 && tablica[y-1][x-1] == przeciwnik) {
            int x_temp = x - 2;
            int y_temp = y - 2;
            while (x_temp > 0 && y_temp > 0 && tablica[y_temp][x_temp] == przeciwnik) {
                --x_temp;
                --y_temp;
            }
            if (tablica[y_temp][x_temp] == gracz) {
                printf(" %c%d", zamiana_na_litere(x), y + 1);
                dalej_szukamy = 0;
            }
        }
    }
}

// Wykonanie ruchu - zamiana odpowiednich pól na piony przeciwnika
void wykonanie_ruchu(int y, int x, char tablica[][KOLUMNY], char gracz, char przeciwnik) {
    // Sprawdzenie po kolei w każdym kierunku czy zamienić piony i jeśli zamienić to wykonanie zamiany.
    // w górę
    if (y > 1 && tablica[y-1][x] == przeciwnik) {
        int y_temp = y - 2;
        while (y_temp > 0 && tablica[y_temp][x] == przeciwnik) {
            --y_temp;
        }
        if (tablica[y_temp][x] == gracz) {
            ++y_temp;
            while (y_temp <= y) {
                tablica[y_temp][x] = gracz;
                ++y_temp;
            }
        }
    }
    // po skosie w prawo-góra
    if (y > 1 && x < KOLUMNY - 2 && tablica[y-1][x+1] == przeciwnik) {
        int x_temp = x + 2;
        int y_temp = y - 2;
        while (x_temp < KOLUMNY - 1 && y_temp > 0 && tablica[y_temp][x_temp] == przeciwnik) {
            ++x_temp;
            --y_temp;
        }
        if (tablica[y_temp][x_temp] == gracz) {
            --x_temp;
            ++y_temp;
            while(y_temp <= y) {
                tablica[y_temp][x_temp] = gracz;
                --x_temp;
                ++y_temp;
            }
        }
    }
    // w prawo
    if (x < KOLUMNY - 2 && tablica[y][x+1] == przeciwnik) {
        int x_temp = x + 2;
        while (x_temp < KOLUMNY - 1 && tablica[y][x_temp] == przeciwnik) {
            ++x_temp;
        }
        if (tablica[y][x_temp] == gracz) {
            --x_temp;
            while (x_temp >= x) {
                tablica[y][x_temp] = gracz;
                --x_temp;
            }
        }
    }
    // po skosie w prawo-dół
    if (y < WIERSZE - 2 && x < KOLUMNY - 2 && tablica[y+1][x+1] == przeciwnik) {
        int x_temp = x + 2;
        int y_temp = y + 2;
        while (x_temp < KOLUMNY - 1 && y_temp < WIERSZE - 1 && tablica[y_temp][x_temp] == przeciwnik) {
            ++x_temp;
            ++y_temp;
        }
        if (tablica[y_temp][x_temp] == gracz) {
            --x_temp;
            --y_temp;
            while (x_temp >= x) {
                tablica[y_temp][x_temp] = gracz;
                --x_temp;
                --y_temp;
            }
        }
    }
    // w dół
    if (y < WIERSZE - 2 && tablica[y+1][x] == przeciwnik) {
        int y_temp = y + 2;
        while (y_temp < WIERSZE - 1 && tablica[y_temp][x] == przeciwnik) {
            ++y_temp;
        }
        if (tablica[y_temp][x] == gracz) {
            --y_temp;
            while (y_temp >= y) {
                tablica[y_temp][x] = gracz;
                --y_temp;
            }
        }
    }
    // po skosie w lewo-dół
    if (x > 1 && y < WIERSZE - 2 && tablica[y+1][x-1] == przeciwnik) {
        int x_temp = x - 2;
        int y_temp = y + 2;
        while (x_temp > 0 && y_temp < WIERSZE - 1 && tablica[y_temp][x_temp] == przeciwnik) {
            --x_temp;
            ++y_temp;
        }
        if (tablica[y_temp][x_temp] == gracz) {
            ++x_temp;
            --y_temp;
            while (x_temp <= x) {
                tablica[y_temp][x_temp] = gracz;
                ++x_temp;
                --y_temp;
            }
        }
    }
    // w lewo
    if (x > 1 && tablica[y][x-1] == przeciwnik) {
        int x_temp = x - 2;
        while (x_temp > 0 && tablica[y][x_temp] == przeciwnik) {
            --x_temp;
        }
        if (tablica[y][x_temp] == gracz) {
            ++x_temp;
            while (x_temp <= x) {
                tablica[y][x_temp] = gracz;
                ++x_temp;
            }
        }
    }
    // po skosie w lewo-góra
    if (y > 1 && x > 1 && tablica[y-1][x-1] == przeciwnik) {
        int x_temp = x - 2;
        int y_temp = y - 2;
        while (x_temp > 0 && y_temp > 0 && tablica[y_temp][x_temp] == przeciwnik) {
            --x_temp;
            --y_temp;
        }
        if (tablica[y_temp][x_temp] == gracz) {
            ++x_temp;
            ++y_temp;
            while (x_temp <= x) {
                tablica[y_temp][x_temp] = gracz;
                ++x_temp;
                ++y_temp;
            }
        }
    }
}

// Uruchomienie programu.
int main(void) {
    // Implemetacja tablicy odpowiadającej startowej planszy do gry.
    char plansza[WIERSZE][KOLUMNY];
    for (int x = 0; x < KOLUMNY; ++x) {
        for (int y = 0; y < WIERSZE; ++y) {
            plansza[y][x] = ' ';
        }
    }
    // Ustawienie pionów początkowych.
    plansza[3][3] = 'B';
    plansza[4][4] = 'B';
    plansza[3][4] = 'C';
    plansza[4][3] = 'C';
    
    // Gre rozpoczyna gracz Czarny.
    char gracz = 'C';
    char przeciwnik = 'B';
    
    // Implementacja zmiennych, które będą odpowiadać współrzędnym.
    int a;
    int b;
    
    // Implementacja zmiennej, która określa czy gra ma się toczyć dalej.
    int dalej = 1;
    
    // Rozpoczęcie gry
    while (dalej) {
        // Wyświetlenie aktualnego gracza.
        printf("%c", gracz);
        // Wyświetlanie legalnych ruchów
        for (int x = 0; x < KOLUMNY; ++x) {
            for (int y = 0; y < WIERSZE; ++y) {
                legalne_ruchy(y, x, plansza, gracz, przeciwnik);
            }
        }
        printf("\n");
        
        // Wczytanie polecenia aktualnego gracza.
        a = getchar();
        if (a == '\n') {
            a = getchar();
        }
        b = getchar();
        
        // Jeśli gracz przerwał grę - koniec programu.
        if (a == '=') {
            dalej = 0;
        }
        else {
            // Jeśli gracz nie zrezygnował z ruchu - wykonanie ruchu.
            if (a != '-') {
                b = b - '0';
                if ( a != '-') {
                    wykonanie_ruchu(b - 1, zamiana_na_cyfre(a), plansza, gracz, przeciwnik);
                }
            }
            // Zamiana aktualnego gracza
            if (gracz == 'C') {
                gracz = 'B';
                przeciwnik = 'C';
            }
            else {
                gracz = 'C';
                przeciwnik = 'B';
            }
            
        }
    }
    return 0;
}
