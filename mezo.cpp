#include "mezo.h"

Mezo::Mezo(int az) : azonosito(az), figura(0)
{
    for(int i = 0; i < 4; i++) szomszedok[i] = 0;
}
Mezo *& Mezo::operator[](int i)
{
    return szomszedok[i];
}
const Mezo * Mezo::operator[](int i) const
{
    return szomszedok[i];
}
bool Mezo::foglalt() const
{
    return figura != 0;
}
void Mezo::setFigura(Figura * figura)
{
    this->figura = figura;
}
Figura * Mezo::getFigura() const
{
    return figura;
}
int Mezo::getAzonosito() const
{
    return azonosito;
}
void Mezo::setKoordinata(SDL_Point koord)
{
    koordinata = koord;
}
SDL_Point Mezo::getKoordinata() const
{
    return koordinata;
}
Mezo::~Mezo() {}

