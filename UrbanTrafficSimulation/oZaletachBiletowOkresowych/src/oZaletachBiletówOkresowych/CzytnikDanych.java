package oZaletachBiletówOkresowych;

import java.util.Scanner;
import java.util.Arrays;

public class CzytnikDanych {
    private final int MINIMALNALICZBADANYCH = 6;

    public String[] czytajDane() {
        String[] dane = new String[MINIMALNALICZBADANYCH];
        int aktualnaLiczbaDanych = 0;
        Scanner skaner = new Scanner(System.in);
        while (skaner.hasNext()) {
            if (aktualnaLiczbaDanych == dane.length) {
                dane = Arrays.copyOf(dane, 3 * dane.length / 2 + 2);
            }
            dane[aktualnaLiczbaDanych] = skaner.next();
            ++aktualnaLiczbaDanych;
        }
        skaner.close();
        dane = Arrays.copyOf(dane, aktualnaLiczbaDanych);
        return dane;
    }
}