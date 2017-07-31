#include <iostream>
#include <algorithm>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include "seged.h"
#include "malompalya.h"

using std::find;
using Figura = Jatekos::Figura;

void MalomPalya::mezokijelol(int mezo)
{
    filledCircleRGBA(renderer,mezok[mezo-1]->getKoordinata().x,mezok[mezo-1]->getKoordinata().y,25,KIJELOLOSZIN.r,KIJELOLOSZIN.g,KIJELOLOSZIN.b,255);
    kepmegjelenit(mezok[mezo-1]->getKoordinata(), mezok[mezo-1]->getFigura()->getTulaj()->getTexture());
    SDL_RenderPresent(renderer);
}

void MalomPalya::szovegmegjelenit(const char* s, int x, int y, SDL_Color szin, TTF_Font * font)
{
    int w,h;
    TTF_SizeUTF8(font, s, &w, &h);
    SDL_FreeSurface(szoveg);
    szoveg = TTF_RenderUTF8_Blended(font,s,szin);
    SDL_DestroyTexture(szovegtexture);
    szovegtexture =  SDL_CreateTextureFromSurface(renderer, szoveg);
    hely = {x,y,w/3,h/3};
    SDL_RenderCopy(renderer,szovegtexture,0,&hely);
    SDL_RenderPresent(renderer);
}

void MalomPalya::kattintasmezon(int & mezo)
{
    SDL_Event event;
    while(SDL_WaitEvent(&event) && event.type != SDL_QUIT)
    {
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)
                for(mezo = 1; mezo < 25; mezo++)
                    if( (mezok[mezo - 1]->getKoordinata().x - event.button.x)*(mezok[mezo - 1]->getKoordinata().x - event.button.x) +
                            (mezok[mezo - 1]->getKoordinata().y - event.button.y)*(mezok[mezo - 1]->getKoordinata().y - event.button.y)  < 20*20 )
                        return;

            break;
        }
    }
}

void MalomPalya::keretujrarajzol(bool felso)
{
    hely = {0,felso ? 0 : 500,400,100};
    SDL_RenderCopy(renderer,felso ? feherhatterkeptexture : feketehatterkeptexture,0,&hely);
    hely = {180,felso ? 30 : 530,40,40};
    SDL_RenderCopy(renderer,felso ? jatekosok[0]->getTexture() : jatekosok[1]->getTexture(),0,&hely);
    szovegmegjelenit(seged::to_string(9 - (felso ? feherfiguraszam : feketefiguraszam)).c_str(),75,felso ? 19 : 519,felso ? FEHER_BETUSZIN : FEKETE_BETUSZIN,szamfont);
    SDL_RenderPresent(renderer);
}

void MalomPalya::keretleveves(bool felso)
{
    hely = {0,felso ? 0 : 500,400,100};
    SDL_RenderCopy(renderer,felso ? feherhatterkeptexture : feketehatterkeptexture,0,&hely);
    szovegmegjelenit("Levehetsz",95,jelenlegi== 0 ? 0 : 500 ,jelenlegi==0 ? FEHER_BETUSZIN : FEKETE_BETUSZIN,betufont);
    szovegmegjelenit("egy korongot!",60,jelenlegi== 0 ? 40 : 540 ,jelenlegi==0 ? FEHER_BETUSZIN : FEKETE_BETUSZIN,betufont);
    SDL_RenderPresent(renderer);
}

void MalomPalya::palyaujrarajzol()
{
    hely = {0,100,400,400};
    SDL_RenderCopy(renderer,malomkeptexture,0,&hely);
    for(int i = 0; i < mezokszama; i++)
    {
        if(mezok[i]->foglalt())
            kepmegjelenit(mezok[i]->getKoordinata(), mezok[i]->getFigura()->getTulaj()->getTexture());
            }
    SDL_RenderPresent(renderer);
}

