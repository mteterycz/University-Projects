package oZaletachBiletówOkresowych;

import java.util.Arrays;

public class LiniaTramwajowa {
    private final int numerLinii;
    private int liczbaTramwajów;
    private Tramwaj[] tramwaje;
    private int długośćLinii;
    private Przystanek[] przystankiNaLinii;
    private int[] czasyDojazdów;
    private int czasPrzejazduCałegoKółka;

    public LiniaTramwajowa (int numerLinii) {
        this.numerLinii = numerLinii;
        liczbaTramwajów = 0;
        tramwaje = new Tramwaj[0];
        długośćLinii = 0;
        przystankiNaLinii = new Przystanek[0];
        czasyDojazdów = new int[0];
    }

    public void dodajTramwaj(Tramwaj tramwaj) {
        if (liczbaTramwajów == tramwaje.length) {
            tramwaje = Arrays.copyOf(tramwaje, 3 * tramwaje.length / 2 + 2);
        }
        tramwaje[liczbaTramwajów] = tramwaj;
        ++liczbaTramwajów;
    }

    public void dodajPrzystanek(Przystanek przystanek, int czasPrzejazdu) {
        if (długośćLinii == przystankiNaLinii.length) {
            czasyDojazdów = Arrays.copyOf(czasyDojazdów,
                    3 * przystankiNaLinii.length / 2 + 2);
            przystankiNaLinii = Arrays.copyOf(przystankiNaLinii,
                    3 * przystankiNaLinii.length / 2 + 2);
        }
        przystankiNaLinii[długośćLinii] = przystanek;
        czasyDojazdów[długośćLinii] = czasPrzejazdu;
        ++długośćLinii;
    }

    public void ustalCzasPrzejazduCałegoKółkaICzasyPierwszychOdjazdów(
            int czasPrzejazduCałegoKółka) {
        this.czasPrzejazduCałegoKółka = czasPrzejazduCałegoKółka;
        tramwaje = Arrays.copyOf(tramwaje, liczbaTramwajów);
        for (int i = 0; i < tramwaje.length; ++i) {
            int temp = i / 2;
            int czasPierwszegoOdjazdu = 360 +
                    (czasPrzejazduCałegoKółka / tramwaje.length) * temp;
            tramwaje[i].ustalCzasPierwszegoOdjazdu(czasPierwszegoOdjazdu);
        }
    }

    public void ustalPoczątkoweKierunkiTramwajów() {
        for (int k = 0; k < liczbaTramwajów; k++) {
            boolean kierunekJazdy = k % 2 == 0;
            tramwaje[k].ustalKierunek(kierunekJazdy);
        }
    }

    public Przystanek losowyPrzystanekPozostałyNaTrasie(
            Przystanek aktualnyPrzystanek, boolean kierunekJazdy) {
        int i = 0;
        while (przystankiNaLinii[i] != aktualnyPrzystanek) {
            ++i;
        }
        int indeksPrzystanku;
        if (kierunekJazdy) {
            indeksPrzystanku = Losowanie.losuj(i + 1, długośćLinii - 1);
        } else {
            indeksPrzystanku = Losowanie.losuj(0, i - 1);
        }
        return przystankiNaLinii[indeksPrzystanku];
    }

    public boolean czyPostójNaPętli(Przystanek aktualnyPrzystanek,
                                    boolean kierunekJazdy) {
        int i = 0;
        while (przystankiNaLinii[i] != aktualnyPrzystanek) {
            ++i;
        }
        if (kierunekJazdy) {
            return i + 1 == długośćLinii;
        } else {
            return i == 0;
        }
    }

    public int czasPostojuNaPętli() {
        return czasyDojazdów[długośćLinii - 1];
    }

    public Przystanek następnyPrzystanek(Przystanek aktualnyPrzystanek,
                                         boolean kierunekJazdy) {
        int i = 0;
        while (przystankiNaLinii[i] != aktualnyPrzystanek) {
            ++i;
        }
        if (kierunekJazdy) {
            return przystankiNaLinii[i + 1];
        } else {
            return przystankiNaLinii[i - 1];
        }
    }

    public int czasDojazduNaNastępnyPrzystanek(Przystanek aktualnyPrzystanek,
                                               boolean kierunekJazdy) {
        int i = 0;
        while (przystankiNaLinii[i] != aktualnyPrzystanek) {
            ++i;
        }
        if (kierunekJazdy) {
            return czasyDojazdów[i];
        } else {
            return czasyDojazdów[i - 1];
        }
    }

    public Tramwaj[] getTramwaje() {
        return tramwaje;
    }

    public int getNumerLinii() {
        return numerLinii;
    }

    public Przystanek przystanekPoczątkowy(Tramwaj tramwaj) {
        if (tramwaj.getKierunekJazdy()) {
            return przystankiNaLinii[0];
        } else {
            return przystankiNaLinii[długośćLinii - 1];
        }
    }

    @Override
    public String toString() {
        // Metoda niepotrzebna, ale była przydatna do testowania
        StringBuilder str = new StringBuilder();
        str.append("Linia numer ").append(numerLinii).append(": ");
        str.append("liczba tramwajów - ").append(liczbaTramwajów);
        str.append(", długość trasy - ").append(długośćLinii);
        str.append(", przystanki - [");
        str.append(przystankiNaLinii[0].toString()).append(" (");
        str.append(czasyDojazdów[0]).append(")");
        for (int i = 1; i < długośćLinii; ++i) {
            str.append(", ");
            str.append(przystankiNaLinii[i].toString()).append(" (");
            str.append(czasyDojazdów[i]).append(")");
        }
        str.append("]");
        return str.toString();
    }

}
