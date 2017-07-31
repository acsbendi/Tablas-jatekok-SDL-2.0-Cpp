#ifndef MEZO_H_INCLUDED
#define MEZO_H_INCLUDED
#include "jatekos.h"
#include <SDL.h>
using Figura = Jatekos::Figura;

class Mezo{
protected:
    const int azonosito;
    Mezo * szomszedok[4];
    Figura * figura;
    SDL_Point koordinata; //x,y koordinátákat tároló struktúra a grafikus megjelentéshez
public:
    Mezo(int az);
    Mezo *& operator[](int i);
    const Mezo * operator[](int i) const;
    bool foglalt() const;
    void setFigura(Figura * figura);
    Figura * getFigura() const;
    int getAzonosito() const;
    void setKoordinata(SDL_Point koord);
    SDL_Point getKoordinata() const;
    virtual ~Mezo();
};

#endif // MEZO_H_INCLUDED