void MalomPalya::mezokbeallit()
{
    /*
    mezők dinamikusan memóriaterületre történő létrehozás
    */
    mezok = new Mezo*[mezokszama];
    for(int i = 0; i < mezokszama ; i++)
        mezok[i] = new Mezo(i+1);

    /*
    mezők koordinátáinak beállítása
    */
    for(int i = 0; i < 3; i++)
        mezok[i]->setKoordinata({52 + 147*i, 151});
    for(int i = 0; i < 3; i++)
        mezok[i+3]->setKoordinata({101 + 98*i, 200});
    for(int i = 0; i < 3; i++)
        mezok[i+6]->setKoordinata({150 + 49*i, 249});
     for(int i = 0; i < 3; i++)
        mezok[i+9]->setKoordinata({52 + 49*i, 299});
    for(int i = 0; i < 3; i++)
        mezok[i+12]->setKoordinata({248 + 49*i ,299});
    for(int i = 0; i < 3; i++)
        mezok[i+15]->setKoordinata({150 + 49*i, 348});
    for(int i = 0; i < 3; i++)
        mezok[i+18]->setKoordinata({101 + 98*i, 397});
    for(int i = 0; i < 3; i++)
        mezok[i+21]->setKoordinata({52 + 147*i, 446});


    /*
    mezők kapcsolatainak beállítása, pointerekkel
    */
    (*mezok[0])[0] = mezok[1]; (*mezok[0])[1] = mezok[9];
    (*mezok[1])[0] = mezok[0]; (*mezok[1])[1] = mezok[2];  (*mezok[1])[2] = mezok[4];
    (*mezok[2])[0] = mezok[1];  (*mezok[2])[1] = mezok[14];
    (*mezok[3])[0] = mezok[4]; (*mezok[3])[1] = mezok[10];
    (*mezok[4])[0] = mezok[1]; (*mezok[4])[1] = mezok[3];  (*mezok[4])[2] = mezok[5]; (*mezok[4])[3] = mezok[7];
    (*mezok[5])[0] = mezok[4];  (*mezok[5])[1] = mezok[13];
    (*mezok[6])[0] = mezok[7];  (*mezok[6])[1] = mezok[11];
    (*mezok[7])[0] = mezok[4]; (*mezok[7])[1] = mezok[6];  (*mezok[7])[2] = mezok[8];
    (*mezok[8])[0] = mezok[7];  (*mezok[8])[1] = mezok[12];
    (*mezok[9])[0] = mezok[0]; (*mezok[9])[1] = mezok[10];  (*mezok[9])[2] = mezok[21];
    (*mezok[10])[0] = mezok[3]; (*mezok[10])[1] = mezok[9];  (*mezok[10])[2] = mezok[11]; (*mezok[10])[3] = mezok[18];
    (*mezok[11])[0] = mezok[6]; (*mezok[11])[1] = mezok[10];  (*mezok[11])[2] = mezok[15];
    (*mezok[12])[0] = mezok[8]; (*mezok[12])[1] = mezok[13];  (*mezok[12])[2] = mezok[17];
    (*mezok[13])[0] = mezok[5]; (*mezok[13])[1] = mezok[12];  (*mezok[13])[2] = mezok[14]; (*mezok[13])[3] = mezok[20];
    (*mezok[14])[0] = mezok[2]; (*mezok[14])[1] = mezok[13];  (*mezok[14])[2] = mezok[23];
    (*mezok[15])[0] = mezok[11]; (*mezok[15])[1] = mezok[16];
    (*mezok[16])[0] = mezok[15]; (*mezok[16])[1] = mezok[17];  (*mezok[16])[2] = mezok[19];
    (*mezok[17])[0] = mezok[12]; (*mezok[17])[1] = mezok[16];
    (*mezok[18])[0] = mezok[10]; (*mezok[18])[1] = mezok[19];
    (*mezok[19])[0] = mezok[16]; (*mezok[19])[1] = mezok[18];  (*mezok[19])[2] = mezok[20]; (*mezok[19])[3] = mezok[22];
    (*mezok[20])[0] = mezok[13]; (*mezok[20])[1] = mezok[19];
    (*mezok[21])[0] = mezok[9];  (*mezok[21])[1] = mezok[22];
    (*mezok[22])[0] = mezok[19]; (*mezok[22])[1] = mezok[21];  (*mezok[22])[2] = mezok[23];
    (*mezok[23])[0] = mezok[14]; (*mezok[23])[1] = mezok[22];


    /*
    lehetséges malmok mezőinek beállítása
    */
    malmok[0][0] = 1; malmok[0][1] = 2; malmok[0][2] = 3;
    malmok[1][0] = 4; malmok[1][1] = 5; malmok[1][2] = 6;
    malmok[2][0] = 7; malmok[2][1] = 8; malmok[2][2] = 9;
    malmok[3][0] = 10; malmok[3][1] = 11; malmok[3][2] = 12;
    malmok[4][0] = 13; malmok[4][1] = 14; malmok[4][2] = 15;
    malmok[5][0] = 16; malmok[5][1] = 17; malmok[5][2] = 18;
    malmok[6][0] = 19; malmok[6][1] = 20; malmok[6][2] = 21;
    malmok[7][0] = 22; malmok[7][1] = 23; malmok[7][2] = 24;
    malmok[8][0] = 1; malmok[8][1] = 10; malmok[8][2] = 22;
    malmok[9][0] = 4; malmok[9][1] = 11; malmok[9][2] = 19;
    malmok[10][0] = 7; malmok[10][1] = 12; malmok[10][2] = 16;
    malmok[11][0] = 2; malmok[11][1] = 5; malmok[11][2] = 8;
    malmok[12][0] = 17; malmok[12][1] = 20; malmok[12][2] = 23;
    malmok[13][0] = 9; malmok[13][1] = 13; malmok[13][2] = 18;
    malmok[14][0] = 6; malmok[14][1] = 14; malmok[14][2] = 21;
    malmok[15][0] = 3; malmok[15][1] = 15; malmok[15][2] = 24;


}

