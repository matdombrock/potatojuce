#pragma once
#include "GPIO.h"

class LED{
public:
    LED(const char* newChipName="gpiochip1", int newLineNum=98) 
    : gpio(newChipName, newLineNum)
    {
        gpio.open();
    }
    ~LED(){
        gpio.release();
    }
    void on(){
        gpio.set(1);
    }
    void off(){
        gpio.set(0);
    }
    void set(bool val){
        gpio.set(val);
    }
    void toggle(){
        bool cur = gpio.getState();
        gpio.set(!cur);
    }
    void pwm(int duty){
        gpio.pwm(duty);
    }
    void pwmStop(){
        gpio.pwm(100);
    }
private:
    GPIO gpio;
};