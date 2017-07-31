#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>
#include <SDL_ttf.h>
#include <string>
#include "malompalya.h"
#include "kigyokesletrakpalya.h"
#include <SDL_image.h>
#include <sstream>



class Alkalmazas
{
    /** \brief A felhasználó által jelenleg kiválasztott játék
     */
    enum Kivalasztott { malom, kigyokesletrak, semmi };
    Kivalasztott kivalasztott = semmi;
    static const int ABLAK_SZELESSEG = 640;
    static const int ABLAK_MAGASSAG = 480;
    const SDL_Color BETUSZIN = {44, 127, 3};
    const SDL_Color KIJELOLOSZIN =  {88,93,102};
    static const int KIJELOLOALPHA = 130;
    SDL_Window* ablak = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* kezdokep = NULL;
    SDL_Texture* kezdokeptexture = NULL;

public:
    Alkalmazas()
    {
        kezdokep = SDL_LoadBMP("kezdo2.bmp");
        if(kezdokep == NULL)
        {
            std::cout << "Képhiba !!! " << SDL_GetError();
            return;
        }

        if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            std::cout << "SDL Hiba !!! " << SDL_GetError() << std::endl;
            return;
        }


        ablak = SDL_CreateWindow( "Táblás játékok", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ABLAK_SZELESSEG, ABLAK_MAGASSAG, SDL_WINDOW_SHOWN );
        if( ablak == NULL )
        {
            std::cout <<  "Ablak hiba !!! " <<  SDL_GetError() << std::endl;
            return;
        }
        renderer = SDL_CreateRenderer(ablak, -1, SDL_RENDERER_ACCELERATED);
        kezdokeptexture = SDL_CreateTextureFromSurface(renderer, kezdokep);
        alappalya();
        SDL_RenderPresent(renderer);
    }

    /** \brief Elindítja az alkalmazást, itt folyik a főmenü kezelése
     *
     * \return void
     *
     */
    void start()
    {
        SDL_Event event;
        while(SDL_WaitEvent(&event) && event.type != SDL_QUIT) //felhasználói beavatkozásra várunk
        {
            switch(event.type)
            {
            case SDL_MOUSEMOTION: //az egér mozgatása
                if(event.motion.x < 264 + event.motion.y*((double)100/480)) //a bal oldalon van, azaz a malom van kijelölve
                {
                    if(kivalasztott != malom) malomkijelolve();
                    kivalasztott = malom;
                }
                else if(event.motion.x > 275 + event.motion.y*((double)100/480)) //a jobb oldalon van, azaz a kígyók és létrák van kijelölve
                {
                    if(kivalasztott != kigyokesletrak) kigyokesletrakkijelolve();
                    kivalasztott = kigyokesletrak;
                }
                else  //középső elválasztó sávon van, semmi nincs kijelölve
                {
                    kivalasztott = semmi;
                    alappalya();
                    SDL_RenderPresent(renderer);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:   //egérkattintás
                switch(kivalasztott)    //hol történt a kattintás
                {
                case malom:
                    SDL_HideWindow(ablak);  //amíg folyik a játék, a főmenüre nincs szükség..
                    malomindit();
                    SDL_Delay(1000);    //megvárjuk, míg a játék rendben bezárul, ez némi időt vesz igénybe
                    SDL_ShowWindow(ablak);
                    malomkijelolve();
                    break;
                case kigyokesletrak:
                    SDL_HideWindow(ablak); //amíg folyik a játék, a főmenüre nincs szükség..
                    kigyokesletrakindit();
                    SDL_Delay(1000);    //megvárjuk, míg a játék rendben bezárul, ez némi időt vesz igénybe
                    SDL_ShowWindow(ablak);
                    kigyokesletrakkijelolve();
                    break;
                case semmi:
                    break;
                }

                break;
            }
        }
    }

    /** \brief A főmenü alapállapotát állítja vissza
     *
     * \return void
     *
     */
    void alappalya()
    {
        SDL_RenderCopy(renderer,kezdokeptexture,0,0);
        SDL_RenderPresent(renderer);
        SDL_SetWindowTitle(ablak,"Táblás játékok");
    }

    /** \brief A főmenün mutatja, hogy a malom van kijelölve
     *
     * \return void
     *
     */
    void malomkijelolve()
    {
        alappalya();
        SDL_SetWindowTitle(ablak,"Malom");
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        thickLineRGBA(renderer,256,0,356,480,15,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,KIJELOLOALPHA);
        thickLineRGBA(renderer,0,5,254,5,20,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,KIJELOLOALPHA);
        thickLineRGBA(renderer,5,11,5,480,20,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,KIJELOLOALPHA);
        thickLineRGBA(renderer,10,475,354,475,20,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,KIJELOLOALPHA);
        SDL_RenderPresent(renderer);
    }

    /** \brief A főmenün mutatja, hogy a kígyók és létrák van kijelölve
     *
     * \return void
     *
     */
    void kigyokesletrakkijelolve()
    {
        alappalya();
        SDL_SetWindowTitle(ablak,"Kígyók és látrák");
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        thickLineRGBA(renderer,290,10,640,10,20,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,KIJELOLOALPHA);
        thickLineRGBA(renderer,284,0,384,480,15,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,KIJELOLOALPHA);
        thickLineRGBA(renderer,630,15,630,480,20,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,KIJELOLOALPHA);
        thickLineRGBA(renderer,388,470,620,470,20,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,KIJELOLOALPHA);
        SDL_RenderPresent(renderer);
    }

    /** \brief Elindítja a kígyók és létrák játékot, új ablakot nyit
     *
     * \return void
     *
     */
    void kigyokesletrakindit()
    {
        KigyokEsLetrakPalya kigyokesletrakpalya(100, "34-1 3-51 25-5 6-27 47-19 20-70 36-55 65-52 87-57 91-61 63-95 68-98 99-69");
        kigyokesletrakpalya.jatek();
    }

    /** \brief Elindítja a malom játékot, új ablakot nyit
     *
     * \return void
     *
     */
    void malomindit()
    {
        MalomPalya malompalya;
        malompalya.jatek();
    }

    ~Alkalmazas()
    {
        SDL_FreeSurface(kezdokep);
        SDL_DestroyTexture(kezdokeptexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ablak);
        SDL_Quit();
    }
};




int main( int argc, char* args[] )
{
    Alkalmazas alkalmazas;  //létrehozzuk ..
    alkalmazas.start();     //és elindítjuk az alkalmazást
    return 0;
}
