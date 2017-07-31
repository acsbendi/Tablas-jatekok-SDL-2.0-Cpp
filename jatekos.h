#ifndef JATEKOS_H_INCLUDED
#define JATEKOS_H_INCLUDED
#include <string>
#include <SDL.h>

using std::string;

class Mezo;

class Jatekos
{
public:
    class Figura
    {
    protected:
        Jatekos * tulaj;
        Mezo * mezo;
        SDL_Point koordinata; /**< x,y koordinátákat tároló struktúra a grafikus megjelenítéshez */
    public:
        Figura(Mezo * mezo, Jatekos * tulaj);
        const Mezo* getMezo() const;
        void setMezo(Mezo * mezo);
        const SDL_Point& getKoordinata() const;
        void setKoordinata(SDL_Point koord);
        const Jatekos* getTulaj() const;
        void setTulaj(Jatekos * t);
        virtual ~Figura();
    };

protected:
    string nev;
    Jatekos::Figura** figurak;
    SDL_Surface* kep;
    SDL_Texture* texture;
    int figuraszam;
public:
    Jatekos(const char* nev, int figuraszam, Mezo * kezdo, const char* kepnev, SDL_Renderer* renderer);
    Figura *& operator[](int i);
    const Figura * operator[](int i) const;
    const string getNev() const;
    SDL_Surface* getKep() const;
    void setKep(SDL_Surface* kep);
    SDL_Texture* getTexture() const;
    void setTexture(SDL_Texture* texture);
    virtual ~Jatekos();
};

#endif // JATEKOS_H_INCLUDED
