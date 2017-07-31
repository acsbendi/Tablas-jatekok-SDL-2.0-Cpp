#include "kigyokesletrakpalya.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sstream>
#include "seged.h"

int KigyokEsLetrakPalya::kattintas(int& x, int& y)
{
    SDL_PumpEvents();
    SDL_FlushEvents(0,SDL_LASTEVENT);
    SDL_Event event;
    while(SDL_WaitEvent(&event) && event.type != SDL_QUIT)
    {
        switch(event.type)
        {
        case SDL_MOUSEBUTTONDOWN: //egér kattintás
            if(event.button.button == SDL_BUTTON_LEFT) //bal klikk
            {
                x = event.button.x;
                y = event.button.y;
                return 1;
            }
            break;
        }
    }
    return 0;
}

void KigyokEsLetrakPalya::dobas(int szam)
{
    int x,y;
    //várunk egy kattintást a dobás gombra
    while(kattintas(x,y))
    {
        if( (DOBO_GOMB_X + (double)DOBO_GOMB_ATMERO/2 - x)*(DOBO_GOMB_X + (double)DOBO_GOMB_ATMERO/2 - x) +
                (DOBO_GOMB_Y + (double)DOBO_GOMB_ATMERO/2 - y)*(DOBO_GOMB_Y + (double)DOBO_GOMB_ATMERO/2 - y)  < ((double)DOBO_GOMB_ATMERO/2)*((double)DOBO_GOMB_ATMERO/2) )
        {
            dobasanimacio(szam);
            SDL_Delay(300);
            doborajzol();
            break; //megtörténhet a dobás
        }
    }
}

void KigyokEsLetrakPalya::figuramezon(const Mezo* mezo, int jatekos)
{
    int xkezdo = mezo->getKoordinata().x;
    int ykezdo = mezo->getKoordinata().y;
    hely = {xkezdo + 1 + (jatekos%3)*21, ykezdo  + 1 + (jatekos/3)*33,BABU_SZELESSEG,BABU_MAGASSAG };
    SDL_RenderCopy(renderer,jatekosok[jatekos]->getTexture(),0,&hely);
}

void KigyokEsLetrakPalya::urespalyarajzol()
{
    /*
    az egyes mezők hátterének megjelenítése
    */
    for(int i = 0; i < 10 ; i++)
    {
        for(int  j = 0; j < 10; j++)
        {
            int szam = 91+j-i*10;
            hely = {mezok[szam-1]->getKoordinata().x,mezok[szam-1]->getKoordinata().y,MEZO_MERET,MEZO_MERET};
            switch((i+j) % 4)
            {
            case 0:
                SDL_RenderCopy(renderer,vilagosmezotexture,0,&hely);
                break;
            case 1:
                SDL_RenderCopy(renderer,sotetmezotexture,0,&hely);
                break;
            case 2:
                SDL_RenderCopy(renderer,vilagosmezo2texture,0,&hely);
                break;
            case 3:
                SDL_RenderCopy(renderer,sotetmezo2texture,0,&hely);
                break;
            }

        }
    }

    /*
    a pálya egyéb elemeinek betöltése - keret, kígyók, létrák
    */
    hely = {0,0,PALYA_SZELESSEG,PALYA_MAGASSAG};
    SDL_RenderCopy(renderer,palyakeptexture,0,&hely);

    /*
    minden 4. mezőről hiányzik a számozás - ennek pótlása
    új ciklus kell, mivel ez kerül legfelülre
    */
    for(int i = 0; i < 10 ; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            int szam = 91+j-i*10;
            hely = {mezok[szam-1]->getKoordinata().x,mezok[szam-1]->getKoordinata().y,MEZO_MERET,MEZO_MERET};
            switch((i+j) % 4)
            {
            case 3:
                int w,h;
                TTF_SizeUTF8(szamfont, seged::to_string(szam).c_str(), &w, &h);
                SDL_FreeSurface(szoveg);
                szoveg = TTF_RenderUTF8_Blended(szamfont,seged::to_string(szam).c_str(), SZOVEGSZIN);
                SDL_DestroyTexture(szovegtexture);
                szovegtexture =  SDL_CreateTextureFromSurface(renderer, szoveg);
                hely = {hely.x+ (szam > 9 ? 30 : 47 ),hely.y-4,w/5,h/5};
                SDL_RenderCopy(renderer,szovegtexture,0,&hely);
                break;
            }
        }
    }
}

