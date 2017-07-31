#ifndef PALYA_H_INCLUDED
#define PALYA_H_INCLUDED
#include "mezo.h"
#include "jatekos.h"


class Palya{
protected:
    Mezo ** mezok;          /**< Mezők tömbje, tetszőleges számú lehet->dinamikusan foglalt */
    Jatekos * jatekosok[6]; /**< Játékosok tömbje, maximum 6 játékos */
    int jelenlegi;          /**< Jelenleg lépő játékos */
    int jatekosokszama;     /**< Jelenlegi játékot hány játékos játssza */
    int mezokszama;         /**< Jelenlegi játék hány mezőt használ */
    Palya();
public:
    virtual void jatek() = 0;
    virtual ~Palya();
};





#endif // PALYA_H_INCLUDED
