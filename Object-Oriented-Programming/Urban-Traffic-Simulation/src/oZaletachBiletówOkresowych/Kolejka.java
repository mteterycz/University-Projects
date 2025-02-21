package oZaletachBiletówOkresowych;

public interface Kolejka {
    void dodaj(Zdarzenie zdarzenie);
    Zdarzenie zwróćPierwszy();
    boolean czyPusta();
}