void MalomPalya::vegeteszt()
{
    int ervenyes1 = 0, ervenyes2 = 0;
    for(int i = 0; i < 9 ; i++)
    {
        if((*jatekosok[0])[i] != NULL) ervenyes1++; //1.játékos bábui
        if((*jatekosok[1])[i] != NULL) ervenyes2++; //2.játékos bábui
    }
    if(ervenyes1 < 3 || ervenyes2 < 3)
    {
        vegemutat(ervenyes2 < 3);
        vege = true;
    }
}

bool MalomPalya::barhova(int jatekos)
{
    int szamlalo = 0;
    for(int i = 0; i < 9; i++)
        if((*jatekosok[jatekos])[i] != NULL) szamlalo++;
    return szamlalo <= 3;
}

void MalomPalya::malom(Mezo* mezo)
{
    bool leveheto = false; //van-e levehető bábúja az ellenfélnek
    for(int i = 0; (i < (jelenlegi==0 ? feketefiguraszam : feherfiguraszam)) && !leveheto; i++)
                if((*jatekosok[jelenlegi==0 ? 1 : 0])[i] != NULL && (*jatekosok[jelenlegi==0 ? 1 : 0])[i]-> getMezo() != NULL
                  && malomvizsgal((*jatekosok[jelenlegi==0 ? 1 : 0])[i]->getMezo()) < 0) leveheto = true;
    if(malomvizsgal(mezo) >= 0 && leveheto)
    {
        keretleveves(jelenlegi==0);
        bool ervenyes = false;
        while(!ervenyes)
        {
            int torlendo;
            kattintasmezon(torlendo);
            if(mezok[torlendo - 1]->foglalt() &&  malomvizsgal(mezok[torlendo - 1]) < 0 && mezok[torlendo - 1]->getFigura()->getTulaj() == jatekosok[jelenlegi==0 ? 1 : 0])
            {
                ervenyes = true;
                for(int i = 0; i < (jelenlegi==0 ? feketefiguraszam : feherfiguraszam); i++)
                    if((*jatekosok[jelenlegi==0 ? 1 : 0])[i] != NULL &&
                       (*jatekosok[jelenlegi==0 ? 1 : 0])[i]->getMezo()->getAzonosito() == torlendo)
                       (*jatekosok[jelenlegi==0 ? 1 : 0])[i] = NULL;
                delete mezok[torlendo - 1]->getFigura();
                mezok[torlendo - 1]->setFigura(NULL);
                palyaujrarajzol();
            }
        }
    }
}

