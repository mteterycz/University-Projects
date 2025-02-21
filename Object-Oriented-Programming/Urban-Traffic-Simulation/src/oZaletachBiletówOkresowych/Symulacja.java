package oZaletachBiletówOkresowych;

import java.util.Objects;

public class Symulacja {
    private int liczbaDniSymulacji;
    private int pojemnośćPrzystanku;
    private int liczbaPrzystanków;
    private Przystanek[] przystanki;
    private int liczbaPasażerów;
    private Pasażer[] pasażerowie;
    private int pojemnośćTramwaju;
    private int liczbaLiniiTramwajowych;
    private LiniaTramwajowa[] linieTramwajowe;

    private void wczytanieDanych() {
        CzytnikDanych czytnikDanych = new CzytnikDanych();
        String[] dane = czytnikDanych.czytajDane();

        int i = 0;

        liczbaDniSymulacji = Integer.parseInt(dane[i++]);

        pojemnośćPrzystanku = Integer.parseInt(dane[i++]);
        liczbaPrzystanków = Integer.parseInt(dane[i++]);
        przystanki = new Przystanek[liczbaPrzystanków];
        for (int j = 0; j < liczbaPrzystanków; ++j) {
            przystanki[j] = new Przystanek(dane[i++], pojemnośćPrzystanku);
        }

        liczbaPasażerów = Integer.parseInt(dane[i++]);
        pasażerowie = new Pasażer[liczbaPasażerów];
        for (int j = 0; j < liczbaPasażerów; ++j) {
            int wylosowanyPrzystanek = Losowanie.losuj(0,
                    liczbaPrzystanków - 1);
            pasażerowie[j] = new Pasażer(j, przystanki[wylosowanyPrzystanek]);
        }

        pojemnośćTramwaju = Integer.parseInt(dane[i++]);
        liczbaLiniiTramwajowych = Integer.parseInt(dane[i++]);
        linieTramwajowe = new LiniaTramwajowa[liczbaLiniiTramwajowych];
        int numerTramwaju = 0;

        for (int j = 0; j < liczbaLiniiTramwajowych; ++j) {
            linieTramwajowe[j] = new LiniaTramwajowa(j);
            int liczbaTramwajówNaLinii = Integer.parseInt(dane[i++]);
            for (int k = 0; k < liczbaTramwajówNaLinii; k++) {
                boolean kierunekJazdy = k % 2 == 0;
                Tramwaj nowyTramwaj = new Tramwaj(numerTramwaju++,
                        pojemnośćTramwaju, linieTramwajowe[j], kierunekJazdy);
                linieTramwajowe[j].dodajTramwaj(nowyTramwaj);
            }
            int długośćLinii = Integer.parseInt(dane[i++]);
            int całkowityCzasPrzejazdu = 0;
            for (int k = 0; k < długośćLinii; k++) {
                String nazwaPrzystanku = dane[i++];
                int czasDojazdu = Integer.parseInt(dane[i++]);
                całkowityCzasPrzejazdu += czasDojazdu;
                int temp = 0;
                while (!Objects.equals(nazwaPrzystanku,
                        przystanki[temp].getNazwa())) {
                    ++temp;
                }
                linieTramwajowe[j].dodajPrzystanek(przystanki[temp],
                        czasDojazdu);
            }
            linieTramwajowe[j].
                    ustalCzasPrzejazduCałegoKółkaICzasyPierwszychOdjazdów(
                            2 * całkowityCzasPrzejazdu);
        }
    }

    private void wypiszDane() {
        System.out.println("Dane:");
        System.out.println("Liczba dni symulacji: " + liczbaDniSymulacji);
        System.out.println("Pojemność przystanku: " + pojemnośćPrzystanku);
        System.out.println("Liczba przystanków: " + liczbaPrzystanków);
        for (Przystanek przystanek : przystanki) {
            System.out.println(przystanek);
        }
        System.out.println("Liczba pasażerów: " + liczbaPasażerów);
        System.out.println("Pojemność tramwaju: " + pojemnośćTramwaju);
        System.out.println("Liczba linii tramwajowych: " +
                liczbaLiniiTramwajowych);
        for (LiniaTramwajowa liniaTramwajowa : linieTramwajowe) {
            System.out.println(liniaTramwajowa);
        }
    }


