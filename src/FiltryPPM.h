#ifndef FILTRYPPM_H
#define FILTRYPPM_H
#include "Obraz.h"

/*
	Funkcja ktora naklada negatyw na obraz wczytany do obiektu
*/
void NegatywPPM(struct Obraz_t*);

/*
	Funkcja progujaca nasz obraz
	Jako argumenty progi dla odpowiednich kolorow
	Progi podajemy w % wi�c w zakresie [0;100] jako liczby ca�kowite

*/
void ProgowaniePPM(struct Obraz_t*, short pr, short pg,  short pb);

/*
	Wprowadza korekcje gamma dla podanego wspolczynnika gamma jako drugi argument
*/
void KorekcjaGammaPPM(struct Obraz_t*, float);

/*
	Filtr konturujacy obraz
	Funkcja moze zwrocic blad jesli nie uda jej sie zaalokowac pamieci dla kopii naszego obrazu
*/
void KonturowaniePPM(struct Obraz_t*);

/*
	Rozciaga obraz poziomo wedlug podanego jako drugi argument promienia
	Drugi argument musi byc dodatni oraz calkowity
	Funkcja moze zwrocic blad jesli nie uda jej sie zaalokowac pamieci dla kopii naszego obrazu
*/
void RozmywaniePoziomePPM(struct Obraz_t*, short);

/*
	Rozciaga obraz pionowo wedlug podanego jako drugi argument promienia
	Drugi argument musi byc dodatni oraz calkowity
	Funkcja moze zwrocic blad jesli nie uda jej sie zaalokowac pamieci dla kopii naszego obrazu
*/
void RozmywaniePionowePPM(struct Obraz_t*, short);

/*
	Rozciaga histogram do wartosci maksymalnych(o ile nie sa wykorzystane)
*/
void RozciaganieHistogramuPPM(struct Obraz_t*);

/*
	Stosuje filtr splotu na obrazku PPM
	Nalezy przyslac maske splotu w postaci Macierzy3x3
*/
void SplotPPM(struct Obraz_t*, Macierz3x3_t*);

/*
	Zmiana kolorowej bitmapy na bitmape w skali szarosci
	Dodatkowo zmienia typ obrazka z P3 na P2
	Zwalnia pami�� dla aktualnej tablicy pikseli kolorowych i rezerwuje pamiec dla tablicy w skali szaro�ci
*/
void Kolor2Szarosc(struct Obraz_t*);

#endif