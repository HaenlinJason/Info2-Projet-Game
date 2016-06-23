///////////////////////////////////////////////
//      programme principale
//
//      date de création    :26/05/2016
//      date de mise à jour :05/06/2016
//      détails             :
//Machine à états avec LCD
///////////////////////////////////////////////

//Bibliothéque(s)
#include "mbed.h"
#include "Game.h"
#include "GameConst.h"
#include "Music.h"
#include "MusicConst.h"
#include "TextLCD.h"

#define JEU
#ifdef JEU
////////////////////////////////////////////////////////////////
//Debut//
////////////////////////////////////////////////////////////////

//Communication pc (UART)
Serial pc(USBTX, USBRX); // tx, rx - Liaison série

//PORT DE COMMUNICATION
//DECODEUR
#define LEDARRAY_D D0 //Décodeur enable Input (D0)
#define LEDARRAY_C D1 //décodeur Input        (D1)
#define LEDARRAY_B D2 //décodeur Input
#define LEDARRAY_A D3 //décodeur Input
#define LEDARRAY_G D4 //Décodeur enable Input
//SPI
#define LEDARRAY_DI D11   // MOSI SPI
#define LEDARRAY_DO PTC7  // MISO SPI broche paumé sans utilité
#define LEDARRAY_CLK D13  // clock SPI
#define LEDARRAY_LAT PTC2 // cs Output
//JOYSTICK
#define VRx A0     // Define the input pin for VRx pin
#define VRy A1     // Define the input pin for VRy pin
#define SW1 D12    // Define the input pin for SW pin
//BUTTONS
#define SW3 D15
#define SW2 D14
//LCD
#define RS    D5
#define E     D6
#define PinD4 D7
#define PinD5 D8
#define PinD6 D9
#define PinD7 D10
//HAUT PARLEUR
#define HP1   A2
#define HP2   A3
// CONSTANTES
#define HIGH    1
#define LOW     0
#define PAS     100
#define END     3000
#define T_SCR   250
#define T_SCRF  2000
#define MAXSIZE 39

// Definition des constantes evenement
#define RIEN       0
#define JOYHAUT    1
#define JOYBAS     2
#define JOYDROITE  3
#define JOYGAUCHE  4
#define SWJOY      5
#define SWPLAY     6
#define SWBACK     7
#define FINLVL1    8
#define FINLVL2    9
#define FINLVL3    10
#define FINLVL4    11
#define FINLVL5    12
#define FINLVL6    13
#define FINLVL7    14
#define FINLVL8    15
#define FINLVL9    16
#define FINLVL10   17
#define TEMPS      18
#define CHRONO     19
#define LOST       20
#define SCROLL     21

// Definition des constantes etats
#define LCDPLAY    0
#define LCDPARA    1
#define EASY       2
#define MIDDLE     3
#define HARD       4
#define PERDU      5
#define GAGNE      6
#define LCDPAUSE   7
#define LCDMENU    8
#define LCDGAME    9
#define LVL       10
#define LCDSCORE  11
#define SCORE     12
#define PSEUDO    13

//Definition des constantes etats de niveaux
#define LVL1       1
#define LVL2       2
#define LVL3       3
#define LVL4       4
#define LVL5       5
#define LVL6       6
#define LVL7       7
#define LVL8       8
#define LVL9       9
#define LVL10      10

//DECLARATION DES PORTS DE COMMUNICATIONS
TextLCD lcd (RS,E,PinD4,PinD5,PinD6,PinD7); //lcd 16*2
Game space(LEDARRAY_D,LEDARRAY_C,LEDARRAY_B,LEDARRAY_A,LEDARRAY_G,LEDARRAY_DI,LEDARRAY_DO,LEDARRAY_CLK,LEDARRAY_LAT);
Music son(HP1,HP2);
AnalogIn x_axis(VRx);                   // Create the analog x movement object
AnalogIn y_axis(VRy);                   // Create the analog y movement object
DigitalIn button1(SW1, PullUp);         // Create the digital button object and setup internall pull-up resistor
DigitalIn button2(SW2, PullUp);
DigitalIn button3(SW3, PullUp);


