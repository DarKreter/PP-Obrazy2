#include "FiltryPPM.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void NegatywPPM(struct Obraz_t* obraz)
{
    RGB_t** dane = (RGB_t**)obraz->data;
    //Modyfikacja pikseli
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
        {
            if(obraz->options.kRed == 1)
                dane[i][j].red = obraz->maxSzarosc - dane[i][j].red;
            if (obraz->options.kGreen == 1)
                dane[i][j].green = obraz->maxSzarosc - dane[i][j].green;
            if (obraz->options.kBlue == 1)
                dane[i][j].blue = obraz->maxSzarosc - dane[i][j].blue;
        }

    DodajKomentarz(obraz, "Wykonano operacje negatywu!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ProgowaniePPM(struct Obraz_t* obraz, short pr, short pg,  short pb)
{
    RGB_t** dane = (RGB_t**)obraz->data;
    //Sprawdzenie blednych danych wejsciowych
    if (pr > 100 || pr < 0 || pg > 100 || pg < 0 || pb > 100 || pb < 0)
    {
        ErrorOccurred(obraz, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    //Przeliczenie procentow na prog
    pr = pr * obraz->maxSzarosc * 0.01;
    pg = pg * obraz->maxSzarosc * 0.01;
    pb = pb * obraz->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
        {
            if (obraz->options.kRed == 1)
            {
                if (dane[i][j].red <= pr)
                    dane[i][j].red = 0;
                else
                    dane[i][j].red = obraz->maxSzarosc;
            }

            if (obraz->options.kGreen == 1)
            {
                if (dane[i][j].green <= pg)
                    dane[i][j].green = 0;
                else
                    dane[i][j].green = obraz->maxSzarosc;
            }

            if (obraz->options.kBlue == 1)
            {
                if (dane[i][j].blue <= pb)
                    dane[i][j].blue = 0;
                else
                    dane[i][j].blue = obraz->maxSzarosc;
            }

        }

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania(%d%% %d%% %d%%)!", pr, pg, pb);
    DodajKomentarz(obraz, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void KorekcjaGammaPPM(struct Obraz_t* obraz, float gamma)
{
    RGB_t** dane = (RGB_t**)obraz->data;
    //Modyfikacja pikseli
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
        {
            if (obraz->options.kRed == 1)
                dane[i][j].red   = pow(((float)dane[i][j].red   / (float)obraz->maxSzarosc), 1.0 / gamma) * (float)obraz->maxSzarosc;
            
            if (obraz->options.kGreen == 1)
                dane[i][j].green = pow(((float)dane[i][j].green / (float)obraz->maxSzarosc), 1.0 / gamma) * (float)obraz->maxSzarosc;
            
            if (obraz->options.kBlue == 1)
                dane[i][j].blue  = pow(((float)dane[i][j].blue  / (float)obraz->maxSzarosc), 1.0 / gamma) * (float)obraz->maxSzarosc;
        }

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania gamma(wspolczynnik - %f)!", gamma);
    DodajKomentarz(obraz, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void KonturowaniePPM(struct Obraz_t* obraz)
{
    RGB_t** dane = (RGB_t**)obraz->data;
    //Stworzenie tablicy ktora bedzie kopia naszych pikseli
    RGB_t** copy;
    if ((copy = malloc(sizeof(RGB_t*) * obraz->y)) == NULL)
    {
        ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Konturowanie!");
        return;
    }
    for (int i = 0; i < obraz->y; i++)
        if ((copy[i] = malloc(sizeof(RGB_t) * obraz->x)) == NULL)
        {
            ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Konturowanie!");
            return;
        }

    //Skopiowanie do niej danych
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
            copy[i][j] = dane[i][j];

    //Odpowiednie modyfikacje pikseli
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
            if (j + 1 < obraz->x && i + 1 < obraz->y) //Sprawdzamy czy nie wychodzimy poza zakres tablicy
            {
                short nowaWartosc = abs(copy[i][j + 1].red - copy[i][j].red) + abs(copy[i + 1][j].red - copy[i][j].red);
                if (nowaWartosc > obraz->maxSzarosc) //Niektore wartosci pikseli wychodza poza zakres przez co trzeba im wpisac wartosc max
                    nowaWartosc = obraz->maxSzarosc;

                dane[i][j].red = nowaWartosc;

                nowaWartosc = abs(copy[i][j + 1].green - copy[i][j].green) + abs(copy[i + 1][j].green - copy[i][j].green);
                if (nowaWartosc > obraz->maxSzarosc) //Niektore wartosci pikseli wychodza poza zakres przez co trzeba im wpisac wartosc max
                    nowaWartosc = obraz->maxSzarosc;

                dane[i][j].green = nowaWartosc;

                nowaWartosc = abs(copy[i][j + 1].blue - copy[i][j].blue) + abs(copy[i + 1][j].blue - copy[i][j].blue);
                if (nowaWartosc > obraz->maxSzarosc) //Niektore wartosci pikseli wychodza poza zakres przez co trzeba im wpisac wartosc max
                    nowaWartosc = obraz->maxSzarosc;

                dane[i][j].blue = nowaWartosc;
            }
            else
                dane[i][j].red = dane[i][j].green = dane[i][j].blue = 0;

    //Zwolnienie pamieci dla kopii
    for (int i = 0; i < obraz->y; i++)
        free(copy[i]);
    free(copy);

    DodajKomentarz(obraz, "Wykonano operacje konturowania!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozmywaniePoziomePPM(struct Obraz_t* obraz, short promien)
{
    RGB_t** dane = (RGB_t**)obraz->data;
    //Sprawdzenie poprawnosci przysylanych argumentow
    if (promien < 0)
    {
        ErrorOccurred(obraz, "Promien powinien byc wiekszy niz 0!");
        return;
    }

    //Sprawdzenie poprawnosci przysylanych argumentow
    RGB_t** copy;
    if ((copy = malloc(sizeof(RGB_t*) * obraz->y)) == NULL)
    {
        ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obraz->y; i++)
        if ((copy[i] = malloc(sizeof(RGB_t) * obraz->x)) == NULL)
        {
            ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie do niej danych
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
            copy[i][j] = dane[i][j];

    //Odpowiednie modyfikacje pikseli
    int licznik, k, max;
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
        {
            //Zerujemy aktualna wartosc piksela
            if (obraz->options.kRed == 1)
                dane[i][j].red = 0;
            if (obraz->options.kGreen == 1)
                dane[i][j].green = 0;
            if (obraz->options.kBlue == 1)
                dane[i][j].blue = 0;

            //Na koniec biezemy srednia wiec musimy wiedziec z ilu elementow
            licznik = 0; 
            //w zaleznosci od promienia powinnismy zaczac iles pikseli w lewo
            //Jednak jesli wychodzimy poza zakres tablicy zaczynamy od poczatku wiersza
            k = (j - promien >= 0 ? j - promien : 0);
            //Jak powyzej w zaleznosci czy wychodzi poza tablice ustalamy max piksel ktory bedziemy brac pod uwage
            max = (promien + j >= obraz->x ? obraz->x - 1 : promien + j);

            //Sumujemy wszystkie piksele z poprzednio wyliczonego przedzialu
            for (; k <= max; k++, licznik++)
            {
                if (obraz->options.kRed == 1)
                    dane[i][j].red += copy[i][k].red;
                if (obraz->options.kGreen == 1)
                    dane[i][j].green += copy[i][k].green;
                if (obraz->options.kBlue == 1)
                    dane[i][j].blue += copy[i][k].blue;
            }

            //Bierzemy srednia
            if (obraz->options.kRed == 1)
                dane[i][j].red /= licznik;
            if (obraz->options.kGreen == 1)
                dane[i][j].green /= licznik;
            if (obraz->options.kBlue == 1)
                dane[i][j].blue /= licznik;
        }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obraz->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje rozmycia poziomego(promien - %d)!", promien);
    DodajKomentarz(obraz, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozmywaniePionowePPM(struct Obraz_t* obraz, short promien)
{
    RGB_t** dane = (RGB_t**)obraz->data;
    //Sprawdzenie poprawnosci przysylanych argumentow
    if (promien < 0)
    {
        ErrorOccurred(obraz, "Promien powinien byc wiekszy niz 0!");
        return;
    }

    //Sprawdzenie poprawnosci przysylanych argumentow
    RGB_t** copy;
    if ((copy = malloc(sizeof(RGB_t*) * obraz->y)) == NULL)
    {
        ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obraz->y; i++)
        if ((copy[i] = malloc(sizeof(RGB_t) * obraz->x)) == NULL)
        {
            ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie danych do kopii
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
            copy[i][j] = dane[i][j];


    //Odpowiednie modyfikacje pikseli
    int k, licznik, max;
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
        {
            //Zerujemy aktualna wartosc piksela
            if (obraz->options.kRed == 1)
                dane[i][j].red = 0;
            if (obraz->options.kGreen == 1)
                dane[i][j].green = 0;
            if (obraz->options.kBlue == 1)
                dane[i][j].blue = 0;

            //Na koniec biezemy srednia wiec musimy wiedziec z ilu elementow
            licznik = 0;
            //w zaleznosci od promienia powinnismy zaczac iles pikseli w gore
            //Jednak jesli wychodzimy poza zakres tablicy zaczynamy od poczatku kolumny
            k = (i - promien >= 0 ? i - promien : 0);
            //Jak powyzej w zaleznosci czy wychodzi poza tablice ustalamy max piksel ktory bedziemy brac pod uwage
            max = (promien + i >= obraz->y ? obraz->y - 1 : promien + i);

            //Sumujemy wszystkie piksele z poprzednio wyliczonego przedzialu
            for (; k <= max; k++, licznik++)
            {
                if (obraz->options.kRed == 1)
                    dane[i][j].red += copy[k][j].red;   
                if (obraz->options.kGreen == 1)
                    dane[i][j].green += copy[k][j].green;
                if (obraz->options.kBlue == 1)
                    dane[i][j].blue += copy[k][j].blue;

            }

            //Bierzemy srednia
            if (obraz->options.kRed == 1)
                dane[i][j].red /= licznik;
            if (obraz->options.kGreen == 1)
                dane[i][j].green /= licznik;
            if (obraz->options.kBlue == 1)
                dane[i][j].blue /= licznik;
        }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obraz->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje rozmycia pionowego(promien - %d)!", promien);
    DodajKomentarz(obraz, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozciaganieHistogramuPPM(struct Obraz_t* obraz)
{
    RGB_t** dane = (RGB_t**)obraz->data;

    int minR, minG, minB, maxR = 0, maxB = 0, maxG = 0;
    minR = minG = minB = obraz->maxSzarosc;

    //Ustalenie max i min
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
        {
            if (obraz->options.kRed == 1)
            {
                if (dane[i][j].red > maxR)
                    maxR = dane[i][j].red;

                if (dane[i][j].red < minR)
                    minR = dane[i][j].red;
            }
            if (obraz->options.kGreen == 1)
            {
                if (dane[i][j].green > maxG)
                    maxG = dane[i][j].green;

                if (dane[i][j].green < minG)
                    minG = dane[i][j].green;
            }
            if (obraz->options.kBlue == 1)
            {
                if (dane[i][j].blue > maxB)
                    maxB = dane[i][j].blue;

                if (dane[i][j].blue < minB)
                    minB = dane[i][j].blue;

            }


        }

    //modyfikacje pikseli
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
        {
            if (obraz->options.kRed == 1)
                dane[i][j].red = (dane[i][j].red - minR) * ((float)obraz->maxSzarosc / (float)(maxR - minR));
            if (obraz->options.kGreen == 1)
                dane[i][j].green = (dane[i][j].green - minG) * ((float)obraz->maxSzarosc / (float)(maxG - minG));
            if (obraz->options.kBlue == 1)
                dane[i][j].blue = (dane[i][j].blue - minB) * ((float)obraz->maxSzarosc / (float)(maxB - minB));
        }

    DodajKomentarz(obraz, "Wykonane operacje rozciagania histogramu!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void SplotPPM(struct Obraz_t* obraz, Macierz3x3_t* maska)
{
    RGB_t** dane = (RGB_t**)obraz->data;
    int maxR, minR, maxG, maxB, minB, minG;
    //Stworzenie kopii
    RGB_t** copy;
    if ((copy = malloc(sizeof(RGB_t*) * obraz->y)) == NULL)
    {
        ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obraz->y; i++)
        if ((copy[i] = malloc(sizeof(RGB_t) * obraz->x)) == NULL)
        {
            ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie danych do kopii
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
            copy[i][j] = dane[i][j];

    //Odpowiednie modyfikacje pikseli
    for (int i = 1; i < obraz->y - 1; i++)
        for (int j = 1; j < obraz->x - 1; j++)
        {
            if (obraz->options.kRed == 1)
            {
                dane[i][j].red = maska->tab[0][0] * copy[i - 1][j - 1].red + maska->tab[0][1] * copy[i - 1][j].red + maska->tab[0][2] * copy[i - 1][j + 1].red
                    + maska->tab[1][0] * copy[i][j - 1].red + maska->tab[1][1] * copy[i][j].red + maska->tab[1][2] * copy[i][j + 1].red
                    + maska->tab[2][0] * copy[i + 1][j - 1].red + maska->tab[2][1] * copy[i + 1][j].red + maska->tab[2][2] * copy[i + 1][j + 1].red;
                if (i == 1 && j == 1)
                    maxR = minR = dane[i][j].red;
                else if (dane[i][j].red > maxR)
                    maxR = dane[i][j].red;
                else if (dane[i][j].red < minR)
                    dane[i][j].red = minR;
            }

            if (obraz->options.kGreen == 1)
            {
                dane[i][j].green = maska->tab[0][0] * copy[i - 1][j - 1].green + maska->tab[0][1] * copy[i - 1][j].green + maska->tab[0][2] * copy[i - 1][j + 1].green
                    + maska->tab[1][0] * copy[i][j - 1].green + maska->tab[1][1] * copy[i][j].green + maska->tab[1][2] * copy[i][j + 1].green
                    + maska->tab[2][0] * copy[i + 1][j - 1].green + maska->tab[2][1] * copy[i + 1][j].green + maska->tab[2][2] * copy[i + 1][j + 1].green;
                if (i == 1 && j == 1)
                    maxG = minG = dane[i][j].green;
                else if (dane[i][j].green > maxG)
                    maxG = dane[i][j].green;
                else if (dane[i][j].green < minG)
                    dane[i][j].green = minG;
            }

            if (obraz->options.kBlue == 1)
            {
                dane[i][j].blue = maska->tab[0][0] * copy[i - 1][j - 1].blue + maska->tab[0][1] * copy[i - 1][j].blue + maska->tab[0][2] * copy[i - 1][j + 1].blue
                    + maska->tab[1][0] * copy[i][j - 1].blue + maska->tab[1][1] * copy[i][j].blue + maska->tab[1][2] * copy[i][j + 1].blue
                    + maska->tab[2][0] * copy[i + 1][j - 1].blue + maska->tab[2][1] * copy[i + 1][j].blue + maska->tab[2][2] * copy[i + 1][j + 1].blue;
                if (i == 1 && j == 1)
                    maxB = minB = dane[i][j].blue;
                else if (dane[i][j].blue > maxB)
                    maxB = dane[i][j].blue;
                else if (dane[i][j].blue < minB)
                    dane[i][j].blue = minB;
            }


        }

    //Spradzenie czy w masce mam wartosci ujemne
    short ujemnaMASKA = 0, sumaMASKI = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if (maska->tab[i][j] < 0)
                ujemnaMASKA = 1;
            sumaMASKI += maska->tab[i][j];
        }

    if (ujemnaMASKA == 1)   //Jesli ujemne
    {
        for (int i = 0; i < obraz->y; i++)
            for (int j = 0; j < obraz->x; j++)
            {
                if (obraz->options.kRed == 1)
                    dane[i][j].red = (float)(dane[i][j].red - minR) / (float)(maxR - minR) * (float)obraz->maxSzarosc;
                if (obraz->options.kGreen == 1)
                    dane[i][j].green = (float)(dane[i][j].green - minG) / (float)(maxG - minG) * (float)obraz->maxSzarosc;
                if (obraz->options.kBlue == 1)
                    dane[i][j].blue = (float)(dane[i][j].blue - minB) / (float)(maxB - minB) * (float)obraz->maxSzarosc;
            }
    }
    else                    //Jesli dodatnie
    {
        for (int i = 0; i < obraz->y; i++)
            for (int j = 0; j < obraz->x; j++)
            {
                if (obraz->options.kRed == 1)
                    dane[i][j].red = (float)dane[i][j].red / (float)sumaMASKI;
                if (obraz->options.kGreen == 1)
                    dane[i][j].green = (float)dane[i][j].green / (float)sumaMASKI;
                if (obraz->options.kBlue == 1)
                    dane[i][j].blue = (float)dane[i][j].blue / (float)sumaMASKI;
            }
    }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obraz->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje liniowej filtracji kontekstowej [%d, %d, %d; %d, %d, %d; %d, %d, %d]!",
        maska->tab[0][0], maska->tab[0][1], maska->tab[0][2], maska->tab[1][0], maska->tab[1][1], maska->tab[1][2],
        maska->tab[2][0], maska->tab[2][1], maska->tab[2][2]);
    DodajKomentarz(obraz, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Kolor2Szarosc(struct Obraz_t* obraz)
{
    RGB_t** dane = (RGB_t**)obraz->data;
    //Zarezerwowanie miejsca dla kopii aktualnej bitmapy w kolorze
    RGB_t** copy;
    if ((copy = malloc(sizeof(RGB_t*) * obraz->y)) == NULL)
    {
        ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Kolor2Szarosc!");
        return;
    }
    for (int i = 0; i < obraz->y; i++)
        if ((copy[i] = malloc(sizeof(RGB_t) * obraz->x)) == NULL)
        {
            ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Kolor2Szarosc!");
            return;
        }
    //Skopiowanie danych do kopii
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
            copy[i][j] = dane[i][j];

    //Zwolnienie pamieci dla tablicy bedacej w oryginalnej strukturze
    for (int i = 0; i < obraz->y; i++)
        free(dane[i]);
    free(dane);

    //Rezerwacja pamieci dla tablicy w strukturze ale dla szaerj bitmapy
    if (( (obraz->data) = malloc(sizeof(short*) * obraz->y )) == NULL)
    {
        ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Kolor2Szarosc!");
        return;
    }
    for (int i = 0; i < obraz->y; i++)
        if (( (obraz->data[i]) = malloc(sizeof(short) * obraz->x )) == NULL)
        {
            ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Kolor2Szarosc!");
            return;
        }

    //Wyliczenie konkretnych wartosci dla szarych pikseli na podstawie kolorywych
    short** daane = (short**)obraz->data;
    for (int i = 0; i < obraz->y; i++)
        for (int j = 0; j < obraz->x; j++)
            daane[i][j] = (copy[i][j].red + copy[i][j].green + copy[i][j].blue) / 3;

    //Zwolnienie pamieci dla Kopii
    for (int i = 0; i < obraz->y; i++)
        free(copy[i]);
    free(copy);

    //Zmiana typu obrazka
    obraz->type = P2;
}