package oZaletachBiletówOkresowych;

public class WejściePasażeraDoTramwaju extends Zdarzenie {
    private final Pasażer pasażer;
    private final Tramwaj tramwaj;
    private final Przystanek przystanek;

    public WejściePasażeraDoTramwaju(int dzieńZdarzenia, int czasZdarzenia,
                                     Pasażer pasażer, Tramwaj tramwaj,
                                     Przystanek przystanek) {
        super(dzieńZdarzenia, czasZdarzenia);
        this.pasażer = pasażer;
        this.tramwaj = tramwaj;
        this.przystanek = przystanek;
    }
    @Override
    public int wykonajZdarzenie() {
        if (pasażer != null) {
            if (tramwaj.czyJestMiejsce()) {
                tramwaj.dodajPasażera(pasażer, przystanek);
                przystanek.usuńPasażera(pasażer);
                pasażer.ustawCzasZakończeniaCzekania(getCzasZdarzenia());
                pasażer.dodajCzasCzekania();
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
                sb.append(" wsiadł do tramwaju linii ");
                sb.append(tramwaj.getLiniaTramwajowa().getNumerLinii());
                sb.append(" (nr bocz. ").append(tramwaj.getNumerBoczny());
                sb.append(")");
                sb.append(" z zamiarem dojechania do przystanku ");
                sb.append(pasażer.getDocelowyPrzystanek()).append(".");
                System.out.println(sb);
                return 1;
            } else {
            // Wyświetlanie informacji o nieudanej próbie wejścia do tramwaju,
            // aby działało należy odkomentować poniższy fragment
//                StringBuilder sb = new StringBuilder();
//                sb.append(getDzieńZdarzenia() + 1).append(", ");
//                if (getCzasZdarzenia() / 60 < 10) {
//                    sb.append(0);
//                }
//                sb.append(getCzasZdarzenia() / 60).append(":");
//                if (getCzasZdarzenia() % 60 < 10) {
//                    sb.append(0);
//                }
//                sb.append(getCzasZdarzenia() % 60);
//                sb.append(": Pasażer ").append(pasażer.getIndeksPasażera());
//                sb.append(" nie wsiadł do tramwaju linii ");
//                sb.append(tramwaj.getLiniaTramwajowa().getNumerLinii());
//                sb.append(" (nr bocz. ").append(tramwaj.getNumerBoczny()).append(")");
//                sb.append(" z powodu braku miejsc w tramwaju.");
//                System.out.println(sb);
                return 0;
            }
        } else {
            return 0;
        }

    }

    @Override
    public Zdarzenie[] następneZdarzenia() {
        return new Zdarzenie[0];
    }
}
