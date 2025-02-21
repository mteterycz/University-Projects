package oZaletachBiletówOkresowych;

import java.util.Arrays;

public class KolejkaTablica implements Kolejka {

    private Zdarzenie[] zdarzenia;
    private int liczbaZdarzeń;
    private int aktualnyIndeksZdarzenia;

    public KolejkaTablica() {
        zdarzenia = new Zdarzenie[0];
        liczbaZdarzeń = 0;
        aktualnyIndeksZdarzenia = 0;
    }

    @Override
    public void dodaj(Zdarzenie zdarzenie) {
        if (liczbaZdarzeń == zdarzenia.length) {
            if (liczbaZdarzeń == 0) {
                zdarzenia = Arrays.copyOf(zdarzenia, 2);
            } else {
                zdarzenia = Arrays.copyOf(zdarzenia, 2 * zdarzenia.length);
            }
        }
        int i = liczbaZdarzeń - 1;
        while (i >= 0 && zdarzenia[i].getCzas() > zdarzenie.getCzas()) {
            zdarzenia[i + 1] = zdarzenia[i];
            --i;
        }
        zdarzenia[i + 1] = zdarzenie;
        ++liczbaZdarzeń;
    }
    @Override
    public Zdarzenie zwróćPierwszy() {
        assert (aktualnyIndeksZdarzenia < liczbaZdarzeń) : "Kolejka jest pusta";
        return zdarzenia[aktualnyIndeksZdarzenia++];
    }

    public boolean czyPusta() {
        return aktualnyIndeksZdarzenia >= liczbaZdarzeń;
    }
}
