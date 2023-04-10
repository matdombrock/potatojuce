#include <iostream>
#include <stdio.h>
#include "LED.h"
int main(int argc, char **argv)
{
    std::cout << "Starting TEST" << std::endl;
    std::cout << "===========================" << std::endl;
    LED led("gpiochip1", 91);
    LED led2("gpiochip1", 98);
    for(int i = 0; i < 99; i++){
        bool state = i % 2;
        led.set(state);
        led.set(!state);
        std::cout << state << std::endl;
        std::sleep(500);
    }
    return 0;
}