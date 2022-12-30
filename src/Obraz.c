#include "Obraz.h"


//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Constructor(struct Obraz_t* obraz)
{
    obraz->status = bad;
    obraz->fileHandle = NULL;
    obraz->data = NULL;
    strcpy(obraz->nazwa, "");
    strcpy(obraz->comments, "");
    strcpy(obraz->statusINFO, "Plik nie zostal jeszcze wczytany!");
    obraz->x = obraz->y = obraz->maxSzarosc = 0;
    obraz->type = nothing;
    OpcjeKonstruktor(&(obraz->options));
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Wyczysc(struct Obraz_t* obrazPGM)
{
    if (obrazPGM->data != NULL)
    {
        for (int i = 0; i < obrazPGM->y; i++)
            free(obrazPGM->data[i]);
        free(obrazPGM->data);
    }
    if (obrazPGM->fileHandle != NULL)
        fclose(obrazPGM->fileHandle);

    //Na koniec wpisujemy domniemane wartosci
    Constructor(obrazPGM);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Zapis(struct Obraz_t* obraz)
{
    if (strcmp(obraz->nazwa, "stdout") == 0)
        obraz->fileHandle = stdout;
    else
        obraz->fileHandle = fopen(obraz->nazwa, "w");               //otwieramy plik do pisania

    if (obraz->fileHandle == NULL)                                  //Jesli pliku nie udalo sie otworzyc
    {
        ErrorOccurred(obraz, "Nie udalo sie zapisac pliku!");
        return;
    }

    if(obraz->type == P2)
        fprintf(obraz->fileHandle, "P2\n");     //Plik z skala szarosci                                               
    else if(obraz->type == P3)
        fprintf(obraz->fileHandle, "P3\n");     //Plik kolorowy

    fprintf(obraz->fileHandle, "%s", obraz->comments);                                      //Komentarze jakie byly i jakie dodalismy
    fprintf(obraz->fileHandle, "%d %d\n%d\n", obraz->x, obraz->y, obraz->maxSzarosc);       //Podajemy wymiary oraz skale szarosci

    if (obraz->type == P2)  //Jesli jest to szary obraz wczytujemy go normalnie piksel po pikselu
    {           
        short** dane = (short**)obraz->data;    //Jeden piksel to jedna liczba
        //Petla ktora zapisuje wszystkie piksele
        for (int i = 0; i < obraz->y; i++)       
        {
            for (int j = 0; j < obraz->x; j++)
                fprintf(obraz->fileHandle, "%hi ", dane[i][j]);
            fprintf(obraz->fileHandle, "\n");
        }
    }
    else if (obraz->type == P3) //Jesli to kolorowy obraz
    {
        RGB_t** dane = (RGB_t**)obraz->data;    //Pojedynczy piksel sklada sie z 3 kolorow
        //Petla ktora zapisuje wszystkie piksele
        for (int i = 0; i < obraz->y; i++)
        {
            for (int j = 0; j < obraz->x; j++)
                fprintf(obraz->fileHandle, "%hi %hi %hi\t", dane[i][j].red, dane[i][j].green, dane[i][j].blue);
            fprintf(obraz->fileHandle, "\n");
        }
    }

    //Zamykamy plik i jednoczesnie sprawdzamy czy poprawnie sie zamknal
    if (fclose(obraz->fileHandle) == EOF)
    {
        ErrorOccurred(obraz, "Nie udalo sie zapisac pliku!");
        return;
    }

    obraz->status = good;
    return;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Odczyt(struct Obraz_t* obraz)
{
    int znak = 0;

    char* buf = (char*)malloc(sizeof(char) * DL_LINII);         // bufor pomocniczy do czytania naglowka i komentarzy
    if (buf == NULL)
    {
        ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Odczyt!");
        return;
    }                                        

    if (strcmp(obraz->nazwa, "stdin") == 0)
        obraz->fileHandle = stdin;
    else
        obraz->fileHandle = fopen(obraz->nazwa, "r");    //Realnie otwieramy plik w trybie czytania    

    if (obraz->fileHandle == NULL)                       //Sprawdzamy czy sie poprawnie otworzyl
    {
        ErrorOccurred(obraz, "Bledna sciezka do pliku!");
        return;
    }

    //Wczytanie pierwszej linii i sprawdzenie przy okazji czy udalo sie to zrobic
    if (fgets(buf, DL_LINII, obraz->fileHandle) == NULL)
    {
        ErrorOccurred(obraz, "Plik jest pusty!");
        return;
    }

    //Sprawdzenie czy jest to plik PGM formatu P2
    if (buf[0] == 'P' && buf[1] == '2')
        obraz->type = P2;
    //Albo formatu P3 PPM
    else if (buf[0] == 'P' && buf[1] == '3')
        obraz->type = P3;
    else
    {
        ErrorOccurred(obraz, "To nie jest plik formatu PGM ani PPM!");
        return;
    }

    //Pominiecie komentarzy
    do {
        znak = fgetc(obraz->fileHandle);
        if (znak == '#')     //Jesli linia rozpoczyna sie od znaku # to jest komentarzem 
        {
            if (fgets(buf, DL_LINII, obraz->fileHandle) == NULL)
            {
                ErrorOccurred(obraz, "Plik konczy sie na komentarzach!");
                return;
            }
            strcat(obraz->comments, "#");
            strcat(obraz->comments, buf);
        }
        else    //Jesli znak okazal sie nie # cofamy sie jeden do tylu bo to nie byl jednak komentarz
            ungetc(znak, obraz->fileHandle);

    } while (znak == '#');   //Powtarzamy dopoki sa to komentarze

    // Pobranie wymiarow obrazu i liczby odcieni szarosci
    if (fscanf(obraz->fileHandle, "%d %d %d", &obraz->x, &obraz->y, &obraz->maxSzarosc) != 3)
    {
        ErrorOccurred(obraz, "Brak wymiarow obrazu lub liczby stopni szarosci!");
        return;
    }

    if (obraz->type == P2)  //Jesli plik jest w skali szarosci
    {
        //Stworzenie tablicy dynamicznej na podstawie wymiarow
        if ( (obraz->data = malloc(sizeof(short*) * obraz->y)) == NULL)
        {
            ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Odczyt!");
            return;
        }
        for (int i = 0; i < obraz->y; i++)
            if ( (obraz->data[i] = malloc(sizeof(short) * obraz->x)) == NULL)
            {
                ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Odczyt!");
                return;
            }
        
        short** dane = (short**)obraz->data;
        // Pobranie obrazu i zapisanie w tablicy
        for (int i = 0; i < obraz->y; i++)
            for (int j = 0; j < obraz->x; j++)
                if (fscanf(obraz->fileHandle, "%hi", &(dane[i][j])) != 1)
                {
                    ErrorOccurred(obraz, "Niewlasciwe wymiary obrazu!");
                    return;
                }
    }
    else if (obraz->type == P3) //Jesli plik jest kolorowy
    {
        //Stworzenie tablicy dynamicznej na podstawie wymiarow
        if ((obraz->data = malloc(sizeof(RGB_t*) * obraz->y)) == NULL)
        {
            ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Odczyt!");
            return;
        }
        for (int i = 0; i < obraz->y; i++)
            if ((obraz->data[i] = malloc(sizeof(RGB_t) * obraz->x)) == NULL)
            {
                ErrorOccurred(obraz, "Blad podczas alokacji pamieci w funkcji Odczyt!");
                return;
            }

        RGB_t** dane = (RGB_t**)obraz->data;
        // Pobranie obrazu i zapisanie w tablicy
        for (int i = 0; i < obraz->y; i++)
            for (int j = 0; j < obraz->x; j++)
                if (fscanf(obraz->fileHandle, "%hi%hi%hi", &(dane[i][j].red), &(dane[i][j].green), &(dane[i][j].blue)) != 3)
                {
                    ErrorOccurred(obraz, "Niewlasciwe wymiary obrazu!");
                    return;
                }
    }

    //Zamykamy plik i jednoczesnie sprawdzamy czy poprawnie sie zamknal
    if (fclose(obraz->fileHandle) == EOF)
    {
        ErrorOccurred(obraz, "Nie udalo sie wczytac pliku!");
        return;
    }

    obraz->status = good;
    return;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ErrorOccurred(struct Obraz_t* obrazPGM, char* info)
{
    strcpy(obrazPGM->statusINFO, info);
    obrazPGM->status = bad;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Wyswietl(char* n_pliku)
{
    //Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"
    char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

    strcpy(polecenie, "display ");  /* konstrukcja polecenia postaci */
    strcat(polecenie, n_pliku);     /* display "nazwa_pliku" &       */
    strcat(polecenie, " &");
    system(polecenie);             /* wykonanie polecenia        */
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void DodajKomentarz(struct Obraz_t* obrazPGM, char* what)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char* temp[DL_LINII];
    sprintf(temp, "# %02d.%02d.%d %02d:%02d:%02d - ",
        tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    strcat(obrazPGM->comments, temp);
    strcat(obrazPGM->comments, what);
    strcat(obrazPGM->comments, "\n");
}