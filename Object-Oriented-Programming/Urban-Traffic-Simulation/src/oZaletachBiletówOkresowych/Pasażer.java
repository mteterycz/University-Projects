package oZaletachBiletówOkresowych;

public class Pasażer {
    private final int indeksPasażera;
    private final Przystanek pobliskiPrzystanek;
    private Przystanek docelowyPrzystanek;
    private int łącznyCzasCzekania;
    private int czasRozpoczęciaCzekania;
    private int czasZakończeniaCzekania;

    public Pasażer(int indeksPasażera, Przystanek pobliskiPrzystanek) {
        this.indeksPasażera = indeksPasażera;
        this.pobliskiPrzystanek = pobliskiPrzystanek;
        łącznyCzasCzekania = 0;
    }

    public void ustawCzasRozpoczęciaCzekania(int czasRozpoczęciaCzekania) {
        this.czasRozpoczęciaCzekania = czasRozpoczęciaCzekania;
    }

    public void ustawCzasZakończeniaCzekania(int czasZakończeniaCzekania) {
        this.czasZakończeniaCzekania = czasZakończeniaCzekania;
    }

    public void dodajCzasCzekania(){
        łącznyCzasCzekania += (czasZakończeniaCzekania
                - czasRozpoczęciaCzekania);
    }

    public int zwróćŁącznyCzasIWyzerujZmienną() {
        int temp = łącznyCzasCzekania;
        łącznyCzasCzekania = 0;
        return temp;
    }

    public int czasPrzybyciaNaPrzystanek() {
        return Losowanie.losuj(360, 720);
    }

    public void ustalPrzystanekDocelowy(Przystanek docelowyPrzystanek) {
        this.docelowyPrzystanek = docelowyPrzystanek;
    }

    public Przystanek getDocelowyPrzystanek() {
        return docelowyPrzystanek;
    }

    public Przystanek getPobliskiPrzystanek() {
        return pobliskiPrzystanek;
    }

    public int getIndeksPasażera() {
        return indeksPasażera;
    }

    @Override
    public String toString() {
        // Metoda niepotrzebna, ale była przydatna do testowania
        return indeksPasażera + " " + pobliskiPrzystanek;
    }
}