int MalomPalya::malomvizsgal(const Mezo * uj)
{
    int jatekos = (uj->getFigura()->getTulaj() == jatekosok[0]) ? 0 : 1;
    int azonosito = uj->getAzonosito();
    for(int i = 0; i < 16; i++)
        if(malmok[i][0] == azonosito || malmok[i][1] == azonosito || malmok[i][2] == azonosito)
        {
            int egyezo = 0;
            for(int j=0; j < 9; j++)
            {
                if((*jatekosok[jatekos])[j] != NULL && (*jatekosok[jatekos])[j]->getMezo() != NULL)
                {
                    int azonosito2 = (*jatekosok[jatekos])[j]->getMezo()->getAzonosito();
                    if(azonosito2 == malmok[i][0] || azonosito2 == malmok[i][1] || azonosito2 == malmok[i][2]) egyezo++;
                }
            }
            if(egyezo == 3) return i; //az i. elem tartalmazza a malomban részt vevő mezők azonosítóját
        }
    return -1; //nincs malom
}

void MalomPalya::elsoszakaszlep()
{
    bool ervenyes = false;
    int mezo;
    while(!ervenyes)
    {
        kattintasmezon(mezo);
        if(!mezok[mezo - 1]->foglalt() )
        {
            kepmegjelenit(mezok[mezo - 1]->getKoordinata(),jatekosok[jelenlegi]->getTexture());
            ervenyes = true;
            mezok[mezo - 1]->setFigura((*jatekosok[jelenlegi])[jelenlegi==0 ? feherfiguraszam : feketefiguraszam]);
            (*jatekosok[jelenlegi])[jelenlegi==0 ? feherfiguraszam : feketefiguraszam]->setMezo(mezok[mezo - 1]);
            jelenlegi==0 ? feherfiguraszam++ : feketefiguraszam++;
            if(feketefiguraszam == 9) elsoszakasz = false;
            malom(mezok[mezo - 1]);
            keretujrarajzol(jelenlegi==0);
        }

    }
}

void MalomPalya::masodikszakaszlep()
{
    bool ervenyes = false;
    int mezo1, mezo2;
    while(!ervenyes)
    {
        palyaujrarajzol();
        Figura * mozoghat[9];
        int mozoghatdb = 0;
        bool barhovabool = barhova(jelenlegi);
        if(!barhovabool)
        {
            for(int i = 0; i < 9; i++)
                if( (*jatekosok[jelenlegi])[i] != NULL && ( !((*(*jatekosok[jelenlegi])[i]->getMezo())[0]->foglalt()) ||
                !((*(*jatekosok[jelenlegi])[i]->getMezo())[1]->foglalt()) ||
                ( (*(*jatekosok[jelenlegi])[i]->getMezo())[2] != NULL && !((*(*jatekosok[jelenlegi])[i]->getMezo())[2]->foglalt()) ) ||
                ( (*(*jatekosok[jelenlegi])[i]->getMezo())[3] != NULL && !((*(*jatekosok[jelenlegi])[i]->getMezo())[3]->foglalt()) ) ) )
                    mozoghat[mozoghatdb++] = (*jatekosok[jelenlegi])[i];
            if(mozoghatdb == 0)
            {
                vegemutat(jelenlegi==1);
                vege = true;
                return;
            }
        }
        else
        {
            for(int i = 0; i < 9; i++)
                if( (*jatekosok[jelenlegi])[i] != NULL )
                    mozoghat[mozoghatdb++] = (*jatekosok[jelenlegi])[i];
        }

        kattintasmezon(mezo1);
        if(mezok[mezo1 - 1]->getFigura() != NULL && find(mozoghat, mozoghat + 9, mezok[mezo1 - 1]->getFigura()) != mozoghat + 9)
        {

            while(!ervenyes)
            {
                mezokijelol(mezo1);
                Figura * mozgo = mezok[mezo1 - 1]->getFigura();
                kattintasmezon(mezo2);
                if(!mezok[mezo2 - 1]->foglalt() &&
                    (barhovabool || (*mezok[mezo2 - 1])[0] == mezok[mezo1 - 1] || (*mezok[mezo2 - 1])[1] == mezok[mezo1 - 1] ||
                     (*mezok[mezo2 - 1])[2] == mezok[mezo1 - 1] || (*mezok[mezo2 - 1])[3] == mezok[mezo1 - 1]) )
                {
                    ervenyes = true;
                    mezok[mezo2 - 1]->setFigura(mozgo);
                    mozgo->setMezo(mezok[mezo2 - 1]);
                    mezok[mezo1 - 1]->setFigura(NULL);
                }
                else if(mezok[mezo2 - 1]->getFigura() != NULL && find(mozoghat, mozoghat + 9, mezok[mezo2 - 1]->getFigura()) != mozoghat + 9)
                {
                    mezo1 = mezo2;
                    palyaujrarajzol();
                }
            }
        }
    }
    palyaujrarajzol();
    malom(mezok[mezo2 - 1]);
    keretujrarajzol(jelenlegi==0);
}

