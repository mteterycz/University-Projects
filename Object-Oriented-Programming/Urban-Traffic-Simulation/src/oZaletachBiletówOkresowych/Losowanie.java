package oZaletachBiletówOkresowych;

import java.util.Random;

public class Losowanie {

    public static int losuj(int dolna, int gorna) {
        Random losowanie = new Random();
        return losowanie.nextInt(gorna - dolna + 1) + dolna;
    }
}
