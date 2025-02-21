/**
 * "Pętlik"
 *
 * Program zaliczeniowy 3 do zajęć labolatoryjnych ze Wstępu do programowania.
 *
 * Kompilacja poleceniem:
 * gcc @opcje main.c -o petlik
 *
 * Program realizuje język programowania Pętlik.
 *
 * Program czyta i wykonuje polecenia:
 * - wypisania wartości zmiennej;
 * - wykonania programu w języku pętlik.
 * Wynik programu jest efektem wykonania poleceń wypisania wartości zmiennej.
 *
 * autor: Maciej Teterycz <mt459557@students.mimuw.edu.pl>
 * wersja: 1.0
 * data: 9 stycznia 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define liczbaZmiennych 26
#define domyslnyRozmiar 1
#define maksWartoscWTabeli 999999999999999999   // 10^18 - 1

typedef struct {
    char nazwa;
    unsigned long long int *wartosc;
    int ostatniIndeks;
    int rozmiar;
} wartosci_zmiennych;

// Zaalokowanie dodatkowej pamięci dla polecenia
void powiekszPolecenie(char **polecenieDoPowiekszenia, int *akt_dlugosc) {
    (*akt_dlugosc) *= 2;
    *polecenieDoPowiekszenia = realloc(*polecenieDoPowiekszenia, (size_t)(*akt_dlugosc) * sizeof(char));
    assert(*polecenieDoPowiekszenia != NULL);
}
// Zaalokowanie dodatkowej pamięci dla zmiennej
void powiekszZmienna(unsigned long long int **zmiennaDoPowiekszenia, int *ostatni_indeks, int *wielkosc) {
    ++(*ostatni_indeks);
    if (*ostatni_indeks == *wielkosc) {
        (*wielkosc) *= 2;
        *zmiennaDoPowiekszenia = realloc(*zmiennaDoPowiekszenia, (size_t)(*wielkosc) * sizeof(unsigned long long int));
    }
    (*zmiennaDoPowiekszenia)[(*ostatni_indeks)] = 0;
    assert(*zmiennaDoPowiekszenia != NULL);
}
// Zwiekszenie zmiennej o 1
void increment(unsigned long long int **zmienna, int *indeks, int *rozmiar) {
    int i = 0;
    while (i <= *indeks && (*zmienna)[i] == maksWartoscWTabeli) {
        (*zmienna)[i] = 0;
        ++i;
    }
    if (i - 1 == *indeks) powiekszZmienna(zmienna, indeks, rozmiar);
    ++(*zmienna)[i];
}
// Dodanie do zmienna0 wartosci zmienna1
void add(unsigned long long int **zmienna0, unsigned long long int zmienna1[], int *indeks0, int *indeks1, int *rozmiar) {
    int i = 0;
    while (i <= *indeks1) {
        if (i > *indeks0) powiekszZmienna(zmienna0, indeks0, rozmiar);
        (*zmienna0)[i] += zmienna1[i];
        if ((*zmienna0)[i] > maksWartoscWTabeli) {
            (*zmienna0)[i] = (*zmienna0)[i] - maksWartoscWTabeli - 1;
            if (i == *indeks0) powiekszZmienna(zmienna0, indeks0, rozmiar);
            (*zmienna0)[i + 1] += 1;
        }
        ++i;
    }
    while (i <= *indeks0 && (*zmienna0)[i] > maksWartoscWTabeli) {
        if (i == *indeks0) powiekszZmienna(zmienna0, indeks0, rozmiar);
        ++(*zmienna0)[i + 1];
        ++i;
    }
}
// Wyzerowanie zmiennej
void reset(unsigned long long int **zmienna, int *indeks, int *rozmiar) {
    free(*zmienna);
    *rozmiar = 1;
    *indeks = 0;
    *zmienna = malloc(1 * sizeof(unsigned long long int));
    assert(*zmienna != NULL);
    (*zmienna)[0] = 0;
}
// Skok do instrukcji o danym adresie
void jump(int *aktAdres, int docAdres) {
    *aktAdres = docAdres;
}
// Wypisanie zmiennej
void wypiszZmienna(unsigned long long int zmienna[], int *indeks) {
    int i = *indeks;
    printf("%llu", zmienna[i]);
    --i;
    while (i >= 0) {
        if (zmienna[i] == 0) {
            for (int j = 0; j < 18; j++) {
                printf("0");
            }
        }
        else {
            unsigned long long int temp = 100000000000000000;
            while (temp > zmienna[i]) {
                printf("0");
                temp /= 10;
            }
            printf("%llu", zmienna[i]);
        }
        --i;
    }
    printf("\n");
}
// Skok do instrukcji lub zmniejsz zmienna
void djz(unsigned long long int **zmienna, int *aktAdres, int docAdres, int *indeks, int *rozmiar) {
    if (*indeks == 0 && (*zmienna)[0] == 0) {
        reset(zmienna, indeks, rozmiar);
        jump(aktAdres, docAdres);
    }
    else {
        int i = 0;
        while ((*zmienna)[i] == 0) {
            (*zmienna)[i] = maksWartoscWTabeli;
            ++i;
        }
        --(*zmienna)[i];
        if (i != 0 && i == (*indeks) && (*zmienna)[i] == 0) {
            --(*indeks);
        }
    }
}
// Instrukcja powtarzaj
void powtarzaj(wartosci_zmiennych zmienne[], char polecenie[], int *aktualnyIndeks, int dl_polecenia) {
    if (*aktualnyIndeks < dl_polecenia - 1) {
        int lewy = 1;
        int prawy = 0;
        int pocz = *aktualnyIndeks;
        int kon;
        char zmienna0 = polecenie[*aktualnyIndeks];
        int czy_optym = 1;
        
        while (lewy != prawy) {
            ++(*aktualnyIndeks);
            if (polecenie[*aktualnyIndeks] == '(') {
                ++lewy;
                czy_optym = 0;
            }
            else if (polecenie[*aktualnyIndeks] == ')') ++prawy;
            else if (polecenie[*aktualnyIndeks] == zmienna0) czy_optym = 0;
        }
        kon = *aktualnyIndeks;
        *aktualnyIndeks = pocz;
        
        // kod zopytmalizowany
        if (czy_optym) {
            ++(*aktualnyIndeks);
            while (*aktualnyIndeks < kon) {
                add(&zmienne[(int)polecenie[*aktualnyIndeks] - 97].wartosc, zmienne[(int)zmienna0 - 97].wartosc, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].ostatniIndeks, &zmienne[(int)zmienna0 - 97].ostatniIndeks, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].rozmiar);
                ++(*aktualnyIndeks);
            }
            reset(&zmienne[(int)zmienna0 - 97].wartosc, &zmienne[(int)zmienna0 - 97].ostatniIndeks, &zmienne[(int)zmienna0 - 97].rozmiar);
            ++(*aktualnyIndeks);
        }
        // najbardziej zagnieżdżona funkcja powtarzaj, ale nie można zoptymalizować
        else if (lewy == 1) {
            while (*aktualnyIndeks < kon) {
                djz(&zmienne[(int)polecenie[*aktualnyIndeks] - 97].wartosc, aktualnyIndeks, kon, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].ostatniIndeks, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].rozmiar);
                if (*aktualnyIndeks < kon) {
                    ++(*aktualnyIndeks);
                    while (*aktualnyIndeks < kon - 1) {
                        increment(&zmienne[(int)polecenie[*aktualnyIndeks] - 97].wartosc, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].ostatniIndeks, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].rozmiar);
                        ++(*aktualnyIndeks);
                    }
                    increment(&zmienne[(int)polecenie[*aktualnyIndeks] - 97].wartosc, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].ostatniIndeks, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].rozmiar);
                    jump(aktualnyIndeks, pocz);
                }
            }
            ++(*aktualnyIndeks);
        }
        // Występują głębsze zagnieżdżenia funkcji powtarzaj
        else {
            while (*aktualnyIndeks < kon) {
                djz(&zmienne[(int)polecenie[*aktualnyIndeks] - 97].wartosc, aktualnyIndeks, kon, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].ostatniIndeks, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].rozmiar);
                if (*aktualnyIndeks < kon) {
                    ++(*aktualnyIndeks);
                    while (*aktualnyIndeks < kon) {
                        if (polecenie[*aktualnyIndeks] >= 'a' && polecenie[*aktualnyIndeks] <= 'z') {
                            increment(&zmienne[(int)polecenie[*aktualnyIndeks] - 97].wartosc, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].ostatniIndeks, &zmienne[(int)polecenie[*aktualnyIndeks] - 97].rozmiar);
                            ++(*aktualnyIndeks);
                        }
                        else if (polecenie[*aktualnyIndeks] == '(') {
                            ++(*aktualnyIndeks);
                            powtarzaj(zmienne, polecenie, aktualnyIndeks, dl_polecenia);
                        }
                    }
                    jump(aktualnyIndeks, pocz);
                }
            }
            ++(*aktualnyIndeks);
        }
    }
}

int main(void) {
    // Inicjalizacja zmiennych
    wartosci_zmiennych zmienne[liczbaZmiennych];
    for (int i = 0; i < liczbaZmiennych; i++) {
        zmienne[i].nazwa = (char)(i + 97);
        zmienne[i].ostatniIndeks = 0;
        zmienne[i].wartosc = malloc(domyslnyRozmiar * sizeof(unsigned long long int));
        assert(zmienne[i].wartosc != NULL);
        zmienne[i].wartosc[0] = 0;
        zmienne[i].rozmiar = domyslnyRozmiar;
    }
    // Inicjalizacja polecenia
    char *wierszPolecenia;
    int rozmiarPolecenia;
    // Rozpoczęcie wykonywania poleceń
    int aktZnak = getchar();
    int aktIndeks;
    while (aktZnak != EOF) {
        // Wypisanie wartosci zmiennej
        if ((char)aktZnak == '=') {
            aktZnak = getchar();
            wypiszZmienna(zmienne[(int)aktZnak - 97].wartosc, &zmienne[(int)aktZnak - 97].ostatniIndeks);
            getchar();
        }
        // Wczytujemy linijke z poleceniem do dynamicznej zmiennej
        else {
            rozmiarPolecenia = domyslnyRozmiar;
            wierszPolecenia = (char*)malloc(domyslnyRozmiar * sizeof(char));
            aktIndeks = 0;
            while (aktZnak != '\n') {
                if (rozmiarPolecenia == aktIndeks) powiekszPolecenie(&wierszPolecenia, &rozmiarPolecenia);
                wierszPolecenia[aktIndeks] = (char)aktZnak;
                aktZnak = getchar();
                ++aktIndeks;
            }
            if (rozmiarPolecenia == aktIndeks) powiekszPolecenie(&wierszPolecenia, &rozmiarPolecenia);
            wierszPolecenia[aktIndeks] = '\0';
            // Wykonanie polecenia - linijki
            aktIndeks = 0;
            while (wierszPolecenia[aktIndeks] != '\0') {
                // obsluga zwiększ
                while (wierszPolecenia[aktIndeks] != '\0' && wierszPolecenia[aktIndeks] != '(') {
                    increment(&zmienne[(int)wierszPolecenia[aktIndeks] - 97].wartosc,  &zmienne[(int)wierszPolecenia[aktIndeks] - 97].ostatniIndeks, &zmienne[(int)wierszPolecenia[aktIndeks] - 97].rozmiar);
                    ++aktIndeks;
                }
                // obsluga powtarzaj
                if (wierszPolecenia[aktIndeks] == '(') {
                    ++aktIndeks; // pomijamy (
                    powtarzaj(zmienne, wierszPolecenia, &aktIndeks, rozmiarPolecenia);
                }
            }
            free(wierszPolecenia);
        }
        aktZnak = getchar();
    }
    
    // Zwolnienie pamięci
    for (int i = 0; i < liczbaZmiennych; i++) {
        free(zmienne[i].wartosc);
    }
    return 0;
}
