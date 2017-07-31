#ifndef MALOMPALYA_H_INCLUDED
#define MALOMPALYA_H_INCLUDED
#include "palya.h"
#include <SDL.h>
#include <SDL_ttf.h>

class MalomPalya : public Palya{

    /*
    konstansok
    */
    static const int ABLAK_SZELESSEG = 400;
    static const int ABLAK_MAGASSAG = 600;
    static const int KEP_MERET = 40;
    static const int VEGE_GOMB_Y = 450;
    static const int VEGE_GOMB_SZELESSEG = 100;
    static const int VEGE_GOMB_MAGASSAG = 50;
    const SDL_Color FEHER_BETUSZIN = {207, 149, 66};
    const SDL_Color FEKETE_BETUSZIN = {50, 30, 10};
    const SDL_Color KIJELOLOSZIN =  {88,93,102};



    bool elsoszakasz;       /**< A játék az első szakaszban van-e, azaz a figurák lerakási szakaszában  */
    int feherfiguraszam;    /**< Fehér eddig hány figurája került lerakásra (játék 1. szakaszában) */
    int feketefiguraszam;   /**< Fekete eddig hány figurája került lerakásra (játék 1. szakaszában) */
    int malmok[16][3];      /**< A lehetéges malmok tömbje, a mezők azonosítóját tárolja */
    bool vege;              /**< A játék végét jelzi, azért van rá szükség, mert többféleképpen is végződhet */

    /*
    SDL/grafikus változók
    */
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* malomkep = NULL;
    SDL_Texture* malomkeptexture = NULL;
    SDL_Surface* feherhatterkep = NULL;
    SDL_Texture* feherhatterkeptexture = NULL;
    SDL_Surface* feketehatterkep = NULL;
    SDL_Texture* feketehatterkeptexture = NULL;
    SDL_Surface* vegehatterkep = NULL;
    SDL_Texture* vegehatterkeptexture = NULL;
    SDL_Surface* vegehatterkupakep = NULL;
    SDL_Texture* vegehatterkupakeptexture = NULL;
    SDL_Surface* gombkep = NULL;
    SDL_Texture* gombkeptexture = NULL;
    TTF_Font* betufont = NULL;
    TTF_Font* szamfont = NULL;
    SDL_Surface* szoveg = NULL;
    SDL_Texture* szovegtexture = NULL;
    SDL_Rect hely;      /**< a képernyő rajzolások helyét ennek segítségével lehet beállítani */


    /** \brief Betölti a grafikus felülethez szükséges ablakot, képeket stb
     *
     * \return void
     *
     */
    void grafikabetolt();

    /** \brief Teszteli, hogy vége van-e már a játéknak
     *
     * \return void
     *
     */
    void vegeteszt();

    /** \brief Megadja, hogy egy adott mezőn álló bábú alkot-e malmot
     *
     * \param uj const Mezo* A vizsgálandó mező
     * \return int Ha nincs -1-et ad vissza, ha van, a sorszámát a malmok tömbjében
     *
     */
    int malomvizsgal(const Mezo * uj);

    /** \brief Esetlegesen keletkező malmot kezeli
     *
     * \param mezo Mezo* A vizsgálandó mező
     * \return void
     *
     */
    void malom(Mezo* mezo);

    /** \brief Megadja, hogy az adott játékosnak 3 figurája van-e, így bármely üres mezőre léphet
     *
     * \param jatekos int A vizsgálandó figura
     * \return bool Léphet-e bárhova
     *
     */
    bool barhova(int jatekos);


    /** \brief A bábuk lerakásátnak szakaszát kezeli
     *
     * \return void
     *
     */
    void elsoszakaszlep();


    /** \brief A babuk lépésének szakaszát kezeli
     *
     * \return void
     *
     */
    void masodikszakaszlep();

    /** \brief Létrehozza és beállítja mezőket, azok helyeit, kapcsolatait és a lehetséges malmok mezőit
     *
     * \return void
     *
     */
    void mezokbeallit();

    /** \brief Megjeleníti valamelyik játékos korongjának képét egy adott pontban
     *
     * \param pont SDL_Point A pont, ahol a kép megjelenljen (a kép közepe lesz itt)
     * \param kep SDL_Texture* A megjelenítendő kép/textura
     * \return void
     *
     */
    void kepmegjelenit(SDL_Point pont,SDL_Texture* kep);

    /** \brief Megjelenit egy szöveget a képernyőn
     *
     * \param s const char* A megjenenítendő szöveg
     * \param x int A megjelenítés helyének x koordinátája
     * \param y int A megjelenítés helyének y koordinátája
     * \param szin SDL_Color A szöveg színe
     * \param font TTF_Font* A szöveg betűtípusa
     * \return void
     *
     */
    void szovegmegjelenit(const char* s, int x, int y, SDL_Color szin, TTF_Font * font);

    /** \brief Újrarajzolja a játék közbeni keretet
     *
     * \param felso bool Igaz, ha a felső kereretet kell újrarajzolni
     * \return void
     *
     */
    void keretujrarajzol(bool felso);

     /** \brief Megrajzolja a korong levevésekor szükséges keretet
     *
     * \param felso bool Igaz, ha a felső kereretet kell megrajzolni
     * \return void
     *
     */
    void keretleveves(bool felso);

    /** \brief Újrarajzolja a pályát, vagyis a játékteret
     *
     * \return void
     *
     */
    void palyaujrarajzol();

    /** \brief Megvárja, hogy a felhasználó egy mezőre kattintson, és visszadja azt
     *
     * \param mezo int& A kattintott mező, referencia paraméterben visszaadva
     * \return void
     *
     */
    void kattintasmezon(int & mezo);

    /** \brief Egy adott mező köré kijelölést rajzol
     *
     * \param mezo int A vizuálisan kijelölendő mező
     * \return void
     *
     */
    void mezokijelol(int mezo);


    /** \brief Megjeleníti a végső képernyőt
     *
     * \param feher bool A nyertes a fehér-e
     * \return void
     *
     */
    void vegemutat(bool feher);
public:
    MalomPalya();
    ~MalomPalya();


    /** \brief Itt folyik a játék
     *
     * \return void
     *
     */
    void jatek();
};

#endif // MALOMPALYA_H_INCLUDED
