package oZaletachBiletówOkresowych;

public class Przystanek {

    private final String nazwa;
    private final int pojemność;
    int liczbaPasażerówNaPrzystanku;
    private final Pasażer[] pasażerowieNaPrzystanku;

    public Przystanek(String nazwa, int pojemność) {
        this.nazwa = nazwa;
        this.pojemność = pojemność;
        liczbaPasażerówNaPrzystanku = 0;
        pasażerowieNaPrzystanku = new Pasażer[pojemność];
    }

    public boolean czyJestMiejsce() {
        return liczbaPasażerówNaPrzystanku < pojemność;
    }

    public Pasażer[] getPasażerowieNaPrzystanku() {
        return pasażerowieNaPrzystanku;
    }

    public void dodajPasażera(Pasażer pasażer) {
        int i = 0;
        while (pasażerowieNaPrzystanku[i] != null) {
            ++i;
        }
        pasażerowieNaPrzystanku[i] = pasażer;
        ++liczbaPasażerówNaPrzystanku;
    }
    public void usuńPasażera(Pasażer pasażer) {
        int i = 0;
        while (i < pasażerowieNaPrzystanku.length
                && pasażerowieNaPrzystanku[i] != pasażer) {
            ++i;
        }
        if (i != pasażerowieNaPrzystanku.length) {
            pasażerowieNaPrzystanku[i] = null;
            --liczbaPasażerówNaPrzystanku;
        }
    }

    public void wyczyśćPrzystanek() {
        for (int i = 0; i < pojemność; ++i) {
            if (pasażerowieNaPrzystanku[i] != null) {
                pasażerowieNaPrzystanku[i].ustawCzasZakończeniaCzekania(24
                        * 60);
                pasażerowieNaPrzystanku[i].dodajCzasCzekania();
                usuńPasażera(pasażerowieNaPrzystanku[i]);
            }
        }
    }

    public String getNazwa() {
        return nazwa;
    }

    @Override
    public String toString() {
        return nazwa;
    }
}

