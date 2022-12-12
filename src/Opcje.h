#ifndef OPCJE_H
#define OPCJE_H

#include <stdio.h>
#include <string.h>

//Struktura na potrzeby funkcji splot
typedef struct
{
    int tab[3][3];
} Macierz3x3_t;

// Wykorzsytane tu funkcje to czêœciowo zmodyfikowany program pobrany ze strony https://kcir.pwr.edu.pl/  

#define W_OK                0       /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA  -1      /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY         -2
#define B_BRAKWARTOSCI      -3
#define B_BRAKPLIKU         -4
#define B_BRAKKOLORU        -5
#define B_ZLYKOLOR          -6

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct
{
    char inputFileName[100], outputFileName[100];           //Nazwy plikow wejsciowych i wyjsciowych
    char wyswietlenie, kRed, kGreen, kBlue;
    short prog, progCzerni, progBieli, zpBiel, zpCzern,     //Wartosci dla konkretnych funkcji
        rozmycieXwartosc, rozmycieYwartosc;
    float gammaWartosc;
    Macierz3x3_t maskaSplotu;
    char listaZadan[100];                                   //Lista zadan przechowuje kolejne zadania
} Opcje_t;

/*******************************************************/
/* Funkcja inicjuje strukture wskazywana przez wybor   */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/
void OpcjeKonstruktor(Opcje_t*);

/************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv  */
/* i zapisuje je w strukturze wybor                                     */
/* Skladnia opcji wywolania programu                                    */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-r] [-d] }  */
/* Argumenty funkcji:                                                   */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu   */
/*         argv  -  tablica argumentow wywolania                        */
/*         wybor -  struktura z informacjami o wywolanych opcjach       */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wybór, do tego ustawia na 1 pola dla opcji, ktore  */
/*	poprawnie wystapily w linii wywolania programu,                     */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;         */
/*	zwraca wartosc W_OK (0), gdy wywolanie bylo poprawne                */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                         */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/
int PrzetwarzajOpcje(int, char**, Opcje_t*);

#endif
