///////////////////////////////////////////////
//      fonction de la library
//      Fichier fonctions avec utilisation de class
//      date de création    : 22/06/2016
//      date de mise à jour : 23/06/2016
//      détails             :
///fonction du projet INFO2 musique de jeu
///////////////////////////////////////////////
#include "mbed.h"
#include "Music.h"
#include "MusicConst.h"
////////////////////////////////////////////////////////////////
//Debut//
////////////////////////////////////////////////////////////////
/*Choix du Haut Parleur*/

int duree;

int choix; // choix de la musique (1,2,3,4)

float frequence1[]= {Dod4,Mi4,Mi4,Mi4,Mi4,Mi4,Mi4,Do4,Sil,Do4,Do4,Mi4,Mi4,Mi4,Mi4,Dod4,Sil,Sold4,Fad4,Sol4,Mi4,
                     Mi4,Mi4,Mi4,Mi4,Mi4,Dod4,Sil,Sold4,Sil,Fad4,Sil,Mi4,Fa4,Sil,Fad4,Fa4,Fa4,Fa4,Fa4,Fa4,Red4,
                     Sil,Sold4,Sil,Fad4,Sil,Mi4,Sil,Red4,Sil,Dod4,Sil,Dod4,Sold4,Si4,Lad4,Lad4,Do4,Sil,Dod4,
                     Sold4,Si4,Lad5,Red5,Mi4,Dod4,Mi4,Mi4,Mi4,Mi4,Mi4,Mi4,Do4,Sil,Do4,Do4,Mi4,Mi4,Mi4,Mi4,Dod4,
                     Sil,Sold4,Fad4,Sol4,Mi4,Mi4,Mi4,Mi4,Mi4,Mi4,Dod4,Sil,Sold4,Sil,Fad4,Sil,Mi4,Fa4,Sil,Fad4,
                     Fa4,Fa4,Fa4,Fa4,Fa4,Red4,Sil,Sold4,Sil,Fad4,Sil,Mi4,Sil,Red4,Sil,Dod4,Sil,Dod4,Sold4,Si4,
                     Lad4,Lad4,Do4,Sil,Dod4,Sold4,Si4,Lad4,Red5,Mi5
                    };//132//middle

float frequence2[]= {Sol4,Re4,Re4,Re4,Re4,Sol4,Re4,Sol4,Si4,Si4,Si4,Si4,Si4,Si4,Si4,Si4,La4,Fa4,Fa4,La4,
                     Do4,Do4,Do4,Do4,Do4,Do4,Do4,Do5,Re5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Re5,Re5,Re5,Re5,Re5,Re5,Re5,Re5,Re5,Re5,
                     Sol4,Sol4,Sol4,Sol4,Sol4,Sol4,Sol4,Sol4,Sol4,La4,La4,Si4,Do4,Do4,Do4,Do4,Do4,Do4,Do4,Do4,Do4,
                     Fa4,Fa4,Sol4,La4,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Do5,Do5,Si4,La4,Sol4,Mi4,Mi4,Mi4,Mi4,Mi4,Mi4,
                     Mi4,Mi4,Sol4,Sol4,La4,Si4,Si4,Si4,Si4,Si4,Si4,Si4,Si4,Mi4,Mi4,Fa4,Sol4,Sol4,Sil,Sil,Sil,Sil,Sil,Sil,Si4,La4,
                     La4,Sol4,Fa4,Re5,Re5,Re5,Re5,Re5,Re5,Re5,Re5,La4,La4,Si4,Do5,Do5,Do5,Do5,Do5,Do5,Do5,Do5,Fa4,Fa4,Sol4,
                     La4,La4,La4,La4,Do5,Do5,Do5,Re5,Mi5,Do5,Do5,Do5,Do5,Do5,Do5,Mi5,Re5,Re5,Do5,Re5,Re5,Re5,Re5,Re5,Re5,Mi5,Fa5,
                     Fa5,Mi5,Re5,Re5,Fa5,Fa5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Mi5,Fad5,Fad5,Sol5,Fad5,
                     Re5,Re5,Re5,Re5,Re5,Re5,Re5,Re5,Sol4,Re4,Re4,Sol4,Sol4,La4,Si4,Do4,Re5,Sil

                    };//398//first

float frequence3[]= {Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Mi2,Mi2,Si2,Si2,Sol2,Sol2,Sol2,Sol2,Mi2,Mi2,Si2,
                     Si2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Re2,Re2,Re2,Re2,Re2,Re2,Re2,Re2,Re2,Re2,Re2,Re2,Mi2,Mi2,
                     Si2,Si2,Fad2,Fad2,Fad2,Fad2,Mi2,Mi2,Si2,Si2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2,Sol2
                    };//66 //boss

