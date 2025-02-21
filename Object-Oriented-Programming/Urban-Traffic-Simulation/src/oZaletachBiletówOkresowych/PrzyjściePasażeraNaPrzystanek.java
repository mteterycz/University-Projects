package oZaletachBiletówOkresowych;

public class PrzyjściePasażeraNaPrzystanek extends Zdarzenie {

    private final Pasażer pasażer;
    private final Przystanek przystanek;

    public PrzyjściePasażeraNaPrzystanek(int dzieńZdarzenia, int czasZdarzenia,
                                         Pasażer pasażer,
                                         Przystanek przystanek) {
        super(dzieńZdarzenia, czasZdarzenia);
        this.pasażer = pasażer;
        this.przystanek = przystanek;
    }

    @Override
    public int wykonajZdarzenie() {
        if (przystanek.czyJestMiejsce()) {
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
            sb.append(" przyszedł na przystanek ");
            sb.append(pasażer.getPobliskiPrzystanek());
            sb.append(" i oczekuje na transport.");
            System.out.println(sb);
        }
        // Wyświetlanie informacji o nieudanej próbie wejścia na przystanek,
        // aby działało należy odkomentować poniższy fragment
//        else {
//            StringBuilder sb = new StringBuilder();
//            sb.append(getDzieńZdarzenia() + 1).append(", ");
//            if (getCzasZdarzenia() / 60 < 10) {
//                sb.append(0);
//            }
//            sb.append(getCzasZdarzenia() / 60).append(":");
//            if (getCzasZdarzenia() % 60 < 10) {
//                sb.append(0);
//            }
//            sb.append(getCzasZdarzenia() % 60);
//            sb.append(": Pasażer ").append(pasażer.getIndeksPasażera());
//            sb.append(" przyszedł na przystanek ");
//            sb.append(pasażer.getPobliskiPrzystanek());
//            sb.append(", lecz zawrócił do domu z powodu braku miejsc na " +
//                    "przystanku.");
//            System.out.println(sb);
//        }
        return 0;
    }

    @Override
    public Zdarzenie[] następneZdarzenia() {
        return new Zdarzenie[0];
    }
}
