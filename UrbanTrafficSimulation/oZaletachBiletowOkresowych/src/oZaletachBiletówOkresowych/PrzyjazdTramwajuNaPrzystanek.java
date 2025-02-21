package oZaletachBiletówOkresowych;

import java.util.Arrays;

public class PrzyjazdTramwajuNaPrzystanek extends Zdarzenie {

    private final Przystanek przystanek;
    private final Tramwaj tramwaj;
    private final LiniaTramwajowa liniaTramwajowa;
    public PrzyjazdTramwajuNaPrzystanek(int dzieńZdarzenia, int czasZdarzenia,
                                        Przystanek przystanek, Tramwaj tramwaj,
                                        LiniaTramwajowa liniaTramwajowa) {
        super(dzieńZdarzenia, czasZdarzenia);
        this.przystanek = przystanek;
        this.tramwaj = tramwaj;
        this.liniaTramwajowa = liniaTramwajowa;
    }

    private PrzyjazdTramwajuNaPrzystanek przyjazdNaNastępnyPrzystanek() {
        int dzień = getDzieńZdarzenia();
        int czas = getCzasZdarzenia() +
                liniaTramwajowa.czasDojazduNaNastępnyPrzystanek(przystanek,
                        tramwaj.getKierunekJazdy());
        Przystanek następnyPrzystanek =
                liniaTramwajowa.następnyPrzystanek(przystanek,
                        tramwaj.getKierunekJazdy());
        return new PrzyjazdTramwajuNaPrzystanek(dzień, czas,
                następnyPrzystanek, tramwaj, liniaTramwajowa);
    }

    private PrzyjazdNaPętle przyjazdNaPętle() {
        int dzień = getDzieńZdarzenia();
        int czas = getCzasZdarzenia();
        return new PrzyjazdNaPętle(dzień, czas, przystanek, tramwaj,
                liniaTramwajowa);
    }

    private PrzyjęcieNowychPasażerówDoTramwaju
            przyjęcieNowychPasażerówDoTramwaju() {
        int dzień = getDzieńZdarzenia();
        int czas = getCzasZdarzenia();
        return new PrzyjęcieNowychPasażerówDoTramwaju(dzień, czas,
                przystanek, tramwaj);
    }

    @Override
    public Zdarzenie[] następneZdarzenia() {
        Zdarzenie[] noweZdarzenia = new Zdarzenie[1];
        int i = 0;
        // Ustalenie dalszej trasy tramwaju
        if (!liniaTramwajowa.czyPostójNaPętli(przystanek,
                tramwaj.getKierunekJazdy())) {
            noweZdarzenia[i] = przyjazdNaNastępnyPrzystanek();
            ++i;
        }
        // Wysadzanie pasażerów
        for (Pasażer pasażer : tramwaj.getPasażerowieWTramwaju()) {
            if (pasażer != null
                    && przystanek == pasażer.getDocelowyPrzystanek()
                    && przystanek.czyJestMiejsce()) {
                if (i == noweZdarzenia.length) {
                    noweZdarzenia = Arrays.copyOf(noweZdarzenia,
                            2 * noweZdarzenia.length);
                }
                noweZdarzenia[i] = new WyjściePasażeraZTramwaju(
                        getDzieńZdarzenia(), getCzasZdarzenia(), pasażer,
                        tramwaj, przystanek);
                ++i;
            }
        }
        if (!liniaTramwajowa.czyPostójNaPętli(przystanek,
                tramwaj.getKierunekJazdy())) {
            if (i == noweZdarzenia.length) {
                noweZdarzenia = Arrays.copyOf(noweZdarzenia,
                        2 * noweZdarzenia.length);
            }
            noweZdarzenia[i] = przyjęcieNowychPasażerówDoTramwaju();
            ++i;
        } else {
            if (i == noweZdarzenia.length) {
                noweZdarzenia = Arrays.copyOf(noweZdarzenia,
                        2 * noweZdarzenia.length);
            }
            noweZdarzenia[i] = przyjazdNaPętle();
            ++i;
        }
        return Arrays.copyOf(noweZdarzenia, i);
    }

    @Override
    public int wykonajZdarzenie() {
        StringBuilder sb = new StringBuilder();
        sb.append(getDzieńZdarzenia() + 1).append(", ");
        if (getCzasZdarzenia() / 60 < 10) {
            sb.append(0);
        }
        sb.append(getCzasZdarzenia() / 60).append(":");
        if (getCzasZdarzenia() % 60 < 10) {
            sb.append(0);
        }
        sb.append(getCzasZdarzenia() % 60);
        sb.append(": Tramwaj (nr. bocz. ").append(tramwaj.getNumerBoczny());
        sb.append(") linii ").append(liniaTramwajowa.getNumerLinii());
        sb.append(" przyjechał na przystanek ").append(przystanek.getNazwa());
        sb.append(".");
        System.out.println(sb);
        return 0;
    }
}
