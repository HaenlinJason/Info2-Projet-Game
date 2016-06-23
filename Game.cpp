///////////////////////////////////////////////
//      fonction de la library
//      Fichier fonctions avec utilisation de class
//      date de création    : 20/05/2016
//      date de mise à jour : 23/06/2016
//      détails             :
//fonction de la classe GAME
///////////////////////////////////////////////
#define GAME
#ifdef GAME
////////////////////////////////////////////////////////////////
//Debut//
////////////////////////////////////////////////////////////////

//Include(s) => Bibliothéque(s)
#include "mbed.h"
#include "Game.h"
#include "GameConst.h"

//variables globales
unsigned int dat[TAILLE]= {NULL};
int tmp[TAILLE]= {NULL};
int D00[TAILLE]= {NULL};
int tabchiffre[TAILLE]= {NULL};
int nombre;
char Display_Buffer[2];
int i=0,j=0,sup1=0,sup2=0,add=0,mx=0,lvltest=0,HP_memo=0;

//Communication PC
//Serial pc(USBTX, USBRX);

//fonction(s)
Game::Game(PinName leD, PinName leC, PinName leB, PinName leA, PinName leG, PinName DI, PinName DO, PinName CLOCK, PinName cs):
    _leD(leD),_leC(leC),_leB(leB),_leA(leA),_leG(leG),_spi(DI,DO,CLOCK),_cs(cs)
{
    InitTabAff();
    reset();
    initprgm();
}
////////////////////////////////////////////////////////////////
//initialisation du programme//
////////////////////////////////////////////////////////////////
void Game::initprgm(void)
{
    clear();
    Difficulty(FAIBLE);
    _score = 0;
    _HP=0;
    _cs=1;
    _spi.format(8,0);//taille du bus SPI : 8 bits
    _spi.frequency(1000000); //fréquance du bus SPI : 1 MHz
    _tirV.attach(this,&Game::tirmoveV,TIRV);
    _balayage.attach(this,&Game::Display,0.0001);
    nombre=0;
}
////////////////////////////////////////////////////////////////
//Transmet les informations au programme principale//
////////////////////////////////////////////////////////////////
int Game::HP(void)     //la vie
{
    return V01.HP;
}
int Game::Score(void)  //le score
{
    if(V01.HP<HP_memo)
        _score=_score-10;
    HP_memo=V01.HP;
    return _score;
}
////////////////////////////////////////////////////////////////
//Alerte fin de niveau : Perdu ou Gagne //
////////////////////////////////////////////////////////////////
int Game::endlvl(void)//Gagne
{
    if(lvl.nbr==0)
        return 1;
    else
        return 0;
}
int Game::dead(void)  //Perdu
{
    if(V01.HP==0)
        return 1;
    else
        return 0;
}
////////////////////////////////////////////////////////////////
//Ajuste la difficulte//
////////////////////////////////////////////////////////////////
void Game::Difficulty (int mode)
{
    if(mode==FAIBLE) {
        //printf("FAIBLE");
        _Lvlstrong=0.17;
        _HP=0;
    } else if(mode==MOYEN) {
        //printf("MOYEN");
        _Lvlstrong=0.16;
        _HP=2;
    } else {
        //printf("FORT");
        _Lvlstrong=0.15;
        _HP=4;
    }
}
////////////////////////////////////////////////////////////////
//Affichage pour le menu//
////////////////////////////////////////////////////////////////
void Game::menuAff(void)
{
    tabMenu[0]=0xFFFF;
    tabMenu[1]=0x8001;
    tabMenu[2]=0xBFFD;
    tabMenu[3]=0xBFFD;
    tabMenu[4]=0xBFFD;
    tabMenu[5]=0xBFFD;
    tabMenu[6]=0xBFFD;
    tabMenu[7]=0xBFFD;
    tabMenu[8]=0xBFFD;
    tabMenu[9]=0xBFFD;
    tabMenu[10]=0xBFFD;
    tabMenu[11]=0xBFFD;
    tabMenu[12]=0xBFFD;
    tabMenu[13]=0xBFFD;
    tabMenu[14]=0x8001;
    tabMenu[15]=0xFFFF;
}
////////////////////////////////////////////////////////////////
//Play and Pause//
////////////////////////////////////////////////////////////////
void Game::Play(void)
{
    _tirV.attach(this,&Game::tirmoveV,TIRV);
    _tirAlien.attach(this,&Game::TtirAlien,_Lvlstrong*5);
    _tirA.attach(this,&Game::tirmoveA,_Lvlstrong);
    _Move.attach(this,&Game::LVlTestMove,0.7);
}
void Game::Pause(void)
{
    _tirV.detach();
    _tirAlien.detach();
    _tirA.detach();
    _Move.detach();
}
////////////////////////////////////////////////////////////////
//Clear and set => reset //
////////////////////////////////////////////////////////////////
void Game::clear(void)
{
    int i;
    A01.HP=0;
    for (i=0; i<TAILLE; i++) {
        TV1.tab[i]=0;
        TA1.tab[i]=0;
        V01.tab[i]=0;
        A01.tab[i]=0;
        tabMenu[i]=0;
    }
    A02=A01;
    A03=A01;
    A04=A01;
    A05=A01;
    A06=A01;
    A07=A01;
    A08=A01;
    A09=A01;
    A10=A01;
    A99=A01;
    A100=A01;
}
void Game::set(int vs,vaisseau *M0)
{
    //V01
    V01.HP=3;
    V01.y=1;
    V01.tab[0]=  0xE000;
    V01.tab[1]=  0x4000;
    switch (vs) {
        case Al1 :
            M0->tab[14]=  0x4000;
            M0->tab[15]=  0xE000;
            M0->y=14;
            break;
        case Al2 :
            M0->tab[13]=  0x4000;
            M0->tab[14]=  0xA000;
            M0->tab[15]=  0xE000;
            M0->y=14;
            break;
        case Al3 :
            M0->tab[13]=  0xA000;
            M0->tab[14]=  0x4000;
            M0->tab[15]=  0xE000;
            M0->y=13;
            break;
        case BOSS:
            M0->tab[10 ]=  0x0490;
            M0->tab[11 ]=  0x05D0;
            M0->tab[12 ]=  0x03E0;
            M0->tab[13 ]=  0x01C0;
            M0->tab[14 ]=  0x03E0;
            M0->y=10;
            break;
    }
}
void Game::reset(void)
{
    clear();
    set(Al1,&A01);
}
////////////////////////////////////////////////////////////////
//construction des niveaux //
////////////////////////////////////////////////////////////////
//****************************************************

