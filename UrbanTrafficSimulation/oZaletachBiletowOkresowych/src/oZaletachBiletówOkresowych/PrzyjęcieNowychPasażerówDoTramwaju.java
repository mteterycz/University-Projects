package oZaletachBiletówOkresowych;

import java.util.Arrays;

public class PrzyjęcieNowychPasażerówDoTramwaju extends Zdarzenie {

    private final Przystanek przystanek;
    private final Tramwaj tramwaj;


    public PrzyjęcieNowychPasażerówDoTramwaju(int dzieńZdarzenia,
                                              int czasZdarzenia,
                                              Przystanek przystanek,
                                              Tramwaj tramwaj) {
        super(dzieńZdarzenia, czasZdarzenia);
        this.przystanek = przystanek;
        this.tramwaj = tramwaj;
    }
    @Override
    public int wykonajZdarzenie() {
        return 0;
    }

    @Override
    public Zdarzenie[] następneZdarzenia() {
        Zdarzenie[] noweZdarzenia = new Zdarzenie[0];
        int i = 0;
        for (int j = 0; j < przystanek.getPasażerowieNaPrzystanku().length;
             ++j) {
                if (i == noweZdarzenia.length) {
                    if (i == 0) {
                        noweZdarzenia = Arrays.copyOf(noweZdarzenia, 1);
                    } else {
                        noweZdarzenia = Arrays.copyOf(noweZdarzenia,
                                2 * noweZdarzenia.length);
                    }
                }
                noweZdarzenia[i] = new WejściePasażeraDoTramwaju(
                        getDzieńZdarzenia(), getCzasZdarzenia(),
                        przystanek.getPasażerowieNaPrzystanku()[j],
                        tramwaj, przystanek);
                ++i;
            }
        return Arrays.copyOf(noweZdarzenia, i);
    }
}
