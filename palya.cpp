#include "palya.h"

Palya::Palya() {}
Palya::~Palya()
{
    for(int i = 0; i < mezokszama; i++)
    {
        delete mezok[i];
    }
    delete[] mezok;
    for(int i = 0; i < jatekosokszama; i++)
        delete jatekosok[i];
}