// PROTOTYPES
int  EventHandler(void) ;
void Init(void) ;
void Algo(void) ;
int A00(void),A01(void),A02(void),A03(void),A04(void),A05(void),A06(void),A07(void),A08(void),A09(void),A10(void),A11(void),A12(void),A13(void),A14(void),A15(void),A16(void),A17(void),A18(void),A19(void),A20(void);
int A21(void),A22(void),A23(void),A24(void),A25(void),A26(void),A27(void),A28(void),A29(void),A30(void),A31(void),A32(void),A33(void),A34(void),A35(void),A36(void),A37(void),A38(void),A39(void),A40(void),A41(void);
int A42(void),A43(void),A44(void),A45(void),A46(void),A47(void),A48(void),A49(void),A50(void),A51(void);
void Affiche(void);
void Gestion(void);
void shipscan(void);
void LCD_FirstScroll(char *Message);
void LCD_NextScroll(void);
char LettrePseudo(void);


// GLOBALS
int SelectionMusic;
int T_MS1;
int T_MS2;
int giH,giM,giS,giD;
int Indice;
int ScIndice;
int scl;
int gilen;
int IndicePseudo;
char pseudo[10]= {NULL};
char PSO[10]= {NULL};
char PSF[40]= {NULL};
char NewMessage[MAXSIZE+1];

//tableau des scores
char ctScore0[40]="JAREON Score=1000 Temps=00:00:59";
char ctScore1[40]="PAUL Score=9999 Temps=00:00:59 ";
char ctScore2[40]="DilNoob Score=-9999999 Temps=30:00:00";
char ctScore3[40]="vide";
char ctScore4[40]="vide";
char ctScore5[40]="vide";
char ctScore6[40]="vide";
char ctScore7[40]="vide";
char ctScore8[40]="vide";
char ctScore9[40]="vide";
//lettres
char Lettres[29]= {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' ',' '};
int  Clettre[10]= {NULL};


//évenements temporels
Ticker scanShip;
Timer Scrolling;
Timer timemove;
Timer timetir;
Timer EndTime;
Timer Chrono;

// Variables globales evenements
int giEvent ;

// Variable d'etat du systeme
int giEtat ;
int giStatelvl;

// Le coeur de la FSM  : un tableau de pointeurs sur fonctions de type
// int fct (void) ; 22 lignes (evenements) - 14 colonnes (etats)
int (*Automate[22][14])(void)= {
    //  LCDPLAY LCDPARA EASY    MIDDLE  HARD    PERDU   GAGNE   LCDPAUSE LCDMENU LCDGAME LVL     LCDSCORE SCORE   PSEUDO
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A00,    A00,     A00,    A00}, //RIEN
    {   A00,    A02,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A32,    A00,     A46,    A48}, //JOYHAUT
    {   A01,    A00,    A00,    A00,    A00,    A00,    A00,    A27,    A00,     A00,    A33,    A01,     A43,    A47}, //JOYBAS
    {   A42,    A42,    A05,    A07,    A00,    A00,    A00,    A29,    A00,     A27,    A34,    A00,     A00,    A49}, //JOYDROITE
    {   A00,    A00,    A00,    A06,    A08,    A00,    A00,    A00,    A29,     A28,    A35,    A02,     A00,    A50}, //JOYGAUCHE
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A37,    A00,     A00,    A00}, //SWJOY
    {   A12,    A03,    A11,    A10,    A09,    A00,    A00,    A00,    A38,     A26,    A36,    A43,     A00,    A51}, //SWPLAY
    {   A00,    A00,    A04,    A04,    A04,    A00,    A00,    A40,    A40,     A40,    A25,    A00,     A45,    A00}, //SWBACK
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A13,    A00,     A00,    A00}, //FINLVL1
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A14,    A00,     A00,    A00}, //FINLVL2
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A15,    A00,     A00,    A00}, //FINLVL3
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A16,    A00,     A00,    A00}, //FINLVL4
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A17,    A00,     A00,    A00}, //FINLVL5
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A18,    A00,     A00,    A00}, //FINLVL6
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A19,    A00,     A00,    A00}, //FINLVL7
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A20,    A00,     A00,    A00}, //FINLVL8
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A21,    A00,     A00,    A00}, //FINLVL9
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A22,    A00,     A00,    A00}, //FINLVL10
    {   A00,    A00,    A00,    A00,    A00,    A38,    A23,    A00,    A00,     A00,    A00,    A00,     A00,    A00}, //TEMPS
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A41,    A00,     A00,    A00}, //CHRONO
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A24,    A00,     A00,    A00}, //LOST
    {   A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,    A00,     A00,    A00,    A00,     A44,    A00}, //SCROLL

};

