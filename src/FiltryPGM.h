#ifndef FILTRYPGM_H
#define FILTRYPGM_H
#include "Obraz.h"

/*
	Funkcja ktora naklada negatyw na obraz wczytany do obiektu
*/
void NegatywPGM(struct Obraz_t*);

/*
	Funkcja progujaca nasz obraz
	Jako drugi argument przyjmuje wartosc w procentach progowania
	Procenty musza byc z przedzialu [0;100] i byc liczba calkowita
*/
void ProgowaniePGM(struct Obraz_t*, short);

/*
	Filtr ktory zajmuje sie polprogowaniem czerni
	Jako drugi argument przyjmuje wartosc w procentach progowania
	Procenty musza byc z przedzialu [0;100] i byc liczba calkowita
*/
void PolprogowanieCzerniPGM(struct Obraz_t*, short);

/*
	Filtr ktory zajmuje sie polprogowaniem bieli
	Jako drugi argument przyjmuje wartosc w procentach progowania
	Procenty musza byc z przedzialu [0;100] i byc liczba calkowita
*/
void PolprogowanieBieliPGM(struct Obraz_t*, short);

/*
	Filtr ktory zajmuje sie zmiana wartosci czerni i bieli
	Jako drugi  argument przyjmuje nowa wartosc dla CZERNI w procentach
	Jako trzeci argument przyjmuje nowa wartosc dla BIELI  w procentach
	Procenty musza byc z przedzialu [0;100] i byc liczba calkowita
*/
void ZmianaPoziomowPGM(struct Obraz_t*, short, short);

/*
	Wprowadza korekcje gamma dla podanego wspolczynnika gamma jako drugi argument
*/
void KorekcjaGammaPGM(struct Obraz_t*, float);

/*
	Filtr konturujacy obraz
	Funkcja moze zwrocic blad jesli nie uda jej sie zaalokowac pamieci dla kopii naszego obrazu
*/
void KonturowaniePGM(struct Obraz_t*);

/*
	Rozciaga obraz poziomo wedlug podanego jako drugi argument promienia
	Drugi argument musi byc dodatni oraz calkowity
	Funkcja moze zwrocic blad jesli nie uda jej sie zaalokowac pamieci dla kopii naszego obrazu
*/
void RozmywaniePoziomePGM(struct Obraz_t*, short);

/*
	Rozciaga obraz pionowo wedlug podanego jako drugi argument promienia
	Drugi argument musi byc dodatni oraz calkowity
	Funkcja moze zwrocic blad jesli nie uda jej sie zaalokowac pamieci dla kopii naszego obrazu
*/
void RozmywaniePionowePGM(struct Obraz_t*, short);

/*
	Rozciaga histogram do wartosci maksymalnych(o ile nie sa wykorzystane)
*/
void RozciaganieHistogramuPGM(struct Obraz_t*);

/*
	Stosuje filtr splotu na obrazku PGM
	Nalezy przyslac maske splotu w postaci Macierzy3x3
*/
void SplotPGM(struct Obraz_t*, Macierz3x3_t*);

#endif