void KigyokEsLetrakPalya::palyarajzol()
{
    urespalyarajzol();
    for(int i = 0; i < jatekosokszama; i++)
        figuramezon((*jatekosok[i])[0]->getMezo(),i);
    SDL_RenderPresent(renderer);
}

void KigyokEsLetrakPalya::lep()
{
    int dobott = rand() % 6 + 1;    //kisorsoljuk a dobott számot
    dobas(dobott);     //a dobás grafikai része

    Mezo * lepett;
    int jelenlegiazonosito = (*jatekosok[jelenlegi])[0]->getMezo()->getAzonosito();
    if(dobott +  jelenlegiazonosito > 100) lepett = mezok[99 - ( dobott - (100 - jelenlegiazonosito))];     //túlment az utolsó mezőn
    else lepett = mezok[jelenlegiazonosito - 1 + dobott];   //nem ment túl
    if(lepett->getAzonosito() == mezokszama  || (*lepett)[0] == mezok[lepett->getAzonosito()])  //utolsóra vagy normál mezőre lépett
    {
        (*jatekosok[jelenlegi])[0]->setMezo(lepett);
    }
    else //különleges mezőre lépett
    {
        (*jatekosok[jelenlegi])[0]->setMezo((*lepett)[0]);
    }
    palyarajzol();
}

void KigyokEsLetrakPalya::szovegmegjelenit(const char* s, int x, int y, SDL_Color szin, TTF_Font * font)
{
    int w,h;
    TTF_SizeUTF8(font, s, &w, &h);
    SDL_FreeSurface(szoveg);
    szoveg = TTF_RenderUTF8_Blended(font,s,szin);
    SDL_DestroyTexture(szovegtexture);
    szovegtexture =  SDL_CreateTextureFromSurface(renderer, szoveg);
    hely = {x,y,w/3,h/3};
    SDL_RenderCopy(renderer,szovegtexture,0,&hely);
}

void KigyokEsLetrakPalya::doborajzol()
{
    hely = {PALYA_SZELESSEG,0,ABLAK_SZELESSEG-PALYA_SZELESSEG,ABLAK_MAGASSAG};
    SDL_RenderCopy(renderer,hattertexture,0,&hely);     //háttér megjelenítése
    hely = {DOBO_GOMB_X,DOBO_GOMB_Y,DOBO_GOMB_ATMERO,DOBO_GOMB_ATMERO};
    SDL_RenderCopy(renderer,dobasgombtexture,0,&hely);  //dobógomb megjelenítése
    hely = {PALYA_SZELESSEG + (ABLAK_SZELESSEG-PALYA_SZELESSEG-DOBASKEP_SZELESSEG)/2,DOBASKEP_Y,DOBASKEP_SZELESSEG,DOBASKEP_MAGASSAG};
    SDL_RenderCopy(renderer,zoldhattertexture,0,&hely);
    hely =  {PALYA_SZELESSEG + (ABLAK_SZELESSEG-PALYA_SZELESSEG-DOBOKEP_SZELESSEG)/2,0,DOBOKEP_SZELESSEG,DOBOKEP_MAGASSAG};
    SDL_RenderCopyEx(renderer,dobokeptexture,0,&hely,0,0,SDL_FLIP_VERTICAL);
    SDL_RenderPresent(renderer);
}

void KigyokEsLetrakPalya::valasztorajzol(int temp)
{
    hely = {PALYA_SZELESSEG,0,ABLAK_SZELESSEG-PALYA_SZELESSEG,ABLAK_MAGASSAG};
    SDL_RenderCopy(renderer,hattertexture,0,&hely);     //háttérkép betöltése
    szovegmegjelenit("Hány", 755, 50, SZOVEGSZIN, betufont);
    szovegmegjelenit("játékos", 735, 100, SZOVEGSZIN, betufont);
    szovegmegjelenit("játsszon?", 715, 150, SZOVEGSZIN, betufont);   //szöveg ("Hány játékos játsszon") megjelenítése
    hely = {NOVELO_X,NOVELO_Y,NOVELO_SZELESSEG,NOVELO_MAGASSAG};
    SDL_RenderCopy(renderer,novelotexture,0,&hely);     //növelő gomb megjelenítése
    szovegmegjelenit(seged::to_string(temp).c_str(), 787, 310, SZOVEGSZIN, betufont);   //jelenleg kiválasztott játékosszám megjelenítése
    hely = {NOVELO_X,CSOKKENTO_Y,NOVELO_SZELESSEG,NOVELO_MAGASSAG};
    SDL_RenderCopyEx(renderer,novelotexture,0,&hely,0,0,SDL_FLIP_VERTICAL); //csökkentő gomb megjelenítése, tükrözéssel
    hely = {GOMB_X,GOMB_Y,GOMB_SZELESSEG,GOMB_MAGASSAG};
    SDL_RenderCopy(renderer,gombtexture,0,&hely);       //indítógomb megjelenítése
    SDL_RenderPresent(renderer);
}