/////////////////////////////////////////////////////////////////////////
// Programme principal
/////////////////////////////////////////////////////////////////////////
int main()
{
    printf("Demarrage du programme\n\r") ;
    Init() ;

    while(true) {

        // Appel au gestionnaire d'evenements
        giEvent = EventHandler() ;
        // Appel a la machine a etats
        Algo() ;
    }
}

/////////////////////////////////////////////////////////////////////////
// Gestionnaire d'evenements
/////////////////////////////////////////////////////////////////////////
int EventHandler(void)
{
    if(EndTime.read_ms()>=END) {
        EndTime.stop();
        EndTime.reset();
        //printf("return TEMPS");
        return TEMPS;
    }
    if(Chrono.read_ms()>=PAS) {
        Chrono.reset();
        return CHRONO;
    }
    if(Scrolling.read_ms()>=T_SCRF && scl==0) {
        Scrolling.reset();
        scl=1;
        return SCROLL;
    } else if(Scrolling.read_ms()>=T_SCR && scl==1) {
        Scrolling.reset();
        return SCROLL;
    }
    //SWPLAY
    if(button3.read()==0&&timetir.read_ms() >=T_MS2) {
        timetir.reset();
        return SWPLAY;
        //SWBACK
    } else if(button2.read()==0&&timetir.read_ms() >=T_MS2) {
        timetir.reset();
        return SWBACK;
        //SWJOY
    } else if(button1.read()==0&&timetir.read_ms() >=T_MS2) {
        timetir.reset();
        return SWJOY;
        //JOYDROITE
    } else if(x_axis.read()>0.8) {
        if(timemove.read_ms() >=T_MS1) {
            timemove.reset();
            return JOYDROITE;
        }
        //JOYGAUCHE
    } else if(x_axis.read()<0.6) {
        if(timemove.read_ms() >=T_MS1) {
            timemove.reset();
            return JOYGAUCHE;
        }
        //JOYBAS
    } else if(y_axis.read()>0.8) {
        if(timemove.read_ms() >=T_MS1) {
            timemove.reset();
            return JOYBAS;
        }
        //JOYHAUT
    } else if(y_axis.read()<0.6) {
        if(timemove.read_ms() >=T_MS1) {
            timemove.reset();
            return JOYHAUT;
        }
    }
    //Mort
    if(space.dead()==1)
        return LOST;
    //LVL
    switch (giStatelvl) {
        case LVL1 :
            if(space.endlvl()==1)
                return FINLVL1;
            break;
        case LVL2 :
            if(space.endlvl()==1)
                return FINLVL2;
            break;
        case LVL3 :
            if(space.endlvl()==1)
                return FINLVL3;
            break;
        case LVL4 :
            if(space.endlvl()==1)
                return FINLVL4;
            break;
        case LVL5 :
            if(space.endlvl()==1)
                return FINLVL5;
            break;
        case LVL6 :
            if(space.endlvl()==1)
                return FINLVL6;
            break;
        case LVL7 :
            if(space.endlvl()==1)
                return FINLVL7;
            break;
        case LVL8 :
            if(space.endlvl()==1)
                return FINLVL8;
            break;
        case LVL9 :
            if(space.endlvl()==1)
                return FINLVL9;
            break;
        case LVL10 :
            if(space.endlvl()==1)
                return FINLVL10;
            break;
    }
    //FIN
    return RIEN;
}
/////////////////////////////////////////////////////////////////////////
// Les moteurs d'automate
/////////////////////////////////////////////////////////////////////////
void Algo(void)
{
    int (*FctPtr)(void);
    FctPtr = Automate [giEvent][giEtat];
    giEtat = (*FctPtr)();
}
/////////////////////////////////////////////////////////////////////////
// Les actions élémentaires
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//Initialisation//
////////////////////////////////////////////////////////////////
void Init(void)
{
    son.MusicOff();
    scanShip.attach(&shipscan,0.2);
    giEvent = RIEN;
    giEtat = LCDPLAY;
    giStatelvl = LVL1;
    lcd.cls();        //efface LCD
    timemove.stop();
    timemove.reset(); //reset timer
    timemove.start();
    timetir.stop();
    timetir.reset();
    timetir.start();
    Scrolling.stop();
    Scrolling.reset();
    Chrono.stop();
    Chrono.reset();
    EndTime.stop();
    EndTime.reset();
    giH = giS = giM = giD = 0;
    ScIndice=-1;
    gilen=0;
    scl=0;
    IndicePseudo=0;
    lcd.printf("=>Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    T_MS1=500; //timer mouvement
    T_MS2=500; //timer tir
    //à 500ms dans le menu pour plus de précision
    //sinon T_MS1=100ms
    //      T_MS2=150ms
    space.menuAff();
}
////////////////////////////////////////////////////////////////
//R.A.S.//
////////////////////////////////////////////////////////////////
int A00(void)
{
    return giEtat;
}
////////////////////////////////////////////////////////////////
//MENU//
////////////////////////////////////////////////////////////////
int A01(void)
{
    T_MS1=500;
    T_MS2=500;
    lcd.cls();
    lcd.printf("  Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("=>Difficulte");
    printf("Action = A01\n\r");
    printf("Etat = LCDPARA\n\r");
    return LCDPARA;
}
int A02(void)
{
    lcd.cls();
    lcd.printf("=>Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    printf("Action = A02\n\r");
    printf("Etat = LCDPLAY\n\r");
    return LCDPLAY;
}
int A03(void)
{
    lcd.cls();
    lcd.printf("=>Difficulte<=");
    lcd.locate(0,1);
    lcd.printf("Facile=>");
    printf("Action = A03\n\r");
    printf("Etat = EASY\n\r");
    return EASY;
}
int A04(void)
{
    lcd.cls();
    lcd.printf("  Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("=>Difficulte");
    printf("Action = A04\n\r");
    printf("Etat = LCDPARA\n\r");
    return LCDPARA;
}
int A05(void)
{
    lcd.cls();
    lcd.printf("=>Difficulte<=");
    lcd.locate(0,1);
    lcd.printf("<=Moyen=>");
    printf("Action = A05\n\r");
    printf("Etat = MIDDLE\n\r");
    return MIDDLE;
}
int A06(void)
{
    lcd.cls();
    lcd.printf("=>Difficulte<=");
    lcd.locate(0,1);
    lcd.printf("Facile=>");
    printf("Action = A06\n\r");
    printf("Etat = EASY\n\r");
    return EASY;
}
int A07(void)
{
    lcd.cls();
    lcd.printf("=>Difficulte<=");
    lcd.locate(0,1);
    lcd.printf("<=Difficile");
    printf("Action = A07\n\r");
    printf("Etat = HARD\n\r");
    return HARD;
}
int A08(void)
{
    lcd.cls();
    lcd.printf("=>Difficulte<=");
    lcd.locate(0,1);
    lcd.printf("<=Moyen=>");
    printf("Action = A08\n\r");
    printf("Etat = MIDDLE\n\r");
    return MIDDLE;
}
////////////////////////////////////////////////////////////////
// Parametre => difficulte => retour menu //
////////////////////////////////////////////////////////////////
int A09(void)//FORT
{
    space.Difficulty(FORT);
    lcd.cls();
    lcd.printf("=>Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    printf("Action = A09\n\r");
    printf("Etat = LCDPLAY\n\r");
    return LCDPLAY;
}
int A10(void)//MOYEN
{
    space.Difficulty(MOYEN);
    lcd.cls();
    lcd.printf("=>Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    printf("Action = A10\n\r");
    printf("Etat = LCDPLAY\n\r");
    return LCDPLAY;
}
int A11(void)//FAIBLE
{
    space.Difficulty(FAIBLE);
    lcd.cls();
    lcd.printf("=>Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    printf("Action = A11\n\r");
    printf("Etat = LCDPLAY\n\r");
    return LCDPLAY;
}
////////////////////////////////////////////////////////////////
//LVL => 1,2,3,4,5,6,7,8,9,10//
////////////////////////////////////////////////////////////////
int A12(void)//lvl1
{
    SelectionMusic=2;
    son.MusicOff();
    son.MusicSelect(SelectionMusic);
    scanShip.attach(&shipscan,0.2);
    space.reset();
    T_MS1=100;
    T_MS2=150;
    space.lvl_1();
    Chrono.reset();
    Chrono.start();
    giStatelvl=LVL1;
    space.Play();
    lcd.cls();
    lcd.printf("=> LVL");
    printf("Action = A12\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A13(void)//fin lvl1 debut lvl2
{
    space.clear();
    space.lvl_2();
    giStatelvl=LVL2;
    printf("Action = A13\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A14(void)//fin lvl2 debut lvl3
{
    space.clear();
    space.lvl_3();
    giStatelvl=LVL3;
    printf("Action = A14\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A15(void)//fin lvl3 debut lvl4
{
    SelectionMusic=1;
    son.MusicOff();
    son.MusicSelect(SelectionMusic);
    space.clear();
    space.lvl_4();
    giStatelvl=LVL4;
    printf("Action = A15\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A16(void)//fin lvl4 debut lvl5
{
    space.clear();
    space.lvl_5();
    giStatelvl=LVL5;
    printf("Action = A16\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A17(void)//fin lvl5 debut lvl6
{
    space.clear();
    space.lvl_6();
    giStatelvl=LVL6;
    printf("Action = A17\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A18(void)//fin lvl6 debut lvl7
{
    SelectionMusic=4;
    son.MusicOff();
    son.MusicSelect(SelectionMusic);
    space.clear();
    space.lvl_7();
    giStatelvl=LVL7;
    printf("Action = A18\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A19(void)//fin lvl7 debut lvl8
{
    space.clear();
    space.lvl_8();
    giStatelvl=LVL8;
    printf("Action = A19\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A20(void)//fin lvl8 debut lvl9
{
    space.clear();
    space.lvl_9();
    giStatelvl=LVL9;
    printf("Action = A20\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A21(void)//fin lvl9 debut lvl10
{
    SelectionMusic=3;
    son.MusicOff();
    son.MusicSelect(SelectionMusic);
    space.clear();
    space.lvl_10();
    giStatelvl=LVL10;
    printf("Action = A21\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
////////////////////////////////////////////////////////////////
//Gagne //
////////////////////////////////////////////////////////////////
int A22(void)//fin lvl10 gagne
{
    space.Pause();
    space.clear();
    space.menuAff();
    T_MS1=250;
    T_MS2=250;
    lcd.cls();
    lcd.printf("GAGNE! GAGNE!");
    Chrono.stop();
    son.MusicOff();
    son.Win();
    EndTime.reset();
    EndTime.start();
    printf("Action = A22\n\r");
    printf("Etat = GAGNE\n\r");
    return GAGNE;
}
////////////////////////////////////////////////////////////////
//PSEUDO//
////////////////////////////////////////////////////////////////
int A23(void)
{
    space.Pause();
    space.clear();
    space.menuAff();
    sprintf(PSO,"pseudo: %c%c%c%c%c",pseudo[0],pseudo[1],pseudo[2],pseudo[3],pseudo[4]);
    lcd.cls();
    lcd.printf("%s",PSO);
    printf("Action = A23\n\r");
    printf("Etat = PSEUDO\n\r");
    return PSEUDO;
}
////////////////////////////////////////////////////////////////
//Perdu/
////////////////////////////////////////////////////////////////
int A24(void)//perdu
{
    lcd.cls();
    lcd.printf("PERDU! PERDU!");
    Chrono.stop();
    son.MusicOff();
    son.Loose();
    EndTime.reset();
    EndTime.start();
    printf("Action = A24\n\r");
    printf("Etat = PERDU\n\r");
    return PERDU;
}
////////////////////////////////////////////////////////////////
//Pause du jeu //
////////////////////////////////////////////////////////////////
int A25(void)
{
    son.MusicOff();
    scanShip.detach();
    T_MS1=500;
    T_MS2=500;
    space.Pause();
    Chrono.stop();
    lcd.cls();
    lcd.printf("[]***PAUSE***[]");
    lcd.locate(0,1);
    lcd.printf("=>Jouer     Menu");
    printf("Action = A25\n\r");
    printf("Etat = LCDGAME\n\r");
    return LCDGAME;
}
int A26(void)
{
    T_MS1=100;
    T_MS2=150;
    space.Play();
    scanShip.attach(&shipscan,0.2);
    son.MusicSelect(SelectionMusic);
    Chrono.reset();
    Chrono.start();
    lcd.cls();
    printf("Action = A26\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A27(void)
{
    lcd.cls();
    lcd.printf("[]***PAUSE***[]");
    lcd.locate(0,1);
    lcd.printf("  Jouer   =>Menu");
    printf("Action = A27\n\r");
    printf("Etat = LCDMENU\n\r");
    return LCDMENU;
}
int A28(void)
{

    lcd.cls();
    lcd.printf("[]***PAUSE***[]");
    lcd.locate(0,1);
    lcd.printf("=>Jouer     Menu");
    printf("Action = A28\n\r");
    printf("Etat = LCDMENU\n\r");
    return LCDMENU;
}
int A29(void)
{
    lcd.cls();
    lcd.printf("[]***PAUSE***[]");
    lcd.locate(0,1);
    lcd.printf("=>Jouer     Menu");
    printf("Action = A29\n\r");
    printf("Etat = LCDGAME\n\r");
    return LCDGAME;
}
int A30(void)//pas utilisé
{
    lcd.cls();
    lcd.printf("[]***PAUSE***[]");
    lcd.locate(0,1);
    lcd.printf("=>Jouer     Menu");
    printf("Action = A30\n\r");
    printf("Etat = LCDMENU\n\r");
    return LCDMENU;
}
int A31(void)//pas utilisé
{
    lcd.cls();
    lcd.printf("[]***PAUSE***[]");
    lcd.locate(0,1);
    lcd.printf("=>Jouer     Menu");
    printf("Action = A31\n\r");
    printf("Etat = LCDGAME\n\r");
    return LCDGAME;
}
////////////////////////////////////////////////////////////////
//Deplacement en partie//
////////////////////////////////////////////////////////////////
int A32(void)//HAUT
{
    space.Deplacement(1,HAUT);
    printf("Action = A32\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A33(void)//BAS
{
    space.Deplacement(1,BAS);
    printf("Action = A33\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A34(void)//DROITE
{
    space.Deplacement(1,DROITE);
    printf("Action = A34\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}

int A35(void)//GAUCHE
{
    space.Deplacement(1,GAUCHE);
    printf("Action = A35\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A36(void)//SWPLAY
{
    space.TirVaisseau();
    printf("Action = A36\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
int A37(void)//SWJOY
{
    space.TirVaisseau();
    printf("Action = A37\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
////////////////////////////////////////////////////////////////
//retour Menu//
////////////////////////////////////////////////////////////////
int A38(void)
{
    son.MusicOff();
    space.Pause();
    space.clear();
    space.menuAff();
    lcd.cls();
    lcd.printf("=>Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    giH = giS = giM = giD = 0;
    printf("Action = A38\n\r");
    printf("Etat = LCDPLAY\n\r");
    return LCDPLAY;
}
int A39(void)// pas utilisé
{
    space.Pause();
    space.clear();
    space.menuAff();
    lcd.cls();
    lcd.printf("=>Jouer     <=");
    lcd.locate(0,1);
    lcd.printf("Difficulte");
    giH = giS = giM = giD = 0;
    printf("Action = A39\n\r");
    printf("Etat = LCDPLAY\n\r");
    return LCDPLAY;
}
////////////////////////////////////////////////////////////////
//Retour partie si Pause//
////////////////////////////////////////////////////////////////
int A40(void)
{
    T_MS1=100;
    T_MS2=150;
    scanShip.attach(&shipscan,0.2);
    space.Play();
    son.MusicSelect(SelectionMusic);
    Chrono.reset();
    Chrono.start();
    printf("Action = A40\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
////////////////////////////////////////////////////////////////
//temps durant la partie//
////////////////////////////////////////////////////////////////
int A41(void)//Chrono
{
    Affiche();
    printf("Action = A41\n\r");
    printf("Etat = LVL\n\r");
    return LVL;
}
////////////////////////////////////////////////////////////////
//LCD Score//
////////////////////////////////////////////////////////////////
int A42(void)
{
    lcd.cls();
    lcd.printf("  Jouer =>Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    printf("Action = A42\n\r");
    printf("Etat = LCDSCORE\n\r");
    return LCDSCORE;
}
int A43(void)
{
    ScIndice++;
    Scrolling.start();
    scl=0;
    switch (ScIndice) {
        case 0:
            LCD_FirstScroll(ctScore0);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore1);
            break;
        case 1:
            LCD_FirstScroll(ctScore1);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore2);
            break;
        case 2:
            LCD_FirstScroll(ctScore2);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore3);
            break;
        case 3:
            LCD_FirstScroll(ctScore3);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore4);
            break;
        case 4:
            LCD_FirstScroll(ctScore4);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore5);
            break;
        case 5:
            LCD_FirstScroll(ctScore5);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore6);
            break;
        case 6:
            LCD_FirstScroll(ctScore6);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore7);
            break;
        case 7:
            LCD_FirstScroll(ctScore7);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore8);
            break;
        case 8:
            LCD_FirstScroll(ctScore8);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore9);
            break;
        case 9:
            LCD_FirstScroll(ctScore9);
            break;
    }
    printf("Action = A43\n\r");
    printf("Etat = SCORE\n\r");
    return SCORE;
}
int A44(void)
{
    switch (ScIndice) {
        case 0:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore1);
            break;
        case 1:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore2);
            break;
        case 2:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore3);
            break;
        case 3:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore4);
            break;
        case 4:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore5);
            break;
        case 5:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore6);
            break;
        case 6:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore7);
            break;
        case 7:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore8);
            break;
        case 8:
            LCD_NextScroll();
            lcd.locate(0,1);
            lcd.printf("%s",ctScore9);
            break;
        case 9:
            LCD_NextScroll();
            break;
    }
    printf("Action = A44\n\r");
    printf("Etat = SCORE\n\r");
    return SCORE;
}
int A45(void)
{
    Scrolling.stop();
    Scrolling.reset();
    ScIndice=-1;
    lcd.cls();
    lcd.printf("=>Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    printf("Action = A45\n\r");
    printf("Etat = LCDPLAY\n\r");
    return LCDPLAY;
}
int A46(void)
{
    ScIndice--;
    Scrolling.start();
    scl=0;
    switch (ScIndice) {
        case 0:
            LCD_FirstScroll(ctScore0);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore1);
            break;
        case 1:
            LCD_FirstScroll(ctScore1);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore2);
            break;
        case 2:
            LCD_FirstScroll(ctScore2);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore3);
            break;
        case 3:
            LCD_FirstScroll(ctScore3);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore4);
            break;
        case 4:
            LCD_FirstScroll(ctScore4);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore5);
            break;
        case 5:
            LCD_FirstScroll(ctScore5);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore6);
            break;
        case 6:
            LCD_FirstScroll(ctScore6);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore7);
            break;
        case 7:
            LCD_FirstScroll(ctScore7);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore8);
            break;
        case 8:
            LCD_FirstScroll(ctScore8);
            lcd.locate(0,1);
            lcd.printf("%s",ctScore9);
            break;
        case 9:
            LCD_FirstScroll(ctScore9);
            break;
    }
    printf("Action = A43\n\r");
    printf("Etat = SCORE\n\r");
    return SCORE;
}
////////////////////////////////////////////////////////////////
//saisie du pseudo à la fin de la game si "Gagné"//
////////////////////////////////////////////////////////////////
int A47(void)
{
    Clettre[IndicePseudo]++;
    if(Lettres[Clettre[IndicePseudo]]==28)
        Clettre[IndicePseudo]=1;
    pseudo[IndicePseudo]=Lettres[Clettre[IndicePseudo]];
    sprintf(PSO,"pseudo: %c%c%c%c%c%c%c%c%c%c",pseudo[0],pseudo[1],pseudo[2],pseudo[3],pseudo[4],pseudo[5],pseudo[6],pseudo[7],pseudo[8],pseudo[9]);
    lcd.cls();
    lcd.printf("%s",PSO);
    lcd.locate((8+IndicePseudo),1);
    lcd.printf("-");
    printf("Action = A47\n\r");
    printf("Etat = PSEUDO\n\r");
    return PSEUDO;
}
int A48(void)
{
    Clettre[IndicePseudo]--;
    if(Lettres[Clettre[IndicePseudo]]==0)
        Clettre[IndicePseudo]=27;
    pseudo[IndicePseudo]=Lettres[Clettre[IndicePseudo]];
    sprintf(PSO,"pseudo: %c%c%c%c%c%c%c%c%c%c",pseudo[0],pseudo[1],pseudo[2],pseudo[3],pseudo[4],pseudo[5],pseudo[6],pseudo[7],pseudo[8],pseudo[9]);
    lcd.cls();
    lcd.printf("%s",PSO);
    lcd.locate((8+IndicePseudo),1);
    lcd.printf("-");
    printf("Action = A48\n\r");
    printf("Etat = PSEUDO\n\r");
    return PSEUDO;
}
int A49(void)
{
    if(IndicePseudo<10)
        IndicePseudo++;
    lcd.cls();
    lcd.printf("%s",PSO);
    lcd.locate((8+IndicePseudo),1);
    lcd.printf("-");
    printf("Action = A49\n\r");
    printf("Etat = PSEUDO\n\r");
    return PSEUDO;
}
int A50(void)
{
    if(IndicePseudo>=0)
        IndicePseudo--;
    lcd.cls();
    lcd.printf("%s",PSO);
    lcd.locate((8+IndicePseudo),1);
    lcd.printf("-");
    printf("Action = A50\n\r");
    printf("Etat = PSEUDO\n\r");
    return PSEUDO;
}
//enregistremement du score
int A51(void)
{
    T_MS1=500;
    T_MS2=500;
    strcpy(ctScore9,ctScore8); // on copie la ligne 8 sur la ligne 9
    strcpy(ctScore8,ctScore7); // on copie la ligne 7 sur la ligne 8
    strcpy(ctScore7,ctScore6); // on copie la ligne 6 sur la ligne 7
    strcpy(ctScore6,ctScore5); // on copie la ligne 5 sur la ligne 6
    strcpy(ctScore5,ctScore4); // on copie la ligne 4 sur la ligne 5
    strcpy(ctScore4,ctScore3); // on copie la ligne 3 sur la ligne 4
    strcpy(ctScore3,ctScore2); // on copie la ligne 2 sur la ligne 3
    strcpy(ctScore2,ctScore1); // on copie la ligne 1 sur la ligne 2
    strcpy(ctScore1,ctScore0); // on copie la ligne 0 sur la ligne 1
    snprintf(PSO,10,"%c%c%c%c%c%c%c%c%c%c",pseudo[0],pseudo[1],pseudo[2],pseudo[3],pseudo[4],pseudo[5],pseudo[6],pseudo[7],pseudo[8],pseudo[9]);
    snprintf(PSF,40,"%s Score:%d Temps:%02d:%02d:%02d",PSO,space.Score(),giM,giS,giD);
    int i;
    for(i=0; i<10; i++) {
        pseudo[i]=NULL;
    }
    strcpy(ctScore0,PSF); // on copie PSF dans la ligne 0
    lcd.cls();
    lcd.printf("=>Jouer   Score");
    lcd.locate(0,1);
    lcd.printf("  Difficulte");
    giH = giS = giM = giD = 0;
    printf("Action = A51\n\r");
    printf("Etat = LCDPLAY\n\r");
    return LCDPLAY;
}
////////////////////////////////////////////////////////////////
//affichage du score pedant la partie + scan des vaisseaux//
//pour vérifier si un vaisseau à était touché//
////////////////////////////////////////////////////////////////
//Ticker
void shipscan (void)
{
    space.ShipDestroyScan();
}
//affiche sur le LCD le temps, le score et la vie pendant la partie
void Affiche(void)
{
    Gestion();
    lcd.cls();
    lcd.printf("Score:%d HP:%d",space.Score(),space.HP());
    lcd.locate(0,1);
    lcd.printf("%02d:%02d:%02d:%02d",giH,giM,giS,giD);
}
////////////////////////////////////////////////////////////////
//Gestion du temps//
////////////////////////////////////////////////////////////////
void Gestion(void)
{
    giD++;
    if(giD>=10) {
        giD = 0;
        giS++;
        if(giS>=60) {
            giS = 0;
            giM++;
            if(giM>=60) {
                giM = 0;
                giH++;
                if(giH>=99) {
                    giH = 0;
                }
            }
        }
    }
}
////////////////////////////////////////////////////////////////
//défilement text du LCD//
////////////////////////////////////////////////////////////////
void LCD_FirstScroll(char *Message)
{
    //copie du message courant
    strcpy(NewMessage,Message);
    //on rajoute 5 espace au message initial
    strcat(NewMessage,"    ");
    //calcul de la longueur du tableau
    gilen=strlen(NewMessage);
    //on affiche à nouveau le message*/
    lcd.cls();
    lcd.printf("%s",NewMessage);
}
void LCD_NextScroll(void)
{
    int i;
    char cLost;
    //construction d'un message tournant
    //On memorise d'abord le 1er element du tableau
    //...que l'on va perdre
    cLost=NewMessage[0];
    //on décale tout d'un rang à gauche
    for (i=0; i<gilen-1; i++)
        NewMessage[i]=NewMessage[i+1];
    //on remet en fin du tableau le caractére perdu
    NewMessage[gilen-1]=cLost;
    //on affiche à nouveau le message
    lcd.cls();
    lcd.printf("%s",NewMessage);
}
////////////////////////////////////////////////////////////////
//Fin//
////////////////////////////////////////////////////////////////
#endif //JEU
