#include "FiltryPGM.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void NegatywPGM(struct Obraz_t* obrazPGM)
{
    short** dane = (short**)obrazPGM->data;
    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            dane[i][j] = obrazPGM->maxSzarosc - dane[i][j];

    DodajKomentarz(obrazPGM, "Wykonano operacje negatywu!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ProgowaniePGM(struct Obraz_t* obrazPGM, short procent)
{
    short** dane = (short**)obrazPGM->data;
    //Sprawdzenie blednych danych wejsciowych
    if (procent > 100 || procent < 0)
    {
        ErrorOccurred(obrazPGM, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    //Przeliczenie procentow na prog
    int prog = procent * obrazPGM->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (dane[i][j] <= prog)
                dane[i][j] = 0;
            else
                dane[i][j] = obrazPGM->maxSzarosc;

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania(%d%%)!", procent);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void PolprogowanieCzerniPGM(struct Obraz_t* obrazPGM, short procent)
{
    short** dane = (short**)obrazPGM->data;
    //Sprawdzenie blednych danych wejsciowych
    if (procent > 100 || procent < 0)
    {
        ErrorOccurred(obrazPGM, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    //Wyliczenie progu
    int prog = procent * obrazPGM->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (dane[i][j] <= prog)
                dane[i][j] = 0;

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania czerni(%d%%)!", procent);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void PolprogowanieBieliPGM(struct Obraz_t* obrazPGM, short procent)
{
    short** dane = (short**)obrazPGM->data;
    //Sprawdzenie blednych danych wejsciowych
    if (procent > 100 || procent < 0)
    {
        ErrorOccurred(obrazPGM, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    //Wyliczenie progu
    int prog = procent * obrazPGM->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (dane[i][j] > prog)
                dane[i][j] = obrazPGM->maxSzarosc;

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania bieli(%d%%)!", procent);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ZmianaPoziomowPGM(struct Obraz_t* obrazPGM, short czern, short biel)
{
    short** dane = (short**)obrazPGM->data;
    //Sprawdzenie blednych danych wejsciowych
    if (czern > 100 || czern < 0 || biel < 0 || biel > 100)
    {
        ErrorOccurred(obrazPGM, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    if (czern >= biel)
    {
        ErrorOccurred(obrazPGM, "Progowanie bieli musi byc wieksze niz progowanie czerni dla zmiany poziomow!");
        return;
    }

    //Wyliczenie progow
    int progBiel = biel * obrazPGM->maxSzarosc * 0.01;
    int progCzern = czern * obrazPGM->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (dane[i][j] <= progCzern)
                dane[i][j] = 0;
            else if (dane[i][j] > progBiel)
                dane[i][j] = obrazPGM->maxSzarosc;
            else
                dane[i][j] = (dane[i][j] - progCzern) * (obrazPGM->maxSzarosc / (progBiel - progCzern));

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje zmiany poziomow(czern - %d%%, biel - %d%%)!", czern, biel);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void KorekcjaGammaPGM(struct Obraz_t* obrazPGM, float gamma)
{
    short** dane = (short**)obrazPGM->data;
    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            dane[i][j] = pow(((float)dane[i][j] / (float)obrazPGM->maxSzarosc), 1.0 / gamma) * (float)obrazPGM->maxSzarosc;

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania gamma(wspolczynnik - %f)!", gamma);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void KonturowaniePGM(struct Obraz_t* obrazPGM)
{
    short** dane = (short**)obrazPGM->data;
    //Stworzenie tablicy ktora bedzie kopia naszych pikseli
    short** copy;
    if ( (copy = malloc(sizeof(short*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji Konturowanie!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if( (copy[i] = malloc(sizeof(short) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji Konturowanie!");
            return;
        }

    //Skopiowanie do niej danych
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            copy[i][j] = dane[i][j];

    //Odpowiednie modyfikacje pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (j + 1 < obrazPGM->x && i + 1 < obrazPGM->y) //Sprawdzamy czy nie wychodzimy poza zakres tablicy
            {
                short nowaWartosc = abs(copy[i][j + 1] - copy[i][j]) + abs(copy[i + 1][j] - copy[i][j]);
                if (nowaWartosc > obrazPGM->maxSzarosc) //Niektore wartosci pikseli wychodza poza zakres przez co trzeba im wpisac wartosc max
                    nowaWartosc = obrazPGM->maxSzarosc;

                dane[i][j] = nowaWartosc;
            }
            else
                dane[i][j] = 0;

    //Zwolnienie pamieci dla kopii
    for (int i = 0; i < obrazPGM->y; i++)
        free(copy[i]);
    free(copy);

    DodajKomentarz(obrazPGM, "Wykonano operacje konturowania!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozmywaniePoziomePGM(struct Obraz_t* obrazPGM, short promien)
{
    short** dane = (short**)obrazPGM->data;
    //Sprawdzenie poprawnosci przysylanych argumentow
    if (promien < 0)
    {
        ErrorOccurred(obrazPGM, "Promien powinien byc wiekszy niz 0!");
        return;
    }

    //Sprawdzenie poprawnosci przysylanych argumentow
    short** copy;
    if ((copy = malloc(sizeof(short*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if ((copy[i] = malloc(sizeof(short) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie do niej danych
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            copy[i][j] = dane[i][j];

    //Odpowiednie modyfikacje pikseli
    int licznik, k, max;
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
        {
            //Zerujemy aktualna wartosc piksela
            dane[i][j] = 0;

            //Na koniec biezemy srednia wiec musimy wiedziec z ilu elementow
            licznik = 0; 
            //w zaleznosci od promienia powinnismy zaczac iles pikseli w lewo
            //Jednak jesli wychodzimy poza zakres tablicy zaczynamy od poczatku wiersza
            k = (j - promien >= 0 ? j - promien : 0);
            //Jak powyzej w zaleznosci czy wychodzi poza tablice ustalamy max piksel ktory bedziemy brac pod uwage
            max = (promien + j >= obrazPGM->x ? obrazPGM->x - 1 : promien + j);

            //Sumujemy wszystkie piksele z poprzednio wyliczonego przedzialu
            for (; k <= max; k++, licznik++)
                dane[i][j] += copy[i][k];

            //Bierzemy srednia
            dane[i][j] /= licznik;
        }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obrazPGM->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje rozmycia poziomego(promien - %d)!", promien);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozmywaniePionowePGM(struct Obraz_t* obrazPGM, short promien)
{
    short** dane = (short**)obrazPGM->data;
    //Sprawdzenie poprawnosci przysylanych argumentow
    if (promien < 0)
    {
        ErrorOccurred(obrazPGM, "Promien powinien byc wiekszy niz 0!");
        return;
    }

    //Sprawdzenie poprawnosci przysylanych argumentow
    short** copy;
    if ((copy = malloc(sizeof(short*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if ((copy[i] = malloc(sizeof(short) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie danych do kopii
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            copy[i][j] = dane[i][j];


    //Odpowiednie modyfikacje pikseli
    int k, licznik, max;
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
        {
            //Zerujemy aktualna wartosc piksela
            dane[i][j] = 0;

            //Na koniec biezemy srednia wiec musimy wiedziec z ilu elementow
            licznik = 0;
            //w zaleznosci od promienia powinnismy zaczac iles pikseli w gore
            //Jednak jesli wychodzimy poza zakres tablicy zaczynamy od poczatku kolumny
            k = (i - promien >= 0 ? i - promien : 0);
            //Jak powyzej w zaleznosci czy wychodzi poza tablice ustalamy max piksel ktory bedziemy brac pod uwage
            max = (promien + i >= obrazPGM->y ? obrazPGM->y - 1 : promien + i);

            //Sumujemy wszystkie piksele z poprzednio wyliczonego przedzialu
            for (; k <= max; k++, licznik++)
                dane[i][j] += copy[k][j];

            //Bierzemy srednia
            dane[i][j] /= licznik;
        }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obrazPGM->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje rozmycia pionowego(promien - %d)!", promien);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozciaganieHistogramuPGM(struct Obraz_t* obrazPGM)
{
    short** dane = (short**)obrazPGM->data;
    int min = obrazPGM->maxSzarosc, max = 0;

    //Ustalenie max i min
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
        {
            if (dane[i][j] > max)
                max = dane[i][j];
            if (dane[i][j] < min)
                min = dane[i][j];
        }

    //modyfikacje pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            dane[i][j] = (dane[i][j] - min) * ((float)obrazPGM->maxSzarosc / (float)(max - min));

    DodajKomentarz(obrazPGM, "Wykonane operacje rozciagania histogramu!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void SplotPGM(struct Obraz_t* obrazPGM, Macierz3x3_t* maska)
{
    short** dane = (short**)obrazPGM->data;
    int max, min;
    //Stworzenie kopii
    short** copy;
    if ((copy = malloc(sizeof(short*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if ((copy[i] = malloc(sizeof(short) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie danych do kopii
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            copy[i][j] = dane[i][j];

    //Odpowiednie modyfikacje pikseli
    for (int i = 1; i < obrazPGM->y - 1; i++)
        for (int j = 1; j < obrazPGM->x - 1; j++)
        {
            dane[i][j] =  maska->tab[0][0] * copy[i - 1][j - 1]  +  maska->tab[0][1] * copy[i - 1][j]   + maska->tab[0][2] * copy[i - 1][j + 1]
                                     +  maska->tab[1][0] * copy[i][j - 1]      +  maska->tab[1][1] * copy[i][j]       + maska->tab[1][2] * copy[i][j + 1]
                                     +  maska->tab[2][0] * copy[i + 1][j - 1]  +  maska->tab[2][1] * copy[i + 1][j]   + maska->tab[2][2] * copy[i + 1][j + 1];
            if (i ==  1 && j == 1)
                max = min = dane[i][j];
            else if (dane[i][j] > max)
                max = dane[i][j];
            else if (dane[i][j] < min)
                dane[i][j] = min;

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
        for (int i = 0; i < obrazPGM->y; i++)
            for (int j = 0; j < obrazPGM->x; j++)
                dane[i][j] = (float)(dane[i][j] - min) / (float)(max - min) * (float)obrazPGM->maxSzarosc;
    }
    else                    //Jesli dodatnie
    {
        for (int i = 0; i < obrazPGM->y; i++)
            for (int j = 0; j < obrazPGM->x; j++)
                dane[i][j] = (float)dane[i][j] / (float)sumaMASKI;
    }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obrazPGM->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje liniowej filtracji kontekstowej [%d, %d, %d; %d, %d, %d; %d, %d, %d]!", 
        maska->tab[0][0], maska->tab[0][1], maska->tab[0][2], maska->tab[1][0], maska->tab[1][1], maska->tab[1][2],
        maska->tab[2][0], maska->tab[2][1], maska->tab[2][2]);
    DodajKomentarz(obrazPGM, temp);
}
