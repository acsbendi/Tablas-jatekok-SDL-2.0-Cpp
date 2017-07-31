#include "kigyokesletrakfigura.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

KigyokEsLetrakFigura::KigyokEsLetrakFigura(Mezo * mezo, Jatekos * tulaj, const char * szin, const char * kepnev, SDL_Renderer* renderer)
    : Figura(mezo,tulaj), szin(szin)
{
    kep = IMG_Load(kepnev);
    if(kep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer,kep);
}
const char* KigyokEsLetrakFigura::getSzin() const
{
    return szin.c_str();
}
void KigyokEsLetrakFigura::setSzin(const char* )
{
    this->szin = szin;
}
const SDL_Texture* KigyokEsLetrakFigura::getTexture() const
{
    return texture;
}
KigyokEsLetrakFigura::~KigyokEsLetrakFigura()
{
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(kep);
}

