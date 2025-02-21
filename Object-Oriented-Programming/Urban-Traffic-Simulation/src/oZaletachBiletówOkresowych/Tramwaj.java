package oZaletachBiletówOkresowych;

public class Tramwaj extends Pojazd {
    private int numerBoczny;
    private int pojemność;
    private LiniaTramwajowa liniaTramwajowa;
    private boolean kierunekJazdy;
    // true oznacza jazdę po przystankach na linii,
    // gdy indeksy rosną (od początku do końca linii)
    // false oznacza jazdę, gdy indeksy maleją (od końca do początku linii)
    private int czasPierwszegoOdjazdu;
    private int liczbaPasażerówWTramwaju;
    private final Pasażer[] pasażerowieWTramwaju;

    public Tramwaj(int numerBoczny, int pojemność,
                   LiniaTramwajowa liniaTramwajowa, boolean kierunekJazdy) {
        this.numerBoczny = numerBoczny;
        this.pojemność = pojemność;
        this.liniaTramwajowa = liniaTramwajowa;
        this.kierunekJazdy = kierunekJazdy;
        liczbaPasażerówWTramwaju = 0;
        pasażerowieWTramwaju = new Pasażer[pojemność];
    }

    public void ustalCzasPierwszegoOdjazdu(int czasPierwszegoOdjazdu) {
        this.czasPierwszegoOdjazdu = czasPierwszegoOdjazdu;
    }

    public boolean czyJestMiejsce() {
        return liczbaPasażerówWTramwaju < pojemność;
    }

    public void dodajPasażera(Pasażer pasażer, Przystanek aktualnyPrzystanek) {
        int i = 0;
        while (pasażerowieWTramwaju[i] != null) {
            ++i;
        }
        pasażerowieWTramwaju[i] = pasażer;
        ++liczbaPasażerówWTramwaju;
        pasażer.ustalPrzystanekDocelowy(
                liniaTramwajowa.losowyPrzystanekPozostałyNaTrasie(
                        aktualnyPrzystanek, kierunekJazdy));
    }

    public void wysadźPasażera(Pasażer pasażer) {
        int i = 0;
        while (pasażerowieWTramwaju[i] != pasażer) {
            ++i;
        }
        pasażerowieWTramwaju[i] = null;
        --liczbaPasażerówWTramwaju;
    }

    public void zmieńKierunek() {
        kierunekJazdy = !kierunekJazdy;
    }

    public void ustalKierunek(boolean kierunekJazdy) {
        this.kierunekJazdy = kierunekJazdy;
    }

    public boolean getKierunekJazdy(){
        return kierunekJazdy;
    }

    public Pasażer[] getPasażerowieWTramwaju() {
        return pasażerowieWTramwaju;
    }

    public int getCzasPierwszegoOdjazdu() {
        return czasPierwszegoOdjazdu;
    }

    public LiniaTramwajowa getLiniaTramwajowa() {
        return liniaTramwajowa;
    }

    public int getNumerBoczny() {
        return numerBoczny;
    }

    public void wyczyśćTramwaj() {
        for (Pasażer pasażer : pasażerowieWTramwaju) {
            if (pasażer != null) {
                wysadźPasażera(pasażer);
            }
        }
    }

    @Override
    public String toString() {
        return String.valueOf(numerBoczny);
    }
}