int KigyokEsLetrakPalya::jatekosokszamakivalaszt()
{
    int temp = 2, x,y;
    valasztorajzol(temp);
    while(kattintas(x,y))
    {
        //játékosszám kiválasztva - start gomb
        if(x > GOMB_X && x < GOMB_X + GOMB_SZELESSEG && y > GOMB_Y && y < GOMB_Y + GOMB_MAGASSAG
                && !(x - GOMB_X + y - GOMB_Y < 6 ||  std::abs(x - GOMB_X - GOMB_SZELESSEG) + y - GOMB_Y < 6
                     || x - GOMB_X + std::abs(y - GOMB_Y - GOMB_MAGASSAG) < 6  || std::abs(x - GOMB_X - GOMB_SZELESSEG) + std::abs(y - GOMB_Y - GOMB_MAGASSAG) < 6) )
            break;
        //növelő gomb
        if( x > NOVELO_X && x < NOVELO_X + NOVELO_SZELESSEG && y > NOVELO_Y && y < NOVELO_Y + NOVELO_MAGASSAG &&
                ((x - NOVELO_X < (double)NOVELO_SZELESSEG/2 && y - NOVELO_Y > NOVELO_MAGASSAG - (double)NOVELO_MAGASSAG/NOVELO_SZELESSEG*(x-NOVELO_X)*2 )
                 || ( x  - NOVELO_X > (double)NOVELO_SZELESSEG/2 && y - NOVELO_Y > (double)NOVELO_MAGASSAG/NOVELO_SZELESSEG*(x-NOVELO_X)*2 - NOVELO_MAGASSAG) ) && temp < 6)
            valasztorajzol(++temp);
        //csökkentő gomb
        if( x > NOVELO_X && x < NOVELO_X + NOVELO_SZELESSEG && y > CSOKKENTO_Y && y < CSOKKENTO_Y + NOVELO_MAGASSAG &&
                ((x - NOVELO_X < (double)NOVELO_SZELESSEG/2 && y - CSOKKENTO_Y < (double)NOVELO_MAGASSAG/NOVELO_SZELESSEG*(x-NOVELO_X)*2 )
                 || ( x  - NOVELO_X > (double)NOVELO_SZELESSEG/2 && y - CSOKKENTO_Y < NOVELO_MAGASSAG - (double)NOVELO_MAGASSAG/NOVELO_SZELESSEG*(x-NOVELO_X-(double)NOVELO_SZELESSEG/2)*2) ) && temp > 1)
            valasztorajzol(--temp);
    }
    return temp;
}