void MalomPalya::kepmegjelenit(SDL_Point pont,SDL_Texture* kep)
{
    hely = {pont.x - KEP_MERET/2,pont.y - KEP_MERET/2,KEP_MERET,KEP_MERET};
    SDL_RenderCopy(renderer,kep,0,&hely);
}

MalomPalya::MalomPalya()
{
    jatekosokszama = 2;
    mezokszama = 24;
    jelenlegi = 1;
    elsoszakasz = true;
    vege = false;
    feherfiguraszam = 0;
    feketefiguraszam = 0;
    mezokbeallit();
    grafikabetolt();
    jatekosok[0] = new Jatekos("Világos",9,0, "feher.png",renderer);
    jatekosok[1] = new Jatekos("Sötét",9,0, "fekete.png", renderer);
    palyaujrarajzol();
    keretujrarajzol(true);
    keretujrarajzol(false);
}

void MalomPalya::jatek()
{
    while(!vege)
    {
        jelenlegi = jelenlegi == 0 ? 1 : 0;
        keretujrarajzol(jelenlegi==0);
        if(jelenlegi == 0)
        {
            szovegmegjelenit("Fehér",250,2,FEHER_BETUSZIN,betufont);
            szovegmegjelenit("lép",280,42,FEHER_BETUSZIN,betufont);
        }
        else
        {
            szovegmegjelenit("Fekete",245,502,FEKETE_BETUSZIN,betufont);
            szovegmegjelenit("lép",280,542,FEKETE_BETUSZIN,betufont);
        }
        elsoszakasz ? elsoszakaszlep() : masodikszakaszlep();
        vegeteszt();
    }
}

