#include "jatekos.h"
#include <iostream>
#include <SDL_image.h>
using Figura = Jatekos::Figura;

Jatekos::Jatekos(const char* nev, int figuraszam, Mezo * kezdo, const char* kepnev, SDL_Renderer* renderer) : nev(nev), figuraszam(figuraszam)
{
    figurak = new Figura*[figuraszam];
    for(int i = 0; i < figuraszam; i++)
        figurak[i] = new Figura(kezdo, this);
   kep = IMG_Load(kepnev);
    if(kep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
   texture = SDL_CreateTextureFromSurface(renderer,kep);
}
Figura *& Jatekos::operator[](int i)
{
    return figurak[i];
}
const Figura * Jatekos::operator[](int i) const
{
    return figurak[i];
}
const string Jatekos::getNev() const
{
    return nev;
}
Jatekos::~Jatekos()
{
    for(int i = 0; i < figuraszam; i++)
        delete figurak[i];
    delete[] figurak;
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(kep);
}

Figura::Figura(Mezo * mezo, Jatekos * tulaj) : tulaj(tulaj), mezo(mezo) {}
const Mezo* Figura::getMezo() const
{
    return mezo;
}
void Figura::setMezo(Mezo * mezo)
{
    this->mezo = mezo;
}
void Figura::setKoordinata(SDL_Point koord)
{
    koordinata = koord;
}
const SDL_Point& Figura::getKoordinata() const
{
    return koordinata;
}
const Jatekos* Figura::getTulaj() const
{
    return tulaj;
}
void Figura::setTulaj(Jatekos * t)
{
    tulaj = t;
}
void Jatekos::setKep(SDL_Surface* kep)
{
    this->kep = kep;
}
SDL_Surface* Jatekos::getKep() const
{
    return kep;
}
void Jatekos::setTexture(SDL_Texture* texture)
{
    this->texture = texture;
}
SDL_Texture* Jatekos::getTexture() const
{
    return texture;
}

Figura::~Figura()
{

}

