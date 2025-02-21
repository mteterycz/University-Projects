/**
 * "L-system"
 *
 * Program zaliczeniowy 2 do zajęć labolatoryjnych ze Wstępu do programowania.
 *
 * Kompilacja poleceniem:
 * gcc @opcje main.c -o lsystem
 *
 * Program wypisuje prolog, ciąg wierszy będący interpretacją słowa, epilog.
 *
 * Program:
 * 1. Wczytuje nieujemną liczbę całkowitą określającą długość wyprowadzenia słowa;
 * 2. Wczytuje wiersz zawierający aksjomat;
 * 3. Wczytuje ciąg wierszy określający reguły zastępowania;
 * 4. Wczytuje ciąg wierszy określający prolog i oddaje na wyjście prolog;
 * 5. Wczytuje pusty wiersz;
 * 6. Wczytuje ciąg wierszy określający reguły interpretacji;
 * 7. Wczytuje pusty wiersz
 * 8. Oddaje na wyjście ciąg wierszy określający interpretacje słowa.
 * 9. Wczytuje ciąg wierszy określający epilog i oddaje na wyjście epilog.
 *
 * autor: Maciej Teterycz <mt459557@students.mimuw.edu.pl>
 * wersja: 1.0
 * data: 12 grudnia 2023
 */

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGHT_LINE 101

char* kolejne_wyprowadzenie(char *akt_wyprowadzenie, char reguly[][MAX_LENGHT_LINE], int *dl_akt_wyprowadzenia) {
    char *temp = (char *)malloc((unsigned long int)(*dl_akt_wyprowadzenia) * sizeof(char));
    if (temp == NULL) exit(1);
    int i = 0;
    char *pomoc;
    int akt_dl = 0;
    int j;
    while (akt_wyprowadzenie[i] != '\0') {
        if (reguly[(int)akt_wyprowadzenie[i]][0] == akt_wyprowadzenie[i]) {
            j = 1;
            while (reguly[(int)akt_wyprowadzenie[i]][j] != '\0') {
                if (akt_dl == *dl_akt_wyprowadzenia){
                    pomoc = temp;
                    *dl_akt_wyprowadzenia *= 2;
                    temp = realloc(temp, ((unsigned long int)(*dl_akt_wyprowadzenia)) * sizeof(char));
                    if (temp == NULL) {
                        free(pomoc);
                        exit(1);
                    }
                }
                temp[akt_dl] = reguly[(int)akt_wyprowadzenie[i]][j];
                akt_dl++;
                j++;
            }
        }
        else {
            if (akt_dl == *dl_akt_wyprowadzenia) {
                pomoc = temp;
                *dl_akt_wyprowadzenia *= 2;
                temp = realloc(temp, ((unsigned long int)(*dl_akt_wyprowadzenia)) * sizeof(char));
                if (temp == NULL) {
                    free(pomoc);
                    exit(1);
                }
            }
            temp[akt_dl] = akt_wyprowadzenie[i];
            akt_dl++;
        }
        i++;
    }
    temp[akt_dl] = '\0';
    pomoc = akt_wyprowadzenie;
    akt_wyprowadzenie = temp;
    temp = pomoc;
    free(temp);
    return akt_wyprowadzenie;
}

char* n_wyprowadzenie(int n, char *aksjomat, char reguly[][MAX_LENGHT_LINE], int *dl_akt_wyprowadzenia) {
    if (n == 0) return aksjomat;
    else if (n == 1) return kolejne_wyprowadzenie(aksjomat, reguly, dl_akt_wyprowadzenia);
    else return n_wyprowadzenie(n-1, kolejne_wyprowadzenie(aksjomat, reguly, dl_akt_wyprowadzenia), reguly, dl_akt_wyprowadzenia);
}

void wczytanie_i_przepisanie_linijki(void) {
    // Wczytanie
    char linijka[MAX_LENGHT_LINE];
    int znak = getchar();
    int p = 0;
    while (znak != '\n') {
        linijka[p] = (char)znak;
        znak = getchar();
        p++;
    }
    linijka[p] = '\n';
    // Wypisanie
    p = 0;
    while (linijka[p] != '\n') {
        printf("%c", linijka[p]);
        p++;
    }
    printf("%c", linijka[p]);
}

void wczytanie_danych(char zapis[][MAX_LENGHT_LINE]) {
    for (int k = 0; k <= CHAR_MAX; k++) {
        zapis[k][0] = '\0';
    }
    int i;
    int akt_znak;
    int symbol = getchar();
    while (symbol != '\n') {
        zapis[symbol][0] = (char)symbol;
        akt_znak = getchar();
        i = 1;
        while (akt_znak != '\n') {
            zapis[symbol][i] = (char)akt_znak;
            akt_znak = getchar();
            i++;
        }
        zapis[symbol][i] = '\0';
        symbol = getchar();
    }
}

int main(void) {
    // Wczytanie długości wyprowadzenia.
    int N;
    scanf("%d", &N);
    getchar();
    
    char *aksjomat = (char *)malloc(MAX_LENGHT_LINE * sizeof(char));
    int symbol;
    int akt_znak;
    int i = 0;
    
    // Wczytanie aksjomatu
    akt_znak = getchar();
    while (akt_znak != '\n') {
        aksjomat[i] = (char)akt_znak;
        akt_znak = getchar();
        i++;
    }
    aksjomat[i] = '\0';
    
    // Wczytanie reguł
    char reguly[CHAR_MAX + 1][MAX_LENGHT_LINE];
    wczytanie_danych(reguly);
    
    // Przepisane prologu
    symbol = getchar();
    while(symbol != '\n') {
        ungetc(symbol, stdin);
        wczytanie_i_przepisanie_linijki();
        symbol = getchar();
    }
    
    // Wczytanie słownika
    char slownik[CHAR_MAX + 1][MAX_LENGHT_LINE];
    wczytanie_danych(slownik);
    
    // Zapis n_wyprowadzenia w tablicy, dlug oznacza poczatkową długosc wyprowadzenia
    int dlug = 2;
    char *wypro = n_wyprowadzenie(N, aksjomat, reguly, &dlug);
    
    i = 0;
    int j;
    
    // Wypisanie interpretacji słowa
    while (wypro[i] != '\0') {
        j = 1;
        if (wypro[i] == slownik[(int)wypro[i]][0]) {
            while (slownik[(int)wypro[i]][j] != '\0') {
                printf("%c", slownik[(int)wypro[i]][j]);
                j++;
            }
            i++;
            printf("\n");
        }
        else i++;
    }
    // Przepisanie epilogu
    symbol = getchar();
    while(symbol != '\n' && symbol != EOF) {
        ungetc(symbol, stdin);
        wczytanie_i_przepisanie_linijki();
        symbol = getchar();
    }
    free(wypro);
    return 0;
}
