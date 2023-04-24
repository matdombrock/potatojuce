#pragma once
#include "GPIO.h"
#include "../Shared/Util.h"

// Deprecated
// class Color{
// public:
//     Color(float R, float G, float B){
//         r = Util::bound<float>(R, 0.0f, 1.0f);
//         g = Util::bound<float>(G, 0.0f, 1.0f);
//         b = Util::bound<float>(B, 0.0f, 1.0f);
//     }
//     float getR(){
//         return r;
//     }
//     float getG(){
//         return g;
//     }
//     float getB(){
//         return b;
//     }
// private:
//     float r = 0.0f;
//     float g = 0.0f;
//     float b = 0.0f;
// };

class RGBLED{
public:
    RGBLED(
        const char* newChipNameR="gpiochip1", 
        int newLineNumR=98,
        const char* newChipNameG="gpiochip1", 
        int newLineNumG=91,
        const char* newChipNameB="gpiochip1", 
        int newLineNumB=92
    ) 
    : 
    gpioR(newChipNameR, newLineNumR),
    gpioG(newChipNameG, newLineNumG),
    gpioB(newChipNameB, newLineNumB)
    {
        gpioR.open();
        gpioG.open();
        gpioB.open();
    }
    ~RGBLED(){
        gpioR.release();
        gpioG.release();
        gpioG.release();
    }
    void on(){
        gpioR.on();
        gpioG.on();
        gpioB.on();
    }
    void set(float r, float g, float b){
        r = Util::bound<float>(r, 0.0f, 1.0f);
        g = Util::bound<float>(g, 0.0f, 1.0f);
        b = Util::bound<float>(b, 0.0f, 1.0f);
        int ri = (int)(r * 100);
        int gi = (int)(g * 100);
        int bi = (int)(b * 100);
        // Util::echo("Setting: ");
        // Util::echo(ri);
        // Util::echo(gi);
        // Util::echo(bi);
        float thresh = 0.1f;

        if(r < thresh){
            gpioR.off();
        }
        // else if(r > (1.0f - thresh)){
        //     gpioR.on();
        // }
        else{
            // Special scaling for red
            // This may differ for other LEDs
            ri *= 0.5f;
            gpioR.pwm(ri);
        }

        if(g < thresh){
            gpioG.off();
        }
        // else if(g > (1.0f - thresh)){
        //     gpioG.on();
        // }
        else{
            gpioG.pwm(gi);
        }

        if(b < thresh){
            gpioB.off();
        }
        // else if(b > (1.0f - thresh)){
        //     gpioB.on();
        // }
        else{
            gpioB.pwm(bi);
        }
    }
private:
    GPIO gpioR;
    GPIO gpioG;
    GPIO gpioB;
};