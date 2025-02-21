package oZaletachBiletówOkresowych;

public abstract class Zdarzenie {

    private final int dzieńZdarzenia;
    private final int czasZdarzenia;

    public Zdarzenie(int dzieńZdarzenia, int czasZdarzenia) {
        this.dzieńZdarzenia = dzieńZdarzenia;
        this.czasZdarzenia = czasZdarzenia;
    }

    public abstract int wykonajZdarzenie();

    public abstract Zdarzenie[] następneZdarzenia();

    public int getCzas() {
        return 24 * 60 * dzieńZdarzenia + czasZdarzenia;
    }

    public int getDzieńZdarzenia(){
        return dzieńZdarzenia;
    }

    public int getCzasZdarzenia() {
        return czasZdarzenia;
    }
}
