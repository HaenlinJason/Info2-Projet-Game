///////////////////////////////////////////////
//      library
//      Prototypes et Class
//      date de création    : 22/06/2016
//      date de mise à jour : 22/06/2016
//      détails             :
// sound.period(1/(frequence[i])); //set PWM period
// sound.write(0.5); //set duty cycle
///////////////////////////////////////////////
#ifndef MBED_MUSIQUE_H
#define MBED_MUSIQUE_H
 
#include "mbed.h"
#include "MusicConst.h"

class Music{
public:
    //déclaration des pins
    Music(PinName HP1,PinName HP2);
    //prototypes
    void initprgm(void);
    void MusicSelect(int son);
    void MusicOff(void);
    void Win(void);
    void Loose(void);
private:
    //prototypes
    void Son (void);
    //évenement temporel
    Ticker sound;
    //variables
    PwmOut _HP1;
    PwmOut _HP2;
};
////////////////////////////////////////////////////////////////
//Fin//
////////////////////////////////////////////////////////////////
#endif
        