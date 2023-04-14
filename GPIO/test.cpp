#include <iostream>
#include <stdio.h>
#include "LED.h"
#include "Rotary.h"
int main(int argc, char **argv)
{
    std::cout << "Starting TEST" << std::endl;
    std::cout << "===========================" << std::endl;
    LED led("gpiochip1", 91);
    LED led2("gpiochip1", 98);

    Rotary rot(
        "gpiochip1", 91,
        "gpiochip1", 93,
        "gpiochip1", 94
    );

    int lr = 0;
    int it = 0;
    while(true){
        usleep(1000);// CPU Optimization
        std::string rotRead = rot.read();


        // Handle blink
        if(std::abs(lr) == 100){
            bool odd = it > 100;
            it = it <= 200 ? it+1 : 0;
            if(lr >= 100){
                led2.set(odd);
            }
            else if(lr <= -100){
                led.set(odd);
            }
        }

        // Dont continue from here if the message is empty
        if(rotRead.size() == 0){
            continue;
        }
        std::cout << rotRead << std::endl;
        // Set the `lr` value.
        for(int i = 0; i < rotRead.size(); i++){
            char dir = rotRead[i];
            if(dir == 'r'){
                lr += 1;
            }
            else{
                lr += -1;
            }
        }
        // Dont allow lr to exceed abs(100)
        lr = lr > 100 ? 100 : lr;
        lr = lr < -100 ? -100 : lr;

        // Set pins according to `lr`
        if(lr > 0){
            led2.pwm(lr);
            led.off();
        }
        else{
            led.pwm(std::abs(lr));// This is safer than `-lr`
            led2.off();
        }
    }
    return 0;
}