//****************************************************
////////////////////////////////////////////////////////////////
//LVL de test //
////////////////////////////////////////////////////////////////
void Game::testLVL(void)
{
    _score=0;
    Affiche(1);
    set(Al1,&A01);
    lvl.HP=1;
    lvl.nbr=4;
    nombre=lvl.nbr;
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04=A01;
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,5,BAS);
    IngameMove(&A04,4,GAUCHE);
    Pause();
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 1 //
////////////////////////////////////////////////////////////////
void Game::lvl_1(void)
{
    _score=0;
    Affiche(1);
    set(Al1,&A01);
    lvl.HP=1;
    lvl.nbr=4;
    nombre=lvl.nbr;
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04=A01;
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,5,BAS);
    IngameMove(&A04,3,GAUCHE);
    Pause();
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 2 //
////////////////////////////////////////////////////////////////
void Game::lvl_2(void)
{
    //lvl parametre
    lvl.HP=1;
    lvl.nbr=6;
    nombre=lvl.nbr;
    //affiche '2' sur la matrice
    Affiche(2);
    //set
    set(Al1,&A01);
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04=A01;
    A05=A01;
    A06=A01;
    //placements
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,3,GAUCHE);
    IngameMove(&A05,3,GAUCHE);
    IngameMove(&A06,3,GAUCHE);
    IngameMove(&A04,5,BAS);
    IngameMove(&A05,1,BAS);
    IngameMove(&A06,9,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 3 //
////////////////////////////////////////////////////////////////
void Game::lvl_3(void)
{
    //lvl parametre
    lvl.HP=2;
    lvl.nbr=9;
    nombre=lvl.nbr;
    //affiche '3' sur la matrice
    Affiche(3);
    //set
    set(Al1,&A01);
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04=A01;
    A05=A01;
    A06=A01;
    A07=A01;
    A08=A01;
    A09=A01;
    //placements
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,3,GAUCHE);
    IngameMove(&A05,3,GAUCHE);
    IngameMove(&A06,3,GAUCHE);
    IngameMove(&A04,5,BAS);
    IngameMove(&A05,1,BAS);
    IngameMove(&A06,9,BAS);
    IngameMove(&A07,7,GAUCHE);
    IngameMove(&A08,7,GAUCHE);
    IngameMove(&A09,7,GAUCHE);
    IngameMove(&A07,5,BAS);
    IngameMove(&A08,1,BAS);
    IngameMove(&A09,9,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 4 //
////////////////////////////////////////////////////////////////
void Game::lvl_4(void)
{
    //lvl parametre
    lvl.HP=2;
    lvl.nbr=6;
    nombre=lvl.nbr;
    //affiche '4' sur la matrice
    Affiche(4);
    //set
    set(Al1,&A04);
    set(Al2,&A01);
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04.HP=_HP+lvl.HP;
    A05=A04;
    A06=A04;
    //placements
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,4,GAUCHE);
    IngameMove(&A05,4,GAUCHE);
    IngameMove(&A06,4,GAUCHE);
    IngameMove(&A04,5,BAS);
    IngameMove(&A05,1,BAS);
    IngameMove(&A06,9,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 5 //
////////////////////////////////////////////////////////////////
void Game::lvl_5(void)
{
    //lvl parametre
    lvl.HP=2;
    lvl.nbr=6;
    nombre=lvl.nbr;
    //affiche '5' sur la matrice
    Affiche(5);
    //set
    set(Al2,&A01);
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04=A01;
    A05=A01;
    A06=A01;
    //placements
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,4,GAUCHE);
    IngameMove(&A05,4,GAUCHE);
    IngameMove(&A06,4,GAUCHE);
    IngameMove(&A04,5,BAS);
    IngameMove(&A05,1,BAS);
    IngameMove(&A06,9,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 6 //
////////////////////////////////////////////////////////////////
void Game::lvl_6(void)
{
    //lvl parametre
    lvl.HP=2;
    lvl.nbr=9;
    nombre=lvl.nbr;
    //affiche '6' sur la matrice
    Affiche(6);
    //set
    set(Al1,&A07);
    set(Al2,&A01);
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04=A01;
    A05=A01;
    A06=A01;
    A07.HP=_HP+lvl.HP;
    A08=A07;
    A09=A07;
    //placements
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,4,GAUCHE);
    IngameMove(&A05,4,GAUCHE);
    IngameMove(&A06,4,GAUCHE);
    IngameMove(&A04,5,BAS);
    IngameMove(&A05,1,BAS);
    IngameMove(&A06,9,BAS);
    IngameMove(&A07,8,GAUCHE);
    IngameMove(&A08,8,GAUCHE);
    IngameMove(&A09,8,GAUCHE);
    IngameMove(&A07,5,BAS);
    IngameMove(&A08,1,BAS);
    IngameMove(&A09,9,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 7 //
////////////////////////////////////////////////////////////////
void Game::lvl_7(void)
{
    //lvl parametre
    lvl.HP=2;
    lvl.nbr=6;
    nombre=lvl.nbr;
    //affiche '7' sur la matrice
    Affiche(7);
    //set
    set(Al3,&A01);
    set(Al2,&A04);
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04.HP=_HP+lvl.HP;
    A05=A04;
    A06=A04;
    //placements
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,4,GAUCHE);
    IngameMove(&A05,4,GAUCHE);
    IngameMove(&A06,4,GAUCHE);
    IngameMove(&A04,5,BAS);
    IngameMove(&A05,1,BAS);
    IngameMove(&A06,9,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 8 //
////////////////////////////////////////////////////////////////
void Game::lvl_8(void)
{
    //lvl parametre
    lvl.HP=3;
    lvl.nbr=6;
    nombre=lvl.nbr;
    //affiche '8' sur la matrice
    Affiche(8);
    //set
    set(Al3,&A01);
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04=A01;
    A05=A01;
    A06=A01;
    //placements
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,4,GAUCHE);
    IngameMove(&A05,4,GAUCHE);
    IngameMove(&A06,4,GAUCHE);
    IngameMove(&A04,5,BAS);
    IngameMove(&A05,1,BAS);
    IngameMove(&A06,9,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 9 //
////////////////////////////////////////////////////////////////
void Game::lvl_9(void)
{
    //lvl parametre
    lvl.HP=3;
    lvl.nbr=9;
    nombre=lvl.nbr;
    //affiche '9' sur la matrice
    Affiche(9);
    //set
    set(Al3,&A01);
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03=A01;
    A04=A01;
    A05=A01;
    A06=A01;
    A07=A01;
    A08=A01;
    A09=A01;
    //placements
    IngameMove(&A01,1,BAS);
    IngameMove(&A02,5,BAS);
    IngameMove(&A03,9,BAS);
    IngameMove(&A04,4,GAUCHE);
    IngameMove(&A05,4,GAUCHE);
    IngameMove(&A06,4,GAUCHE);
    IngameMove(&A04,5,BAS);
    IngameMove(&A05,1,BAS);
    IngameMove(&A06,9,BAS);
    IngameMove(&A07,8,GAUCHE);
    IngameMove(&A08,8,GAUCHE);
    IngameMove(&A09,8,GAUCHE);
    IngameMove(&A07,5,BAS);
    IngameMove(&A08,1,BAS);
    IngameMove(&A09,9,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//LVL 10 //
////////////////////////////////////////////////////////////////
void Game::lvl_10(void)
{
    //lvl parametre
    lvl.HP=4;
    lvl.nbr=5;
    nombre=lvl.nbr;
    //affiche '10' sur la matrice
    Affiche(10);
    //set
    set(BOSS,&A100);
    set(Al1,&A01);
    set(Al3,&A03);
    A100.HP=_HP+lvl.HP+40;
    A01.HP=_HP+lvl.HP;
    A02=A01;
    A03.HP=_HP+lvl.HP;
    A04=A03;
    //placements
    IngameMove(&A01,3,BAS);
    IngameMove(&A01,7,GAUCHE);
    IngameMove(&A02,11,BAS);
    IngameMove(&A02,7,GAUCHE);
    IngameMove(&A03,13,BAS);
    //Pause
    Pause();
    //début lvl
    Play();
}
////////////////////////////////////////////////////////////////
//Deplacement des enemis durant le niveau//
////////////////////////////////////////////////////////////////
void Game::LVlTestMove(void)
{
    switch (lvltest) {
        case 0:
        case 1:
        case 2:
            IngameMove(&A01,1,BAS);
            IngameMove(&A02,1,BAS);
            IngameMove(&A03,1,BAS);
            IngameMove(&A04,1,HAUT);
            IngameMove(&A05,1,HAUT);
            IngameMove(&A06,1,HAUT);
            IngameMove(&A07,1,BAS);
            IngameMove(&A08,1,BAS);
            IngameMove(&A09,1,BAS);
            lvltest++;
            break;
        case 3:
        case 4:
        case 5:
            IngameMove(&A01,1,HAUT);
            IngameMove(&A02,1,HAUT);
            IngameMove(&A03,1,HAUT);
            IngameMove(&A04,1,BAS);
            IngameMove(&A05,1,BAS);
            IngameMove(&A06,1,BAS);
            IngameMove(&A07,1,HAUT);
            IngameMove(&A08,1,HAUT);
            IngameMove(&A09,1,HAUT);
            lvltest++;
            break;
    }
    switch(lvltest) {
        case 1 :
            IngameMove(&A100,1,HAUT);
            break;
        case 3 :
            IngameMove(&A100,1,BAS);
            break;
    }
    if(lvltest>=6)
        lvltest=0;
}
////////////////////////////////////////////////////////////////
//tirs des ennemis //
////////////////////////////////////////////////////////////////
void Game::TtirAlien(void)
{
    srand (time(NULL));
    int rdm1 = (rand()%nombre);
    int rdm2 = (rand()%4);
    switch (rdm1) {
        case 0:
            tirEnemy(A01);
            tirEnemy(A09);
            break;
        case 1:
            tirEnemy(A02);
            tirEnemy(A08);
            tirEnemy(A100);
            break;
        case 2:
            tirEnemy(A03);
            tirEnemy(A07);
            tirEnemy(A100);
            break;
        case 3:
            tirEnemy(A04);
            switch(rdm2) {
                case 0:
                    tirEnemy(A01);
                    break;
                case 1:
                    tirEnemy(A02);
                    break;
                case 2:
                    tirEnemy(A03);
                    break;
            }
            break;
        case 4:
            tirEnemy(A05);
            tirEnemy(A100);
            switch(rdm2) {
                case 0:
                    tirEnemy(A01);
                    break;
                case 1:
                    tirEnemy(A02);
                    break;
                case 2:
                    tirEnemy(A03);
                    break;
            }
            break;
        case 5:
            tirEnemy(A06);
            tirEnemy(A100);
            switch(rdm2) {
                case 0:
                    tirEnemy(A01);
                    break;
                case 1:
                    tirEnemy(A02);
                    break;
                case 2:
                    tirEnemy(A03);
                    break;
            }
            break;
        case 6:
            tirEnemy(A07);
            break;
        case 7:
            tirEnemy(A08);
            break;
        case 8:
            tirEnemy(A09);
            break;
        case 9:
            tirEnemy(A100);
            break;
    }
}
////////////////////////////////////////////////////////////////
//Verifie si un ennemi a etait touche, si Oui => detruit à 0 HP//
////////////////////////////////////////////////////////////////
void Game::ShipDestroyScan(void)
{
    shipdestroy(&V01,&TA1);
    shipdestroy(&A01,&TV1);
    shipdestroy(&A02,&TV1);
    shipdestroy(&A03,&TV1);
    shipdestroy(&A04,&TV1);
    shipdestroy(&A05,&TV1);
    shipdestroy(&A06,&TV1);
    shipdestroy(&A07,&TV1);
    shipdestroy(&A08,&TV1);
    shipdestroy(&A09,&TV1);
    shipdestroy(&A10,&TV1);
    shipdestroy(&A100,&TV1);
}
void Game::shipdestroy(vaisseau *M0,vaisseau *T0)
{
    int i,j,mask,T00[TAILLE],M00[TAILLE],l;
    for(mask=0; mask<TAILLE; mask++) {
        for(l=0; l<TAILLE; l++) {
            T00[l]=T0->tab[l] & (0x8000>>mask);
            M00[l]=M0->tab[l] & (0x8000>>mask);
        }
        for(i=0; i<TAILLE; i++) {
            if((M00[i]==T00[i]) && ((M00[i] != 0 && T00[i] != 0))) {
                M0->HP=M0->HP-1;//vie du vaisseau
                T0->tab[i]=(T0->tab[i])-(0x8000>>mask);
                if(M0->HP==0) {
                    _score=_score+10;//score du joueur
                    lvl.nbr--;
                    for(j=0; j<TAILLE ; j++) {
                        D00[j]=M0->tab[j];
                        tmp[j]=M0->tab[j];
                        M0->tab[j]=0;
                    }
                    if(mx==1) {
                        _supp1.attach(this,&Game::suppShip1,0.05);
                    } else {
                        _supp2.attach(this,&Game::suppShip2,0.05);
                    }
                    mx++;
                    if(mx>=2)
                        mx=1;
                }
                break;
            }
        }
    }
    //rattach pour eviter un bug ...
    _tirA.attach(this,&Game::tirmoveA,_Lvlstrong);
}
////////////////////////////////////////////////////////////////
//clignotement du vaisseau détruit//
////////////////////////////////////////////////////////////////
void Game::suppShip1(void)
{
    int j;
    switch (sup1) {
        case 0:
            for(j=0; j<TAILLE ; j++) {
                D00[j]=0;
            }
            sup1++;
            break;
        case 1:
            for(j=0; j<TAILLE ; j++) {
                D00[j]=tmp[j];
            }
            sup1++;
            break;
        case 2:
            for(j=0; j<TAILLE ; j++) {
                D00[j]=0;
            }
            sup1=NULL;
            _supp1.detach();
            break;

    }
}
void Game::suppShip2(void)
{
    int j;
    switch (sup2) {
        case 0:
            for(j=0; j<TAILLE ; j++) {
                D00[j]=0;
            }
            sup2++;
            break;
        case 1:
            for(j=0; j<TAILLE ; j++) {
                D00[j]=tmp[j];
            }
            sup2++;
            break;
        case 2:
            for(j=0; j<TAILLE ; j++) {
                D00[j]=0;
            }
            sup2=NULL;
            _supp2.detach();
            break;

    }
}
////////////////////////////////////////////////////////////////
//Effectue un tir //
////////////////////////////////////////////////////////////////
void Game::TirVaisseau(void)
{
    TV1.tab[V01.y]=V01.tab[V01.y];
}
void Game::tirEnemy(vaisseau M00)
{
    TA1.tab[M00.y]=TA1.tab[M00.y]|M00.tab[M00.y];
}
////////////////////////////////////////////////////////////////
//Deplacement des tirs //
////////////////////////////////////////////////////////////////
void Game::tirmoveA(void)
{
    tabmovehorizontal(&TA1,1,GAUCHE);
}
void Game::tirmoveV(void)
{
    tabmovehorizontal(&TV1,1,DROITE);
}
////////////////////////////////////////////////////////////////
//Deplacement des vaisseaux//
////////////////////////////////////////////////////////////////
void Game::IngameMove(vaisseau*M00,int move,int sens)
{
    int OK = MovePossibility(M00,sens);
    if(OK==1) {
        switch(sens) {
            case 1:
                tabmovevertical(M00,move,1);
                break;  // décalage vers le haut
            case 2:
                tabmovevertical(M00,move,2);
                break;  // décalage vers le bas
            case 3:
                tabmovehorizontal(M00,move,3);
                break;// décalage vers la droite
            case 4:
                tabmovehorizontal(M00,move,4);
                break;// décalage vers la gauche
            default:
                break;
        }
    }
}
void Game::Deplacement(int move,int sens)
{
    int OK = MovePossibility(&V01,sens);
    if(OK==1) {
        switch(sens) {
            case 1:
                tabmovevertical(&V01,move,1);
                break;  // décalage vers le haut
            case 2:
                tabmovevertical(&V01,move,2);
                break;  // décalage vers le bas
            case 3:
                tabmovehorizontal(&V01,move,3);
                break;// décalage vers la droite
            case 4:
                tabmovehorizontal(&V01,move,4);
                break;// décalage vers la gauche
            default:
                break;
        }
    }
}
////////////////////////////////////////////////////////////////
//Deplacement vertical //
////////////////////////////////////////////////////////////////
void Game::tabmovevertical(vaisseau *M00,int move,int sens)
{
    int m;
    if(sens==1) {
        for(m=0; m<TAILLE; m++) {
            M00->tab[m]=M00->tab[m]<<move;
        }
    } else if(sens==2) {
        for(m=0; m<TAILLE; m++) {
            M00->tab[m]=M00->tab[m]>>move;
        }

    }
}
////////////////////////////////////////////////////////////////
//Deplacement horizontal //
////////////////////////////////////////////////////////////////
void Game::tabmovehorizontal(vaisseau* M00,int move,int sens)
{
    unsigned int tmp[TAILLE]= {NULL};
    int m,o;
    if(sens==3) {
        M00->y=M00->y+move;
        for(o=1; o<=move; o++) {
            for(m=0; m<TAILLE; m++) {
                if(m==(TAILLE-1))
                    tmp[0]=0;
                else
                    tmp[m+1]=M00->tab[m];
            }
            for(m=0; m<TAILLE; m++) {
                M00->tab[m]=tmp[m];
            }
        }
    } else if(sens==4) {
        M00->y=M00->y-move;
        for(o=1; o<=move; o++) {
            for(m=0; m<TAILLE; m++) {
                if(m==0)
                    tmp[TAILLE-1]=0;
                else
                    tmp[m-1]=M00->tab[m];
            }
            for(m=0; m<TAILLE; m++) {
                M00->tab[m]=tmp[m];
            }
        }

    }
}
////////////////////////////////////////////////////////////////
//Verifie si le vaisseau est pas à la limite de la matrice //
////////////////////////////////////////////////////////////////
int Game::MovePossibility(vaisseau* M00,int test)
{
    int i;
    switch (test) {
        case 1:
            for (i=0; i<TAILLE; i++) {
                if((M00->tab[i]&0x8000) == 0x8000)
                    return 0;
            }
            break;
        case 2:
            for (i=0; i<TAILLE; i++) {
                if((M00->tab[i]&0x0001) == 0x0001)
                    return 0;
            }
            break;
        case 3:
            if(M00->tab[15]!=0)
                return 0;
            break;
        case 4:
            if(M00->tab[0]!=0)
                return 0;
            break;
    }
    return 1;
}
////////////////////////////////////////////////////////////////
//Affiche sur la matrice à led 16*16 //
////////////////////////////////////////////////////////////////
//*************************************************************

//*************************************************************
////////////////////////////////////////////////////////////////
//SPI //
////////////////////////////////////////////////////////////////
void Game::Display(void)
{
    dat[add]=(tabchiffre[add]|tabMenu[add]|V01.tab[add]|A01.tab[add]|A02.tab[add]|A03.tab[add]|A04.tab[add]|A05.tab[add]|A06.tab[add]|A07.tab[add]|
              A08.tab[add]|A09.tab[add]|A10.tab[add]|A99.tab[add]|A100.tab[add]|TV1.tab[add]|TA1.tab[add]|D00[add]);
    _leG.write(HIGH);
    Display_Buffer[0] = ~(unsigned char) (dat[add] & 0x00ff);
    Display_Buffer[1] = ~(unsigned char)((dat[add] & 0xff00)>>8);

    _cs=LOW;
    _spi.write(Display_Buffer[1]);
    _spi.write(Display_Buffer[0]);
    _cs=HIGH;

    Scan_Line(add);
    _leG.write(LOW);
    add++;
    if(add>=16)
        add=0;
}
////////////////////////////////////////////////////////////////
//Decodeur //
////////////////////////////////////////////////////////////////
//****************************************************

//****************************************************
void Game::Scan_Line(int m)
{
    switch(m) {
        case 0:
            _leD.write(LOW);
            _leC.write(LOW);
            _leB.write(LOW);
            _leA.write(LOW);
            break;
        case 1:
            _leD.write(LOW);
            _leC.write(LOW);
            _leB.write(LOW);
            _leA.write(HIGH);
            break;
        case 2:
            _leD.write(LOW);
            _leC.write(LOW);
            _leB.write(HIGH);
            _leA.write(LOW);
            break;
        case 3:
            _leD.write(LOW);
            _leC.write(LOW);
            _leB.write(HIGH);
            _leA.write(HIGH);
            break;
        case 4:
            _leD.write(LOW);
            _leC.write(HIGH);
            _leB.write(LOW);
            _leA.write(LOW);
            break;
        case 5:
            _leD.write(LOW);
            _leC.write(HIGH);
            _leB.write(LOW);
            _leA.write(HIGH);
            break;
        case 6:
            _leD.write(LOW);
            _leC.write(HIGH);
            _leB.write(HIGH);
            _leA.write(LOW);
            break;
        case 7:
            _leD.write(LOW);
            _leC.write(HIGH);
            _leB.write(HIGH);
            _leA.write(HIGH);
            break;
        case 8:
            _leD.write(HIGH);
            _leC.write(LOW);
            _leB.write(LOW);
            _leA.write(LOW);
            break;
        case 9:
            _leD.write(HIGH);
            _leC.write(LOW);
            _leB.write(LOW);
            _leA.write(HIGH);
            break;
        case 10:
            _leD.write(HIGH);
            _leC.write(LOW);
            _leB.write(HIGH);
            _leA.write(LOW);
            break;
        case 11:
            _leD.write(HIGH);
            _leC.write(LOW);
            _leB.write(HIGH);
            _leA.write(HIGH);
            break;
        case 12:
            _leD.write(HIGH);
            _leC.write(HIGH);
            _leB.write(LOW);
            _leA.write(LOW);
            break;
        case 13:
            _leD.write(HIGH);
            _leC.write(HIGH);
            _leB.write(LOW);
            _leA.write(HIGH);
            break;
        case 14:
            _leD.write(HIGH);
            _leC.write(HIGH);
            _leB.write(HIGH);
            _leA.write(LOW);
            break;
        case 15:
            _leD.write(HIGH);
            _leC.write(HIGH);
            _leB.write(HIGH);
            _leA.write(HIGH);
            break;
        default :
            break;
    }
}
////////////////////////////////////////////////////////////////
//Initialisation des tableaux pour afficher le début des lvls //
////////////////////////////////////////////////////////////////
void Game::InitTabAff(void)
{
    //UN/////////////////////
    tabUn[0 ]=  0xFFFF  ;
    tabUn[1 ]=  0x8001  ;
    tabUn[2 ]=  0xAFF5  ;
    tabUn[3 ]=  0x9009  ;
    tabUn[4 ]=  0xA005  ;
    tabUn[5 ]=  0xA005  ;
    tabUn[6 ]=  0xA105  ;
    tabUn[7 ]=  0xA225  ;
    tabUn[8 ]=  0xA7E5  ;
    tabUn[9 ]=  0xA025  ;
    tabUn[10]=  0xA005  ;
    tabUn[11]=  0xA005  ;
    tabUn[12]=  0x9009  ;
    tabUn[13]=  0xAFF5  ;
    tabUn[14]=  0x8001  ;
    tabUn[15]=  0xFFFF  ;

    //DEUX///////////////////
    tabDeux[0   ]=  0xFFFF  ;
    tabDeux[1   ]=  0x8001  ;
    tabDeux[2   ]=  0xAFF5  ;
    tabDeux[3   ]=  0x9009  ;
    tabDeux[4   ]=  0xA005  ;
    tabDeux[5   ]=  0xA005  ;
    tabDeux[6   ]=  0xA9E5  ;
    tabDeux[7   ]=  0xA925  ;
    tabDeux[8   ]=  0xA925  ;
    tabDeux[9   ]=  0xAF25  ;
    tabDeux[10  ]=  0xA005  ;
    tabDeux[11  ]=  0xA005  ;
    tabDeux[12  ]=  0x9009  ;
    tabDeux[13  ]=  0xAFF5  ;
    tabDeux[14  ]=  0x8001  ;
    tabDeux[15  ]=  0xFFFF  ;

    //TROIS//////////////////
    tabTrois[0  ]=  0xFFFF  ;
    tabTrois[1  ]=  0x8001  ;
    tabTrois[2  ]=  0xAFF5  ;
    tabTrois[3  ]=  0x9009  ;
    tabTrois[4  ]=  0xA005  ;
    tabTrois[5  ]=  0xA005  ;
    tabTrois[6  ]=  0xA925  ;
    tabTrois[7  ]=  0xA925  ;
    tabTrois[8  ]=  0xA925  ;
    tabTrois[9  ]=  0xAFE5  ;
    tabTrois[10 ]=  0xA005  ;
    tabTrois[11 ]=  0xA005  ;
    tabTrois[12 ]=  0x9009  ;
    tabTrois[13 ]=  0xAFF5  ;
    tabTrois[14 ]=  0x8001  ;
    tabTrois[15 ]=  0xFFFF  ;

    //QUATRE/////////////////
    tabQuatre[0 ]=  0xFFFF  ;
    tabQuatre[1 ]=  0x8001  ;
    tabQuatre[2 ]=  0xAFF5  ;
    tabQuatre[3 ]=  0x9009  ;
    tabQuatre[4 ]=  0xA005  ;
    tabQuatre[5 ]=  0xA005  ;
    tabQuatre[6 ]=  0xAF05  ;
    tabQuatre[7 ]=  0xA105  ;
    tabQuatre[8 ]=  0xA105  ;
    tabQuatre[9 ]=  0xAFE5  ;
    tabQuatre[10]=  0xA005  ;
    tabQuatre[11]=  0xA005  ;
    tabQuatre[12]=  0x9009  ;
    tabQuatre[13]=  0xAFF5  ;
    tabQuatre[14]=  0x8001  ;
    tabQuatre[15]=  0xFFFF  ;

    //CINQ///////////////////
    tabCinq[0   ]=  0xFFFF  ;
    tabCinq[1   ]=  0x8001  ;
    tabCinq[2   ]=  0xAFF5  ;
    tabCinq[3   ]=  0x9009  ;
    tabCinq[4   ]=  0xA005  ;
    tabCinq[5   ]=  0xA005  ;
    tabCinq[6   ]=  0xAF25  ;
    tabCinq[7   ]=  0xA925  ;
    tabCinq[8   ]=  0xA925  ;
    tabCinq[9   ]=  0xA9E5  ;
    tabCinq[10  ]=  0xA005  ;
    tabCinq[11  ]=  0xA005  ;
    tabCinq[12  ]=  0x9009  ;
    tabCinq[13  ]=  0xAFF5  ;
    tabCinq[14  ]=  0x8001  ;
    tabCinq[15  ]=  0xFFFF  ;

    //SIX////////////////////
    tabSix[0    ]=  0xFFFF  ;
    tabSix[1    ]=  0x8001  ;
    tabSix[2    ]=  0xAFF5  ;
    tabSix[3    ]=  0x9009  ;
    tabSix[4    ]=  0xA005  ;
    tabSix[5    ]=  0xA005  ;
    tabSix[6    ]=  0xAFE5  ;
    tabSix[7    ]=  0xA925  ;
    tabSix[8    ]=  0xA925  ;
    tabSix[9    ]=  0xA9E5  ;
    tabSix[10   ]=  0xA005  ;
    tabSix[11   ]=  0xA005  ;
    tabSix[12   ]=  0x9009  ;
    tabSix[13   ]=  0xAFF5  ;
    tabSix[14   ]=  0x8001  ;
    tabSix[15   ]=  0xFFFF  ;

    //SEPT///////////////////
    tabSept[0   ]=  0xFFFF  ;
    tabSept[1   ]=  0x8001  ;
    tabSept[2   ]=  0xAFF5  ;
    tabSept[3   ]=  0x9009  ;
    tabSept[4   ]=  0xA005  ;
    tabSept[5   ]=  0xA005  ;
    tabSept[6   ]=  0xA805  ;
    tabSept[7   ]=  0xA805  ;
    tabSept[8   ]=  0xA805  ;
    tabSept[9   ]=  0xAFE5  ;
    tabSept[10  ]=  0xA005  ;
    tabSept[11  ]=  0xA005  ;
    tabSept[12  ]=  0x9009  ;
    tabSept[13  ]=  0xAFF5  ;
    tabSept[14  ]=  0x8001  ;
    tabSept[15  ]=  0xFFFF  ;

    //HUIT///////////////////
    tabHuit[0   ]=  0xFFFF  ;
    tabHuit[1   ]=  0x8001  ;
    tabHuit[2   ]=  0xAFF5  ;
    tabHuit[3   ]=  0x9009  ;
    tabHuit[4   ]=  0xA005  ;
    tabHuit[5   ]=  0xA005  ;
    tabHuit[6   ]=  0xAFE5  ;
    tabHuit[7   ]=  0xA925  ;
    tabHuit[8   ]=  0xA925  ;
    tabHuit[9   ]=  0xAFE5  ;
    tabHuit[10  ]=  0xA005  ;
    tabHuit[11  ]=  0xA005  ;
    tabHuit[12  ]=  0x9009  ;
    tabHuit[13  ]=  0xAFF5  ;
    tabHuit[14  ]=  0x8001  ;
    tabHuit[15  ]=  0xFFFF  ;

    //NEUF///////////////////
    tabNeuf[0   ]=  0xFFFF  ;
    tabNeuf[1   ]=  0x8001  ;
    tabNeuf[2   ]=  0xAFF5  ;
    tabNeuf[3   ]=  0x9009  ;
    tabNeuf[4   ]=  0xA005  ;
    tabNeuf[5   ]=  0xA005  ;
    tabNeuf[6   ]=  0xAF25  ;
    tabNeuf[7   ]=  0xA925  ;
    tabNeuf[8   ]=  0xA925  ;
    tabNeuf[9   ]=  0xAFE5  ;
    tabNeuf[10  ]=  0xA005  ;
    tabNeuf[11  ]=  0xA005  ;
    tabNeuf[12  ]=  0x9009  ;
    tabNeuf[13  ]=  0xAFF5  ;
    tabNeuf[14  ]=  0x8001  ;
    tabNeuf[15  ]=  0xFFFF  ;

    //DIX////////////////////
    tabDix[0    ]=  0xFFFF  ;
    tabDix[1    ]=  0x8001  ;
    tabDix[2    ]=  0xAFF5  ;
    tabDix[3    ]=  0x9009  ;
    tabDix[4    ]=  0xA005  ;
    tabDix[5    ]=  0xA7E5  ;
    tabDix[6    ]=  0xA005  ;
    tabDix[7    ]=  0xA7E5  ;
    tabDix[8    ]=  0xA425  ;
    tabDix[9    ]=  0xA425  ;
    tabDix[10   ]=  0xA7E5  ;
    tabDix[11   ]=  0xA005  ;
    tabDix[12   ]=  0x9009  ;
    tabDix[13   ]=  0xAFF5  ;
    tabDix[14   ]=  0x8001  ;
    tabDix[15   ]=  0xFFFF  ;
}
////////////////////////////////////////////////////////////////
//AFFICHE 1 à 10//
////////////////////////////////////////////////////////////////
void Game::Affiche(int chiffre)
{
    clear();
    int i,j;
    switch (chiffre) {
        case 1:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabUn[i];
            }
            break;
        case 2:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabDeux[i];
            }
            break;
        case 3:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabTrois[i];
            }
            break;
        case 4:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabQuatre[i];
            }
            break;
        case 5:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabCinq[i];
            }
            break;
        case 6:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabSix[i];
            }
            break;
        case 7:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabSept[i];
            }
            break;
        case 8:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabHuit[i];
            }
            break;
        case 9:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabNeuf[i];
            }
            break;
        case 10:
            for(i=0; i<TAILLE; i++) {
                tabchiffre[i]=tabDix[i];
            }
            break;
    }
    wait(2);
    for(j=0; j<TAILLE; j++) {
        for(i=0; i<TAILLE; i++) {
            tabchiffre[j]=tabchiffre[j]<<1;
        }
    }
}
////////////////////////////////////////////////////////////////
//Fin//
////////////////////////////////////////////////////////////////
#endif //GAME
