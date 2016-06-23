///////////////////////////////////////////////
//      library
//      Prototypes et Class
//      date de création    : 20/05/2016
//      date de mise à jour : 05/06/2016
//      détails             :
//
///////////////////////////////////////////////
#ifndef GAME_H //vérifie pour affecter qu'une fois la library en
#define GAME_H //cas de doublon de déclaration
////////////////////////////////////////////////////////////////
//Debut//
////////////////////////////////////////////////////////////////

#include "mbed.h"
#include "GameConst.h"

//class
class Game
{

//public: accée à l'éxtérieur de la class (utilisable par le main)
public:
    Game();   // constructeur

    //PinName(s)
    Game(PinName leD,PinName leC, PinName leB, PinName leA, PinName leG, PinName DI, PinName DO, PinName CLOCK, PinName cs);

    //structure(s)
    typedef struct {
        unsigned int tab[TAILLE];
        int HP;
        int y;
    } vaisseau;

    typedef struct {
        int nbr;
        int HP;
    } lvldesign;
    //déclaration des structures
    lvldesign lvl;
//-------------------------------------------------------------------------//
//vaisseau : V01
//Enemy    : A01,A02...A100
    vaisseau V01,A01,A02,A03,A04,A05,A06,A07,A08,A09,A10,A99,A100;
//-------------------------------------------------------------------------//
//tir vaisseau TV1
//tir Enemy    TA1
    vaisseau TV1,TA1;

//prototype(s)
//-------------------------------------------------------------------------//
//fin du lvl quand tous les ennemis sont détruits
    int endlvl(void);   //gagné
//-------------------------------------------------------------------------//
//fin du lvl quand le vaisseau est détruit
    int dead(void);     //perdu
//-------------------------------------------------------------------------//
//tir du vaisseau
    void TirVaisseau(void);
//-------------------------------------------------------------------------//
//Déplacement des vaisseaux 
//move => distance du déplacement
//sens => sens du déplacement  (BAS, HAUT, DROITE, GAUCHE)
    void Deplacement(int move,int sens);
//-------------------------------------------------------------------------//
//Point de vie du vaisseau 
//renvoi au programme les points de vie restant
    int HP(void);
//-------------------------------------------------------------------------//
//Score du joueur
//Ennemis détruit => +10 points
//-10 Points par point de vie perdu
    int Score(void);
//-------------------------------------------------------------------------//
//niveau d'essaie
    void testLVL(void);
//-------------------------------------------------------------------------//
//vérifie vaisseau par vaisseau si il y a contact
    void ShipDestroyScan(void);
//-------------------------------------------------------------------------//
//La difficulté du jeu (FAIBLE, MOYEN, FORT)
    void Difficulty(int mode);
//-------------------------------------------------------------------------//
//met le jeu en pause 
//detach tous les tickers
    void Pause(void);
//-------------------------------------------------------------------------//
//relance le jeu
//rattach tous les tickers
    void Play(void);
//-------------------------------------------------------------------------//
//reset la matrice à Led et set le les premiérs Ennemis
    void reset(void);
//-------------------------------------------------------------------------//
//clear la matrice à Leds
    void clear(void);
//-------------------------------------------------------------------------//
//copie la forme du vaisseau 
//vs => Al1 : 1er Ennemis { --0 }
//                        { -00 }
//                        { --0 }  
//----------------------------------//
//vs => Al2 : 2er Ennemis { -00 }
//                        { 0-0 }
//                        { -00 }
//----------------------------------//  
//vs => Al3 : 3er Ennemis { 0-0 }
//                        { -00 }
//                        { 0-0 }  
//----------------------------------// 
//vs => BOSS
//----------------------------------// 
//*M0=> demande l'adress du vaisseau (&) qui va récuperer la forme
    void set(int vs,vaisseau *M0);
//-------------------------------------------------------------------------//
//Affiche sur la matrice l'image du menu
    void menuAff(void);
//-------------------------------------------------------------------------//
//niveau 1
    void lvl_1(void);
//-------------------------------------------------------------------------//
//niveau 2
    void lvl_2(void);
//-------------------------------------------------------------------------//
//niveau 3
    void lvl_3(void);
//-------------------------------------------------------------------------//
//niveau 4
    void lvl_4(void);
//-------------------------------------------------------------------------//
//niveau 5
    void lvl_5(void);
//-------------------------------------------------------------------------//
//niveau 6
    void lvl_6(void);
//-------------------------------------------------------------------------//
//niveau 7
    void lvl_7(void);
//-------------------------------------------------------------------------//
//niveau 8
    void lvl_8(void);
//-------------------------------------------------------------------------//
//niveau 9
    void lvl_9(void);
//-------------------------------------------------------------------------//
//niveau 10
    void lvl_10(void);
//-------------------------------------------------------------------------//


//protected: seulement à l'intérieur de la class 
//(le main ne peux pas l'appeler)
protected:
//prototype(s)
//-------------------------------------------------------------------------//
//Affichage au début de chaque niveau du n° du niveau
    void InitTabAff(void);
//-------------------------------------------------------------------------//
//Vérifie si le mouvement demandé est possible
//Impossible si le vaisseau est aux extrémités de la matrice
//vaisseau* => l'adresse du vaisseau qui ce déplace
//sens      => direction du déplacement
    int MovePossibility(vaisseau*,int sens);
//-------------------------------------------------------------------------//
//tir effectue par un ennemis
    void tirEnemy(vaisseau M00);
//-------------------------------------------------------------------------//
//mouvement horizontal : DROITE ou GAUCHE
//move => distance du déplacement
    void tabmovehorizontal(vaisseau*,int move,int sens);
//-------------------------------------------------------------------------//
//mouvement vertical : HAUT ou BAS
//move => distance du déplacement
    void tabmovevertical(vaisseau*,int move, int sens);
//-------------------------------------------------------------------------//
//mouvement des vaisseaux ennemis pendant la partie
//(DROITE, GAUCHE, BAS, HAUT)
//move => distance du déplacement
    void IngameMove(vaisseau*,int move,int sens);
//-------------------------------------------------------------------------//
//initialiser le programme
    void initprgm(void);
//-------------------------------------------------------------------------//
//Assemble tous les tableaux et envoi ce qui dois être affiché
    void Display(void);
//-------------------------------------------------------------------------//
//Scan les lignes une par une avec décodeur
    void Scan_Line(int m);
//-------------------------------------------------------------------------//
//supprime le vaisseau en le faisant clignoter
    void suppShip1(void);
//-------------------------------------------------------------------------//
//supprime le vaisseau en le faisant clignoter
    void suppShip2(void);
//-------------------------------------------------------------------------//
//Ticker des mouvements des tirs du vaisseau
    void tirmoveV(void);
//-------------------------------------------------------------------------//
//Ticker des mouvements des tirs ennemis
    void tirmoveA(void);
//-------------------------------------------------------------------------//
//destruction du vaisseau
//1er  argument : vaisseau ennemis ou allié
//2eme argument : tir allié ou ennemis
    void shipdestroy(vaisseau*,vaisseau*);
//-------------------------------------------------------------------------//
//tir aléatoire des ennemis pendant la partie 
//utilise la fonction rand (randum)
    void TtirAlien(void);
//-------------------------------------------------------------------------//
//mouvement des ennemis automatique pendant la partie
    void LVlTestMove(void);
//-------------------------------------------------------------------------//
//affiche le n° du niveau au début de chaque niveau
    void Affiche(int chiffre);
//-------------------------------------------------------------------------//

