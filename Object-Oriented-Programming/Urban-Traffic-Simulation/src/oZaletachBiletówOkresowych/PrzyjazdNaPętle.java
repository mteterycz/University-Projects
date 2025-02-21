package oZaletachBiletówOkresowych;

public class PrzyjazdNaPętle extends Zdarzenie {

    private final Przystanek przystanek;
    private final Tramwaj tramwaj;
    private final LiniaTramwajowa liniaTramwajowa;

    public PrzyjazdNaPętle(int dzieńZdarzenia, int czasZdarzenia,
                           Przystanek przystanek, Tramwaj tramwaj,
                           LiniaTramwajowa liniaTramwajowa) {
        super(dzieńZdarzenia, czasZdarzenia);
        this.przystanek = przystanek;
        this.tramwaj = tramwaj;
        this.liniaTramwajowa = liniaTramwajowa;
    }

    @Override
    public int wykonajZdarzenie() {
        if (getCzasZdarzenia() <= 23 * 60) {
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
            sb.append(": Tramwaj (nr. bocz. ").append(tramwaj.getNumerBoczny()).append(") linii ").append(liniaTramwajowa.getNumerLinii());
            sb.append(" rozpoczął postój na pętli przy przystanku ").append(przystanek.getNazwa()).append(".");
            System.out.println(sb);
        }
        return 0;
    }

    @Override
    public Zdarzenie[] następneZdarzenia() {
        if (getCzasZdarzenia() <= 23 * 60) {
            Zdarzenie[] noweZdarzenia = new Zdarzenie[1];
            int dzień = getDzieńZdarzenia();
            int czas = getCzasZdarzenia() + liniaTramwajowa.czasPostojuNaPętli();
            noweZdarzenia[0] = new WyjazdZPętli(dzień, czas,
                    przystanek, tramwaj, liniaTramwajowa);
            return noweZdarzenia;
        }
        else {
            return new Zdarzenie[0];
        }
    }
}
