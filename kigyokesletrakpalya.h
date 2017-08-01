#ifndef KIGYOKESLETRAKPALYA_H_INCLUDED
#define KIGYOKESLETRAKPALYA_H_INCLUDED
#include "palya.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


class KigyokEsLetrakPalya : public Palya
{
    /*
    konstansok a grafikához
    */
    static const int ABLAK_SZELESSEG = 904;
    static const int ABLAK_MAGASSAG = 704;
    static const int PALYA_SZELESSEG = 704;
    static const int PALYA_MAGASSAG = 704;
    static const int GOMB_SZELESSEG = 150;
    static const int GOMB_MAGASSAG = 42;
    static const int BABU_SZELESSEG = 19;
    static const int BABU_MAGASSAG = 30;
    static const int BABU_VEGE_SZELESSEG = 60;
    static const int BABU_VEGE_MAGASSAG = 100;
    static const int GOMB_X = 730;
    static const int GOMB_Y = 550;
    static const int DOBO_GOMB_ATMERO = 175;
    static const int DOBO_GOMB_X = 715;
    static const int DOBO_GOMB_Y = 500;
    static const int VEGE_GOMB_SZELESSEG = 175;
    static const int VEGE_GOMB_MAGASSAG = 47;
    static const int VEGE_GOMB_Y = 500;
    static const int NOVELO_X = 765;
    static const int NOVELO_Y = 220;
    static const int NOVELO_SZELESSEG = 75;
    static const int NOVELO_MAGASSAG = 57;
    static const int CSOKKENTO_Y = 420;
    static const int KOSZORU_SZELESSEG = 150;
    static const int KOSZORU_MAGASSAG = 150;
    static const int MEZO_MERET = 66;
    static const int ANIMACIO_KESLELTETES = 33;
    static const int DOBASKEP_SZELESSEG = 130;
    static const int DOBASKEP_MAGASSAG = 280;
    static const int DOBASKEP_Y = 140;
    static const int DOBOKEP_SZELESSEG = 150;
    static const int DOBOKEP_MAGASSAG = 150;
    const SDL_Color SZOVEGSZIN = {24,22,22}; /**< A pályán látható számok színe */


    /*
    SDL/grafikus változók
    */
    SDL_Window* ablak = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* palyakep = NULL;
    SDL_Texture* palyakeptexture = NULL;
    SDL_Surface* vilagosmezo = NULL;
    SDL_Texture* vilagosmezotexture = NULL;
    SDL_Surface* vilagosmezo2 = NULL;
    SDL_Texture* vilagosmezo2texture = NULL;
    SDL_Surface* sotetmezo = NULL;
    SDL_Texture* sotetmezotexture = NULL;
    SDL_Surface* sotetmezo2 = NULL;
    SDL_Texture* sotetmezo2texture = NULL;
    SDL_Surface* hatter = NULL;
    SDL_Texture* hattertexture = NULL;
    SDL_Surface* novelo = NULL;
    SDL_Texture* novelotexture = NULL;
    SDL_Surface* gomb = NULL;
    SDL_Texture* gombtexture = NULL;
    SDL_Surface* vegegomb = NULL;
    SDL_Texture* vegegombtexture = NULL;
    SDL_Surface* dobasgomb = NULL;
    SDL_Texture* dobasgombtexture = NULL;
    SDL_Surface* dobaskep = NULL;
    SDL_Texture* dobaskeptexture = NULL;
    SDL_Surface* dobokep = NULL;
    SDL_Texture* dobokeptexture = NULL;
    SDL_Surface* zoldhatter = NULL;
    SDL_Texture* zoldhattertexture = NULL;
    SDL_Surface* vegehatterkep = NULL;
    SDL_Texture* vegehatterkeptexture = NULL;
    SDL_Surface* koszoru = NULL;
    SDL_Texture* koszorutexture = NULL;
    TTF_Font* szamfont = NULL;
    TTF_Font* betufont = NULL;
    SDL_Surface* szoveg = NULL;
    SDL_Texture* szovegtexture = NULL;
    SDL_Rect hely;      /**< A megjenítéshez szabadon átírandó hely a képernyőn */



    /*!
    \brief Kattintás eseményt vár

    \param x A kattintás x koordinátáját állítja be
    \param y A kattintás y koordinátáját állítja be

    \return int Siker esetén 1, amúgy 0
    */
    int kattintas(int& x, int& y);

    /*!
    \brief Megjelenít egy figurát egy mezőn

    \param mezo A mező amin meg kell jeleníteni (1-100)
    \param figura A figura amit meg kell jeleníteni (0-5)
    */
    void figuramezon(const Mezo* mezo, int figura);

    /** \brief Az üres pályát (bábúk nélkül) megrajzoló metódus
     *
     * \return void
     *
     */
    void urespalyarajzol();

    /**
    \brief Dobás esemény vizuális részét kezeli- kattintást vár, megmutatja a dobás eredményét

    \param szam A véletlenszerűen sorsolt dobott szám
    */
    void dobas(int szam);


    /** \brief Itt történik a grafikus felületen a játékosok számának kiválasztása a felhasználó által
     *
     * \return int A felhasználó által kiválasztott játékosok száma
     *
     */
    int jatekosokszamakivalaszt();

    /** \brief Következő lépés feldolgozása
     *
     * \return void
     *
     */
    void lep();

    /** \brief Betölti a grafikus felülethez szükséges ablakot, képeket stb
     *
     * \return void
     *
     */
    void grafikabetolt();


    /** \brief Létrehozza és beállítja a mezőket (egymás közti kapcsolatokat)
     *
     * \param meret int A pálya mérete, azaz hány mezőböl álljon - grafikus felülethez 100 kell
     * \param kapcsolatok const char* Szövegesen tartalmazza a speciális mezőket, azaz, ahol kígyó vagy létra kezdődik/végződik - grafikus felülethez adott
     * \return void
     *
     */
    void mezokbeallit(int meret, const char* kapcsolatok);


    /** \brief Megrajzolja a játékosszám kiválasztásához szükséges grafikus felületet
     *
     * \param temp int  Jelenleg kiválasztott játékosszám
     * \return void
     *
     */
    void valasztorajzol(int temp);

    /** \brief Megrajzolja a pályát, figurákkal együtt
     *
     * \return void
     *
     */
    void palyarajzol();


    /** \brief Megrajzolja a jobb oldalon található felületet, rajta a dobást indító gombbal
     *
     * \return void
     *
     */
    void doborajzol();


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


    /** \brief Megjelenít egy rövid kockadobás animációt
     *
     * \param dobas int A dobott szám
     * \return void
     *
     */
    void dobasanimacio(int dobas);


    /** \brief Megjeleníti a vége képernyőt
     *
     * \param nyertes int A nyertes játékos
     * \return void
     *
     */
    void vegemutat(int nyertes);


public:
    /** \brief Pálya létrehozása, a kapcsolatok szövegesen tartalmazza a speciális mezőket,
     *
     * \param 100 int meret= A pálya mérete, azaz hány mezőböl álljon - grafikus felülethez 100 kell
     * \param const char* kapcsolatok = "" Szövegesen tartalmazza a speciális mezőket, azaz, ahol kígyó vagy létra kezdődik/végződik - grafikus felülethez adott
     *
     */
    KigyokEsLetrakPalya(int meret = 100, const char* kapcsolatok = "");

    /** \brief Itt folyik a játék a pálya létrehozása után
     *
     * \return void
     *
     */
    void jatek();

    ~KigyokEsLetrakPalya();
};

#endif // KIGYOKESLETRAKPALYA_H_INCLUDED
