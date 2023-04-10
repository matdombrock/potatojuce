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

    // for(int i = 0; i < 4; i++){
    //     bool state = i % 2;
    //     led.set(state);
    //     led2.set(!state);
    //     std::cout << state << std::endl;
    //     sleep(1);
    // }
    // for(int i = 1; i < 8; i++){
    //     led.pwm(i * 10000);
    //     std::cout << i << std::endl;
    //     sleep(1);
    // }
    // led.off();
    // for(int i = 1; i < 8; i++){
    //     led2.pwm(i * 10000);
    //     std::cout << i << std::endl;
    //     sleep(1);
    // }
    return 0;
}