    public void symulacja() {
        wczytanieDanych();
        wypiszDane();
        int[] liczbaPrzejazdówDlaDnia = new int[liczbaDniSymulacji];
        int[] łącznyCzasOczekiwaniaDlaDnia = new int[liczbaDniSymulacji];
        Kolejka kolejka = new KolejkaTablica();
        for (int d = 0; d < liczbaDniSymulacji; ++d) {
            // Początek dnia
            for (int i = 0; i < liczbaPasażerów; ++i) {
                int czasPrzybyciaNaPrzystanek =
                        pasażerowie[i].czasPrzybyciaNaPrzystanek();
                kolejka.dodaj(new PrzyjściePasażeraNaPrzystanek(d,
                        czasPrzybyciaNaPrzystanek, pasażerowie[i],
                        pasażerowie[i].getPobliskiPrzystanek()));
            }
            for (int i = 0; i < liczbaLiniiTramwajowych; ++i) {
                for (Tramwaj tramwaj : linieTramwajowe[i].getTramwaje()) {
                    int czas = tramwaj.getCzasPierwszegoOdjazdu();
                    Przystanek przystanek =
                            linieTramwajowe[i].przystanekPoczątkowy(tramwaj);
                    kolejka.dodaj(new PrzyjazdTramwajuNaPrzystanek(d, czas,
                            przystanek, tramwaj, linieTramwajowe[i]));
                }
            }
            // Realizacja symulacji
            while (!kolejka.czyPusta()) {
                Zdarzenie aktualneZdarzenie = kolejka.zwróćPierwszy();
                Zdarzenie[] noweZdarzenia =
                        aktualneZdarzenie.następneZdarzenia();
                for (Zdarzenie zdarzenie : noweZdarzenia) {
                    kolejka.dodaj(zdarzenie);
                }
                liczbaPrzejazdówDlaDnia[d] +=
                        aktualneZdarzenie.wykonajZdarzenie();
            }

            // Koniec dnia, godzina 24:00
            for (LiniaTramwajowa liniaTramwajowa : linieTramwajowe) {
                for(Tramwaj tramwaj : liniaTramwajowa.getTramwaje()) {
                    tramwaj.wyczyśćTramwaj();
                }
                liniaTramwajowa.ustalPoczątkoweKierunkiTramwajów();
            }
            for (Przystanek przystanek : przystanki) {
                przystanek.wyczyśćPrzystanek();
            }
            for (Pasażer pasażer : pasażerowie) {
                łącznyCzasOczekiwaniaDlaDnia[d] +=
                        pasażer.zwróćŁącznyCzasIWyzerujZmienną();
            }
        }

        int sumaPrzejazdów = 0;
        int sumaCzasuOczekiwania = 0;
        for (int i = 0; i < liczbaDniSymulacji; ++i) {
            sumaPrzejazdów += liczbaPrzejazdówDlaDnia[i];
            sumaCzasuOczekiwania += łącznyCzasOczekiwaniaDlaDnia[i];
            System.out.println("Dzień " + (i + 1) +
                    ": liczba przejazdów - " + liczbaPrzejazdówDlaDnia[i]
                    + ", łączny czas oczekiwania - " +
                    łącznyCzasOczekiwaniaDlaDnia[i] + " minut");
        }
        System.out.println("Łączna liczba przejazdów: " + sumaPrzejazdów);
        System.out.println("Łączny czas oczekiwania: " + sumaCzasuOczekiwania
                + " minut");
        System.out.print("Średni czas oczekiwania: ");
        System.out.format("%.2f minut",
                (double)sumaCzasuOczekiwania / sumaPrzejazdów);
    }
}
