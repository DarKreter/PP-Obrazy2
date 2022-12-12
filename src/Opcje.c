#include "Opcje.h"

int PrzetwarzajOpcje(int argc, char** argv, Opcje_t* wybor)
{
    int i, prog, prog2;
    float progG;
    char* nazwa_pliku_we = NULL, * nazwa_pliku_wy = NULL;
    OpcjeKonstruktor(wybor);
    strcpy(wybor->outputFileName, "stdout");

    for (i = 1; i < argc; i++) 
    {
        if (argv[i][0] != '-')  /* blad: to nie jest opcja - brak znaku "-" */
            return B_NIEPOPRAWNAOPCJA;

        switch (argv[i][1]) 
        {
        case 'i': {                                     /* opcja z nazwa pliku wejsciowego */
            if (++i < argc)                             /* wczytujemy kolejny argument jako nazwe pliku */
            {
                nazwa_pliku_we = argv[i];
                if (strcmp(nazwa_pliku_we, "-") == 0)   /* gdy nazwa jest "-"        */
                    strcpy(wybor->inputFileName, "stdin");
                else
                    strcpy(wybor->inputFileName, nazwa_pliku_we);
            }
            else
                return B_BRAKNAZWY;                     /* blad: brak nazwy pliku */
            break;
        }
        case 'o': {                                     /* opcja z nazwa pliku wyjsciowego */
            if (++i < argc)                             /* wczytujemy kolejny argument jako nazwe pliku */
            {
                nazwa_pliku_wy = argv[i];
                if (strcmp(nazwa_pliku_wy, "-") == 0)   /* gdy nazwa jest "-"         */
                    strcpy(wybor->outputFileName, "stdout");
                else
                    strcpy(wybor->outputFileName, nazwa_pliku_wy);

            }
            else
                return B_BRAKNAZWY;                     /* blad: brak nazwy pliku */
            break;
        }
        case 'p': {
            if (argv[i][2] == 'c')
                goto pc;
            if (argv[i][2] == 'b')
                goto pb;

            if (++i < argc) { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%d", &prog) == 1) 
                {
                    strcat(wybor->listaZadan, "p");
                    wybor->prog = prog;
                }
                else
                    return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
            break;
        }
        case 'pc': {
            pc:
            if (++i < argc) { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%d", &prog) == 1)
                {
                    strcat(wybor->listaZadan, "c");
                    wybor->progCzerni = prog;
                }
                else
                    return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
            break;
        }
        case 'pb': {
            pb:
            if (++i < argc) { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%d", &prog) == 1)
                {
                    strcat(wybor->listaZadan, "b");
                    wybor->progBieli = prog;
                }
                else
                    return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
            break;
        }
        case 'g': {
            if (++i < argc) { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%f", &progG) == 1)
                {
                    strcat(wybor->listaZadan, "g");
                    wybor->gammaWartosc = progG;
                }
                else
                    return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
            break;
        }
        case 'r':
            if (argv[i][2] == 'x')
                goto rx;
            if (argv[i][2] == 'y')
                goto ry;

            break;
        case 'rx': {
            rx:
            if (++i < argc) { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%d", &prog) == 1)
                {
                    strcat(wybor->listaZadan, "x");
                    wybor->rozmycieXwartosc = prog;
                }
                else
                    return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
            break;
        }
        case 'ry': {
            ry:
            if (++i < argc) { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%d", &prog) == 1)
                {
                    strcat(wybor->listaZadan, "y");
                    wybor->rozmycieYwartosc = prog;
                }
                else
                    return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
            break;
        }
        case 'z': {
            if ( (i = i + 2) < argc) { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%d%d", &prog, &prog2) == 2)
                {
                    strcat(wybor->listaZadan, "z");
                    wybor->zpCzern = prog;
                    wybor->zpBiel = prog;
                }
                else
                    return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
            break;
        }
        case 'm': {
            if (++i < argc)
            {
                for (int j = 0; argv[i][j]; j++)
                {
                    switch (argv[i][j])
                    {
                    case 'r':
                        wybor->kRed = 1;
                        break;
                    case 'g':
                        wybor->kGreen = 1;
                        break;
                    case 'b':
                        wybor->kBlue = 1;
                        break;
                    case 's':
                        strcat(wybor->listaZadan, "m");
                        break;
                    default:
                        return B_ZLYKOLOR;
                    }
                }
            }
            else
                return B_BRAKKOLORU;
            break;
        }
        case 'n': {                                       /* mamy wykonac negatyw */
            strcat(wybor->listaZadan, "n");
            break;
        }
        case 'k': {                                     /* mamy wykonac konturowanie */
            strcat(wybor->listaZadan, "k");
            break;
        }
        case 'h': {                                     /* mamy wykonac konturowanie */
            strcat(wybor->listaZadan, "h");
            break;
        }
        case 's': {                                     /* mamy wykonac konturowanie */
            if ( (i = i + 9) < argc) { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%d%d%d%d%d%d%d%d%d", &(wybor->maskaSplotu.tab[0][0]), &(wybor->maskaSplotu.tab[0][1])
                    , &(wybor->maskaSplotu.tab[0][2]), &(wybor->maskaSplotu.tab[1][0]), &(wybor->maskaSplotu.tab[1][1])
                    , &(wybor->maskaSplotu.tab[1][2]), &(wybor->maskaSplotu.tab[2][0]), &(wybor->maskaSplotu.tab[2][1])
                    , &(wybor->maskaSplotu.tab[2][2])) == 9)
                {
                    strcat(wybor->listaZadan, "s");
                }
                else
                    return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
            break;
        }
        case 'd': {                                     /* mamy wyswietlic obraz */
            wybor->wyswietlenie = 1;
            break;
        }
        default:                                        /* nierozpoznana opcja */
            return B_NIEPOPRAWNAOPCJA;
        } /*koniec switch */

    } /* koniec for */

    if (strcmp(wybor->inputFileName, "") != 0)     /* ok: wej. strumien danych zainicjowany */
        return W_OK;
    else
        return B_BRAKPLIKU;                         /* blad:  nie otwarto pliku wejsciowego  */

}
///---------------------------------------------------------------------
///---------------------------------------------------------------------
///---------------------------------------------------------------------
void OpcjeKonstruktor(Opcje_t* wybor)
{

    strcpy(wybor->inputFileName,  "");
    strcpy(wybor->outputFileName, "");
    strcpy(wybor->listaZadan, "");

    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            wybor->maskaSplotu.tab[i][j] = 0;

    wybor->gammaWartosc = 0;
    wybor->rozmycieYwartosc = 0;
    wybor->rozmycieXwartosc = 0;
    wybor->zpBiel = 0;
    wybor->zpCzern = 0;
    wybor->progCzerni = 0;
    wybor->progBieli = 0;
    wybor->wyswietlenie = 0;
    wybor->kBlue = 0;
    wybor->kGreen = 0;
    wybor->kRed = 0;
    wybor->prog = 0;
}