void KigyokEsLetrakPalya::grafikabetolt()
{
    ablak = SDL_CreateWindow("Kígyók és létrák",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ABLAK_SZELESSEG, ABLAK_MAGASSAG, SDL_WINDOW_SHOWN );
    if( ablak == NULL )
    {
        std::cerr <<  "Ablak hiba !!! " <<  SDL_GetError() << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(ablak, -1, SDL_RENDERER_ACCELERATED);

    if(TTF_Init() == -1 )
    {
        std::cerr << "SDL_TTF hiba !!! " << TTF_GetError();
        return;
    }
    szamfont = TTF_OpenFont("segoe-print.ttf",115);
    if (!szamfont)  std::cerr << "Betű hiba !!! " << SDL_GetError() << std::endl;
    betufont = TTF_OpenFont("segoe-print-bold.ttf",115);
    if (!betufont)  std::cerr << "Betű hiba !!! " << SDL_GetError() << std::endl;


    if( !( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) )
    {
        std::cerr << "SDL_image hiba !!! " << IMG_GetError();
        return;
    }

    palyakep = IMG_Load("kigyokesletrak.png");
    if(palyakep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    palyakeptexture = SDL_CreateTextureFromSurface(renderer,palyakep);

    vilagosmezo = IMG_Load("vilagosmezo.png");
    if(vilagosmezo == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    vilagosmezotexture = SDL_CreateTextureFromSurface(renderer,vilagosmezo);

    vilagosmezo2 = IMG_Load("vilagosmezo2.png");
    if(vilagosmezo2 == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    vilagosmezo2texture = SDL_CreateTextureFromSurface(renderer,vilagosmezo2);

    sotetmezo = IMG_Load("sotetmezo.png");
    if(sotetmezo == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    sotetmezotexture = SDL_CreateTextureFromSurface(renderer,sotetmezo);

    sotetmezo2 = IMG_Load("sotetmezo2.png");
    if(sotetmezo2 == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    sotetmezo2texture = SDL_CreateTextureFromSurface(renderer,sotetmezo2);

    hatter = IMG_Load("klhatter.png");
    if(hatter == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    hattertexture = SDL_CreateTextureFromSurface(renderer,hatter);

    novelo = IMG_Load("novelo.png");
    if(novelo == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    novelotexture = SDL_CreateTextureFromSurface(renderer,novelo);

    gomb = IMG_Load("klgomb.png");
    if(gomb == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    gombtexture = SDL_CreateTextureFromSurface(renderer,gomb);

    dobasgomb = IMG_Load("dobasgomb.png");
    if(dobasgomb == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    dobasgombtexture = SDL_CreateTextureFromSurface(renderer,dobasgomb);

    vegehatterkep = IMG_Load("vegehatterkl.png");
    if(vegehatterkep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    vegehatterkeptexture = SDL_CreateTextureFromSurface(renderer, vegehatterkep);

    vegegomb = IMG_Load("vegegomb.png");
    if(vegegomb == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    vegegombtexture = SDL_CreateTextureFromSurface(renderer, vegegomb);

    koszoru = IMG_Load("koszoru2.png");
    if(koszoru == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    koszorutexture = SDL_CreateTextureFromSurface(renderer, koszoru);

    dobokep = IMG_Load("dice_shaker.png");
    if(dobokep == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    dobokeptexture = SDL_CreateTextureFromSurface(renderer, dobokep);

    zoldhatter = IMG_Load("zoldhatter.png");
    if(zoldhatter == NULL)
    {
        std::cerr << "Képhiba !!! " << SDL_GetError();
        return;
    }
    zoldhattertexture = SDL_CreateTextureFromSurface(renderer, zoldhatter);

    //mezők koordinátáinak beállítása
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            mezok[90+j-i*10]->setKoordinata({4+70*j,4+70*i});
}

void KigyokEsLetrakPalya::vegemutat(int nyertes)
{
    hely = {0,0,ABLAK_SZELESSEG,ABLAK_MAGASSAG};
    SDL_RenderCopy(renderer,vegehatterkeptexture,0,&hely); //a vége képernyő hátterének betöltése
    hely = {ABLAK_SZELESSEG/2-KOSZORU_SZELESSEG/2,107,KOSZORU_SZELESSEG,KOSZORU_MAGASSAG};
    SDL_RenderCopy(renderer,koszorutexture,0,&hely);        //a koszorú betöltése a bábú köré
    hely = {ABLAK_SZELESSEG/2-BABU_VEGE_SZELESSEG/2,125,BABU_VEGE_SZELESSEG,BABU_VEGE_MAGASSAG};
    SDL_RenderCopy(renderer,jatekosok[nyertes]->getTexture(),0,&hely);  //a vége képernyőn található bábu betöltése
    szovegmegjelenit(jatekosok[nyertes]->getNev().c_str(),397,260,SZOVEGSZIN,betufont);
    szovegmegjelenit("játékos",385,330,SZOVEGSZIN,betufont);
    szovegmegjelenit("nyert!",397,400,SZOVEGSZIN,betufont);
    hely = {ABLAK_SZELESSEG/2-VEGE_GOMB_SZELESSEG/2,VEGE_GOMB_Y,VEGE_GOMB_SZELESSEG,VEGE_GOMB_MAGASSAG};
    SDL_RenderCopy(renderer,vegegombtexture,0,&hely);
    SDL_RenderPresent(renderer);


    int x,y;
    //várunk egy kattintást az OK gombra
    while(kattintas(x,y))
    {
        x = x - ABLAK_SZELESSEG/2 + VEGE_GOMB_SZELESSEG/2;  //gombon belüli x koordináta
        y = y - VEGE_GOMB_Y;        //gombon belüli y koordináta
        if(x > 0 && x < VEGE_GOMB_SZELESSEG && y > 0 && y < VEGE_GOMB_MAGASSAG
                && !(x+y<7 || (VEGE_GOMB_SZELESSEG - x) + y < 7 || x + (VEGE_GOMB_MAGASSAG - y) < 7 ||  (VEGE_GOMB_SZELESSEG - x) + (VEGE_GOMB_MAGASSAG - y) < 7 ) )//sarkok
            break; //megtörtént a kattintás
    }
}

void KigyokEsLetrakPalya::mezokbeallit(int meret, const char* kapcsolatok)
{
    mezokszama = meret;
    mezok = new Mezo*[mezokszama];
    for(int i = 0; i < mezokszama; i++)
    {
        mezok[i] = new Mezo(i+1);
    }
    for(int i = 0; i < mezokszama-1; i++)
    {
        (*mezok[i])[0] = mezok[i+1];        //alapesetben mindegyik után a következő sorszámú jön
    }
    (*mezok[mezokszama-1])[0] = NULL;
    std::stringstream ss;
    int a, b;
    ss << kapcsolatok;
    while(ss >> a)
    {
        ss.ignore(1);
        ss >> b;
        ss.ignore(1);
        (*mezok[a-1])[0] = mezok[b-1];
    }
}

KigyokEsLetrakPalya::KigyokEsLetrakPalya(int meret, const char* kapcsolatok)
{
    mezokbeallit(meret, kapcsolatok);   //mezők beállítása
    srand(time(0));     //random szám generálás inicializálása
    grafikabetolt();    //grafikus felület betöltése
    urespalyarajzol();  //üres pálya megjelenítése
    SDL_RenderPresent(renderer);

    jatekosokszama = jatekosokszamakivalaszt();
    switch(jatekosokszama)
    {
    case 6:
        jatekosok[5] = new Jatekos("Zöld",1,mezok[0],"zoldbabu_n.png", renderer);
    case 5:
        jatekosok[4] = new Jatekos("Piros",1,mezok[0],"pirosbabu_n.png", renderer);
    case 4:
        jatekosok[3] = new Jatekos("Rózsaszín",1,mezok[0],"rozsaszinbabu_n.png", renderer);
    case 3:
        jatekosok[2] = new Jatekos("Kék",1,mezok[0],"kekbabu_n.png", renderer);
    case 2:
        jatekosok[1] = new Jatekos("Fehér",1,mezok[0],"feherbabu_n.png", renderer);
    case 1:
        jatekosok[0] = new Jatekos("Fekete",1,mezok[0],"feketebabu_n.png", renderer);
        break;
    default:
        break;
    }
    jelenlegi = jatekosokszama - 1; //lépés előtt változik a jelenlegi játékos => itt az utolsó kell
    doborajzol();   //dobás felület rajzolása
    palyarajzol();  //pálya rajzolása, most már bábukkal
}

void KigyokEsLetrakPalya::jatek()
{
    while((*jatekosok[jelenlegi])[0]->getMezo()->getAzonosito() != mezokszama )
    {
        jelenlegi = jelenlegi == jatekosokszama - 1 ? 0 : jelenlegi + 1;
        lep();
    }
    vegemutat(jelenlegi); //vége a játéknak, a jelenlegi játékos elérte az utolsó mezőt
}

void KigyokEsLetrakPalya::dobasanimacio(int dobas)
{
    std::stringstream nev;
    for(int i = 3; i < 17; i++)
    {
        nev << "animacio/" << i-2 << ".png";
        dobaskep = IMG_Load(nev.str().c_str());
        nev.str("");
        if(dobaskep == NULL)
        {
            std::cout << "Képhiba !!! " << SDL_GetError();
            return;
        }
        dobaskeptexture = SDL_CreateTextureFromSurface(renderer,dobaskep);
        SDL_Delay(ANIMACIO_KESLELTETES);
        hely = {PALYA_SZELESSEG + (ABLAK_SZELESSEG-PALYA_SZELESSEG-DOBASKEP_SZELESSEG)/2,DOBASKEP_Y,DOBASKEP_SZELESSEG,DOBASKEP_MAGASSAG};
        SDL_RenderCopy(renderer,dobaskeptexture,0,&hely);
        hely =  {PALYA_SZELESSEG + (ABLAK_SZELESSEG-PALYA_SZELESSEG-DOBOKEP_SZELESSEG)/2,0,DOBOKEP_SZELESSEG,DOBOKEP_MAGASSAG};
        SDL_RenderCopyEx(renderer,dobokeptexture,0,&hely,0,0,SDL_FLIP_VERTICAL);
        SDL_RenderPresent(renderer);
        SDL_FreeSurface(dobaskep);
        SDL_DestroyTexture(dobaskeptexture);
    }
    for(int i = 0; i < 3; i++)
    {
        nev << "animacio/" << i+15 << "-" << dobas << ".png";
        dobaskep = IMG_Load(nev.str().c_str());
        nev.str("");
        if(dobaskep == NULL)
        {
            std::cout << "Képhiba !!! " << SDL_GetError();
            return;
        }
        dobaskeptexture = SDL_CreateTextureFromSurface(renderer,dobaskep);
        SDL_Delay(ANIMACIO_KESLELTETES);
        hely = {PALYA_SZELESSEG + (ABLAK_SZELESSEG-PALYA_SZELESSEG-DOBASKEP_SZELESSEG)/2,DOBASKEP_Y,DOBASKEP_SZELESSEG,DOBASKEP_MAGASSAG};
        SDL_RenderCopy(renderer,dobaskeptexture,0,&hely);
        hely =  {PALYA_SZELESSEG + (ABLAK_SZELESSEG-PALYA_SZELESSEG-DOBOKEP_SZELESSEG)/2,0,DOBOKEP_SZELESSEG,DOBOKEP_MAGASSAG};
        SDL_RenderCopyEx(renderer,dobokeptexture,0,&hely,0,0,SDL_FLIP_VERTICAL);
        SDL_RenderPresent(renderer);
        SDL_FreeSurface(dobaskep);
        SDL_DestroyTexture(dobaskeptexture);
    }
}

KigyokEsLetrakPalya::~KigyokEsLetrakPalya()
{
    SDL_DestroyWindow(ablak);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(palyakep);
    SDL_DestroyTexture(palyakeptexture);
    SDL_FreeSurface(vilagosmezo);
    SDL_DestroyTexture(vilagosmezotexture);
    SDL_FreeSurface(vilagosmezo2);
    SDL_DestroyTexture(vilagosmezo2texture);
    SDL_FreeSurface(sotetmezo);
    SDL_DestroyTexture(sotetmezotexture);
    SDL_FreeSurface(sotetmezo2);
    SDL_DestroyTexture(sotetmezo2texture);
    SDL_FreeSurface(hatter);
    SDL_DestroyTexture(hattertexture);
    SDL_FreeSurface(novelo);
    SDL_DestroyTexture(novelotexture);
    SDL_FreeSurface(gomb);
    SDL_DestroyTexture(gombtexture);
    SDL_FreeSurface(vegegomb);
    SDL_DestroyTexture(vegegombtexture);
    SDL_FreeSurface(dobasgomb);
    SDL_DestroyTexture(dobasgombtexture);
    SDL_FreeSurface(dobaskep);
    SDL_DestroyTexture(dobaskeptexture);
    SDL_FreeSurface(vegehatterkep);
    SDL_DestroyTexture(vegehatterkeptexture);
    SDL_FreeSurface(koszoru);
    SDL_DestroyTexture(koszorutexture);
    SDL_FreeSurface(zoldhatter);
    SDL_DestroyTexture(zoldhattertexture);
    TTF_CloseFont(szamfont);
    TTF_CloseFont(betufont);
    SDL_FreeSurface(szoveg);
    SDL_DestroyTexture(szovegtexture);
    IMG_Quit();
    TTF_Quit();
}