    //Ticker(s)
    Ticker _Move;
    Ticker _tirAlien;
    Ticker _balayage;
    Ticker _tirV;
    Ticker _tirA;
    Ticker _supp1;
    Ticker _supp2;
    //structure(s)

    //déclaration des variable(s)
    float _Lvlstrong;
    int _HP;
    unsigned int _score;
    unsigned int tabMenu[TAILLE];
    unsigned int tabUn[TAILLE];
    unsigned int tabDeux[TAILLE];
    unsigned int tabTrois[TAILLE];
    unsigned int tabQuatre[TAILLE];
    unsigned int tabCinq[TAILLE];
    unsigned int tabSix[TAILLE];
    unsigned int tabSept[TAILLE];
    unsigned int tabHuit[TAILLE];
    unsigned int tabNeuf[TAILLE];
    unsigned int tabDix[TAILLE];
    //déclaration des structures

//déclaration des ports de communications
    //décodeur
    DigitalOut _leD;
    DigitalOut _leC;
    DigitalOut _leB;
    DigitalOut _leA;
    DigitalOut _leG;
    //mosi miso clock
    SPI _spi;
    //chip select
    DigitalOut _cs;
    //les broches sont déclarées dans le main par l'utilisateur
};
////////////////////////////////////////////////////////////////
//Fin//
////////////////////////////////////////////////////////////////
#endif //GAME_H

