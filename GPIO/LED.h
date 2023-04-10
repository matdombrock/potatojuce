#include "GPIO.h"

class LED{
public:
    LED(const char* newChipName="gpiochip1", int newLineNum=98) 
    : gpio(newChipName, newLineNum)
    {}
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
    void blink(int seconds){
        // Multiply by 100 to get uSeconds. 
        gpio.pwm(seconds * 100);
    }
    void pwm(int uSeconds){
        gpio.pwm(uSeconds);
    }
    void pwmStop(){
        gpio.pwm(0);
    }
private:
    GPIO gpio;
};