float frequence4[]= {Fa6,Fa6,Mi6,Mi6,Sil,Sil,Sil,Sil,Mi6,Fa6,Fa6,Sil,Sil,Fa6,Sil,Sil,Fa6,Mi6,Fa4,Sil,Sil,Fa4,Mi4,Sil,Sil,Sil,Sil,
                     Mi4,Sil,Sil,Mi4,Fa4,Sil,Sil,Sil,Sil,Fa4,Sil,Sil,Fa4,Mi4,Mi6,Sil,Sil,Sil,Sil,Mi6,Sil,Sil,Mi6,Fa6,Fa6,Fa5,La5,
                     Si5,Re6,Do6,La5,Si5,Fa6,Sil,Sil,Fa6,Mi6,Mi6,Sil,Sil,Sil,Sil,Sol4,Sil,Sil,Fa4,Sol4,Sol4,Sol4,Re4,Fa4,Si4,La4,Fa4,
                     Sol4,Sol4,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Sil,Mi6,Sil,Sil,Mi6,Fa6,Fa6,Si4,Re5,Mi5,Mi5,Si4,Fad5,Fad5,Fa6,Sil,Sil,
                     Mi6,Mi6,Sil,Mi6,Mi6,Mi6,Mi6,Mi6,Mi6,Fa4,Sil,Sil,Fa4,Sol4,Sol4,Red4,Fa4,Sol4,Sol4,Re4,Si4,La4,Sol4,Sil,Sil,Sol4,
                     Fa4,Sil,Sil,Sil,Sil,Sil,Mi6,Mi6,Mi6,Mi6,Sil,Mi6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,
                     Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Fa6,Si4,Mi6,Mi6,Si4,Do6,Re6,Si4,Sil,Sil,Sil,Sil,Si4,Do6,Si5,Si4,Do6,Fa4,
                     Sil,Sil,Fa4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Re4,Sol5,Re6,Sil,Sil,
                     Sil,Sil,Sol5,Re6,Sol5,Re6,Sil,Sil,Sil,Sil,Sol5,Re6,Sol5,Re6,Sil,Sil,Sil,Sil,Re6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,Do6,
                     Do6,Re6,Do6,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5,La5
                    };//267//middle+moyen
float frequence6[]= {1174,0,1174,0,1174,0,1174,0,1174,932,1046,1174,1046,1174}; //frequency array
float beat6[]= {1,0.1,0.5,0.1,0.5,0.1,1,0.1,1,1,1,0.5,0.5,3}; //beat array//Variables Globale(s)

float frequence7[]= {329,440,493,440,415,466,415,392,293,392}; //frequency array
float beat7[]= {2,2,2,2,2,2,2,4,3,6}; //beat array//Variables Globale(s)

Music::Music(PinName HP1,PinName HP2):
    _HP1(HP1),_HP2(HP2)
{
    initprgm();
}
void Music::initprgm(void)
{
    MusicOff();
    duree=0;
    choix=0;
}
void Music::MusicSelect(int note)
{
    choix=note;
    sound.attach(this,&Music::Son,0.25);
}
void Music::Win(void)
{
    int l;
    for(l=0; l<14; l++) {
        _HP1.period(1/(frequence6[l])); //set PWM period
        _HP1.write(0.5); //set duty cycle
        _HP2.period(1/(frequence6[l])); //set PWM period
        _HP2.write(0.5); //set duty cycle
        wait(0.3*beat6[l]); //hold for beat second
    }
    MusicOff();
}
void Music::Loose(void)
{
    int l;
    for(l=0; l<10; l++) {
        _HP1.period(1/(frequence7[l])); //set PWM period
        _HP1.write(0.5); //set duty cycle
        _HP2.period(1/(frequence7[l])); //set PWM period
        _HP2.write(0.5); //set duty cycle
        wait(0.3*beat7[l]); //hold for beat second
    }
    MusicOff();
}
void Music::Son(void)
{
    duree++;
    switch(choix) {
        case 1:
            if(duree>131)
                duree=0;
            _HP1.period(1/(frequence1[duree])); //set PWM period
            _HP1.write(0.5); //set duty cycle
            _HP2.period(1/(frequence1[duree])); //set PWM period
            _HP2.write(0.5); //set duty cycle
            break;
        case 2:
            if(duree>397)
                duree=0;
            _HP1.period(1/(frequence2[duree])); //set PWM period
            _HP1.write(0.5); //set duty cycle
            _HP2.period(1/(frequence2[duree])); //set PWM period
            _HP2.write(0.5); //set duty cycle
            break;
        case 3:
            if(duree>65)
                duree=0;
            _HP1.period(1/(frequence3[duree])); //set PWM period
            _HP1.write(0.5); //set duty cycle
            _HP2.period(1/(frequence3[duree])); //set PWM period
            _HP2.write(0.5); //set duty cycle
            break;
        case 4:
            if(duree>266)
                duree=0;
            _HP1.period(1/(frequence4[duree])); //set PWM period
            _HP1.write(0.5); //set duty cycle
            _HP2.period(1/(frequence4[duree])); //set PWM period
            _HP2.write(0.5); //set duty cycle
            break;
    }
}
void Music::MusicOff(void)
{
    _HP1.write(0);
    _HP2.write(0);
    sound.detach();
}
////////////////////////////////////////////////////////////////
//Fin//
////////////////////////////////////////////////////////////////