void MalomPalya::grafikabetolt()
{
    if( !( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) )
    {
        std::cerr << "SDL_image hiba !!! " << IMG_GetError();
        return;
    }
    if(TTF_Init() == -1 )
    {
        std::cerr << "SDL_TTF hiba !!! " << TTF_GetError();
        return;
    }
    betufont = TTF_OpenFont("Garamond Classico SC.ttf", 160);
    if (!betufont)  std::cerr << "Betű hiba !!! " << SDL_GetError() << std::endl;
    szamfont = TTF_OpenFont("times.ttf", 160);
    if (!szamfont)  std::cerr << "Betű hiba !!! " << SDL_GetError() << std::endl;

    malomkep = IMG_Load("malom_palya.png");
    if(malomkep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }

    feherhatterkep = IMG_Load("feherhatter.jpg");
    if(feherhatterkep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    feketehatterkep = IMG_Load("feketehatter.jpg");
    if(feketehatterkep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    vegehatterkep = IMG_Load("vegehatter3.png");
    if(vegehatterkep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    vegehatterkupakep = IMG_Load("vegehatterkupa.png");
    if(vegehatterkupakep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    gombkep = IMG_Load("gomb3.png");
    if(gombkep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }

    window = SDL_CreateWindow( "Malom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ABLAK_SZELESSEG, ABLAK_MAGASSAG, SDL_WINDOW_SHOWN );
    if( window == NULL )
    {
        std::cerr <<  "Ablak hiba !!! " <<  SDL_GetError() << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    malomkeptexture = SDL_CreateTextureFromSurface(renderer, malomkep);
    feherhatterkeptexture = SDL_CreateTextureFromSurface(renderer, feherhatterkep);
    feketehatterkeptexture = SDL_CreateTextureFromSurface(renderer, feketehatterkep);
    vegehatterkeptexture = SDL_CreateTextureFromSurface(renderer, vegehatterkep);
    vegehatterkupakeptexture = SDL_CreateTextureFromSurface(renderer, vegehatterkupakep);
    gombkeptexture = SDL_CreateTextureFromSurface(renderer, gombkep);
}

void MalomPalya::vegemutat(bool feher)
{
    hely = {0,0,ABLAK_SZELESSEG,ABLAK_MAGASSAG};
    SDL_RenderCopy(renderer,vegehatterkeptexture,0,&hely); //a vége képernyő hátterének betöltése
    hely = {150,60,100,100};
    SDL_RenderCopy(renderer,vegehatterkupakeptexture,0,&hely);  //a vége képernyőn található kupa betöltése
    szovegmegjelenit(feher ? "fehér" : "fekete",feher ? 150 : 140,190,FEKETE_BETUSZIN,betufont);
    szovegmegjelenit("játékos",130,265,FEKETE_BETUSZIN,betufont);
    szovegmegjelenit("nyert!",140,340,FEKETE_BETUSZIN,betufont);
    hely = {(ABLAK_SZELESSEG-VEGE_GOMB_SZELESSEG)/2,VEGE_GOMB_Y,VEGE_GOMB_SZELESSEG,VEGE_GOMB_MAGASSAG};
    SDL_RenderCopy(renderer,gombkeptexture,0,&hely);
    szovegmegjelenit("ok",174,437,FEHER_BETUSZIN,betufont);
    SDL_RenderPresent(renderer);
    SDL_Event event;
    while(SDL_WaitEvent(&event) && event.type != SDL_QUIT)
    {
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN: //egér kattintás
            if(event.button.button == SDL_BUTTON_LEFT) //bal klikk
            {
                int x = event.button.x - (ABLAK_SZELESSEG-VEGE_GOMB_SZELESSEG)/2;
                int y = event.button.y - VEGE_GOMB_Y;
                if(x > 0 && x < VEGE_GOMB_SZELESSEG && y > 0 && y < VEGE_GOMB_MAGASSAG
                   && !(x+y<7 || (VEGE_GOMB_SZELESSEG - x) + y < 7 || x + (VEGE_GOMB_MAGASSAG - y) < 7 ||  (VEGE_GOMB_SZELESSEG - x) + (VEGE_GOMB_MAGASSAG - y) < 7    //sarkok
                        || ((x == 1  || x==100) && (y == 7 || y == 8 || y == 94 || y == 93))
                        || ((x == 7  || x==94) && (y == 1 || y == 50))  ) ) return;
            }
            break;
        }
    }
}

MalomPalya::~MalomPalya()
{
    TTF_CloseFont(betufont);
    TTF_CloseFont(szamfont);
    SDL_FreeSurface(malomkep);
    SDL_FreeSurface(feherhatterkep);
    SDL_FreeSurface(feketehatterkep);
    SDL_FreeSurface(vegehatterkep);
    SDL_FreeSurface(vegehatterkupakep);
    SDL_FreeSurface(gombkep);
    SDL_FreeSurface(szoveg);
    SDL_DestroyTexture(malomkeptexture);
    SDL_DestroyTexture(feherhatterkeptexture);
    SDL_DestroyTexture(feketehatterkeptexture);
    SDL_DestroyTexture(vegehatterkeptexture);
    SDL_DestroyTexture(vegehatterkupakeptexture);
    SDL_DestroyTexture(gombkeptexture);
    SDL_DestroyTexture(szovegtexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow( window );
    TTF_Quit();
    IMG_Quit();
}
