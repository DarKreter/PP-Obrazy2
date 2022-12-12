#include "FiltryPGM.h"
#include "FiltryPPM.h"


int main(int argc, char** argv)
{
    int czyOK;
    struct Obraz_t obraz;
    Constructor(&obraz);

    printf("%d\n", czyOK = PrzetwarzajOpcje(argc, argv, &(obraz.options)));

    if (czyOK == W_OK)
    {
        strcpy(obraz.nazwa, obraz.options.inputFileName);
        Odczyt(&obraz);


        if (obraz.status == bad)    //Jesli wystapil blad
        {
            fprintf(stderr, "Wystapil blad podczas otwierania pliku!\nOpis bledu:\t%s\n\n", obraz.statusINFO);
            Wyczysc(&obraz);    //Nie wiadomo w ktorym momencie wystapil wiec dla bezpieczenstwa lepiej wyzerowac wszystko
            return -100;
        }
        
        

        for (int i = 0; obraz.options.listaZadan[i] ; i++)
        {
            /*printf("%c", obraz.options.listaZadan[i]);*/
            switch (obraz.options.listaZadan[i])
            {
            case 'n': {         
                printf("aa");                              /* mamy wykonac negatyw */
                if (obraz.type == P2)
                    NegatywPGM(&obraz);
                else if (obraz.type == P3)
                    NegatywPPM(&obraz);
                break;
            }
            case 'p': {
                if (obraz.type == P2)
                    ProgowaniePGM(&obraz, obraz.options.prog);
                else if (obraz.type == P3)
                    ProgowaniePPM(&obraz, obraz.options.prog, obraz.options.prog, obraz.options.prog);
                break;
            }
            case 'c': {
                if (obraz.type == P3)
                {
                    fprintf(stderr, "Polprogowanie czerni nie dziala dla kolorowego obrazka!\n");
                    Wyczysc(&obraz);    //Nie wiadomo w ktorym momencie wystapil wiec dla bezpieczenstwa lepiej wyzerowac wszystko
                    return -45;
                }
                else if (obraz.type == P2)
                    PolprogowanieCzerniPGM(&obraz, obraz.options.progCzerni);
                break;
            }
            case 'b': {
                if (obraz.type == P3)
                {
                    fprintf(stderr, "Polprogowanie czerni nie dziala dla kolorowego obrazka!\n");
                    Wyczysc(&obraz);    //Nie wiadomo w ktorym momencie wystapil wiec dla bezpieczenstwa lepiej wyzerowac wszystko
                    return -45;
                }
                else if (obraz.type == P2)
                    PolprogowanieBieliPGM(&obraz, obraz.options.progBieli);
                break;
            }
            case 'g': {
                if (obraz.type == P2)
                    KorekcjaGammaPGM(&obraz, obraz.options.gammaWartosc);
                if (obraz.type == P3)
                    KorekcjaGammaPPM(&obraz, obraz.options.gammaWartosc);
                break;
            }
            case 'x': {
                if (obraz.type == P2)
                    RozmywaniePoziomePGM(&obraz, obraz.options.rozmycieXwartosc);
                if (obraz.type == P3)
                    RozmywaniePoziomePPM(&obraz, obraz.options.rozmycieXwartosc);
                break;
            }
            case 'y': {
                if (obraz.type == P2)
                    RozmywaniePionowePGM(&obraz, obraz.options.rozmycieYwartosc);
                if (obraz.type == P3)
                    RozmywaniePionowePPM(&obraz, obraz.options.rozmycieYwartosc);
                break;
            }
            case 'z': {
                if (obraz.type == P3)
                {
                    fprintf(stderr, "Zmiana poziomow nie dziala dla kolorowego obrazka!\n");
                    Wyczysc(&obraz);    //Nie wiadomo w ktorym momencie wystapil wiec dla bezpieczenstwa lepiej wyzerowac wszystko
                    return -45;
                }
                else if (obraz.type == P2)
                    ZmianaPoziomowPGM(&obraz, obraz.options.zpCzern, obraz.options.zpBiel);

                break;
            }
            case 'm': {
                if (obraz.type == P2)
                {
                    fprintf(stderr, "Nie da sie wywalac funkcji zmieniajacej Kolorowa bitmape na szara, jesli pracujemy na bitmapie w skali szarosci!\n");
                    Wyczysc(&obraz);    //Nie wiadomo w ktorym momencie wystapil wiec dla bezpieczenstwa lepiej wyzerowac wszystko
                    return -45;
                }
                if (obraz.type == P3)
                    Kolor2Szarosc(&obraz);
                break;
            }
            case 'k': {                                     /* mamy wykonac konturowanie */
                if (obraz.type == P2)
                    KonturowaniePGM(&obraz);
                if (obraz.type == P3)
                    KonturowaniePPM(&obraz);
                break;
            }
            case 'h': {                                     /* mamy wykonac konturowanie */
                if (obraz.type == P2)
                    RozciaganieHistogramuPGM(&obraz);
                if (obraz.type == P3)
                    RozciaganieHistogramuPPM(&obraz);
                break;
            }
            case 's': {                                     /* mamy wykonac konturowanie */
                if (obraz.type == P2)
                    SplotPGM(&obraz, &(obraz.options.maskaSplotu));
                if (obraz.type == P3)
                    SplotPPM(&obraz, &(obraz.options.maskaSplotu));
                break;
            }
            default:                                        /* nierozpoznana opcja - nie wiadomo jak*/
                return -420;
            }

            if (obraz.status == bad)    //Jesli wystapil blad
            {
                fprintf(stderr, "Wystapil blad podczas przetwarzania obrazu!\nOpis bledu:\t%s\n\n", obraz.statusINFO);
                Wyczysc(&obraz);    //Nie wiadomo w ktorym momencie wystapil wiec dla bezpieczenstwa lepiej wyzerowac wszystko
                return -100;
            }
        }

        strcpy(obraz.nazwa, obraz.options.outputFileName);

        if (obraz.options.wyswietlenie == 1)
        {
            if (obraz.type == P2)
            {
                strcpy(obraz.nazwa, "temp.pgm");
                Zapis(&obraz);
                Wyswietl("temp.pgm");
            }
            else if (obraz.type == P3)
            {
                strcpy(obraz.nazwa, "temp.ppm");
                Zapis(&obraz);
                Wyswietl("temp.ppm");
            }
        }

        Zapis(&obraz);
        if (obraz.status == bad)    //Jesli wystapil blad
        {
            fprintf(stderr, "Wystapil blad podczas otwierania pliku!\nOpis bledu:\t%s\n\n", obraz.statusINFO);
            Wyczysc(&obraz);    //Nie wiadomo w ktorym momencie wystapil wiec dla bezpieczenstwa lepiej wyzerowac wszystko
            return -100;
        }
    }
 
    return czyOK;
}

