#pragma once
#include "GPIO.h"
#include "../Shared/Util.h"

class Color{
public:
    Color(float R, float G, float B){
        r = Util::bound<float>(R, 0.0f, 1.0f);
        g = Util::bound<float>(G, 0.0f, 1.0f);
        b = Util::bound<float>(B, 0.0f, 1.0f);
    }
    float getR(){
        return r;
    }
    float getG(){
        return g;
    }
    float getB(){
        return b;
    }
private:
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

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
    void set(float r, float g, float b){
        gpioR.pwm((int)(r * 100));
        gpioG.pwm((int)(g * 100));
        gpioB.pwm((int)(b * 100));
    }
    void set(Color color){
        gpioR.pwm((int)(color.getR() * 100));
        gpioG.pwm((int)(color.getG() * 100));
        gpioB.pwm((int)(color.getB() * 100));
    }
private:
    GPIO gpioR;
    GPIO gpioG;
    GPIO gpioB;
};