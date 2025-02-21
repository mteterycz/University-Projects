package oZaletachBiletówOkresowych;

public class WyjściePasażeraZTramwaju extends Zdarzenie {

    private final Pasażer pasażer;
    private final Tramwaj tramwaj;
    private final Przystanek przystanek;

    public WyjściePasażeraZTramwaju(int dzieńZdarzenia, int czasZdarzenia,
                                    Pasażer pasażer, Tramwaj tramwaj,
                                    Przystanek przystanek) {
        super(dzieńZdarzenia, czasZdarzenia);
        this.pasażer = pasażer;
        this.tramwaj = tramwaj;
        this.przystanek = przystanek;
    }

    @Override
    public int wykonajZdarzenie() {
        if (przystanek.czyJestMiejsce()) {
            tramwaj.wysadźPasażera(pasażer);
            przystanek.dodajPasażera(pasażer);
            pasażer.ustawCzasRozpoczęciaCzekania(getCzasZdarzenia());
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
            sb.append(": Pasażer ").append(pasażer.getIndeksPasażera());
            sb.append(" wysiadł z tramwaju linii ");
            sb.append(tramwaj.getLiniaTramwajowa().getNumerLinii());
            sb.append(" (nr bocz. ").append(tramwaj.getNumerBoczny());
            sb.append(")").append(" na przystanku ");
            sb.append(przystanek.getNazwa()).append(".");
            System.out.println(sb);
        }
        return 0;
    }

    @Override
    public Zdarzenie[] następneZdarzenia() {
        return new Zdarzenie[0];
    }
}
