#include <iostream>
#include <fstream>
#include <sstream>
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
    Rotary rot2(
        "gpiochip1", 91,
        "gpiochip1", 82,
        "gpiochip1", 83
    );
    std::ofstream fifoOut;
    std::string fifoPathOut = "/tmp/pinp";
    fifoOut.open(fifoPathOut);
    std::cout << "Waiting on fifo..." << std::endl;
    if (!fifoOut.is_open()) {
        std::cerr << "Failed to open output FIFO: "+fifoPathOut << std::endl;
        return 1;
    }
    else{
        std::cout << "Opened output FIFO" << std::endl;
    }

    int lr = 0;
    int it = 0;
    int r1 = 0;
    int r2 = 0;
    while(true){
        usleep(1000);// CPU Optimization
        std::string rotRead = rot.read();
        std::string rotRead2 = rot2.read();

        // Handle blink
        if(std::abs(r1) == 100){
            bool odd = it > 100;
            led.set(odd);
        }
        if(std::abs(r2) == 100){
            bool odd = it > 100;
            led2.set(odd);
        }
        it = it <= 200 ? it+1 : 0;

        // Dont continue from here if the message is empty
        if(rotRead.size() == 0 && rotRead2.size() == 0){
            continue;
        }

        for(int i = 0; i < rotRead.size(); i++){
            char dir = rotRead[i];
            if(dir == 'r'){
                r1 += 1;
            }
            else{
                r1 += -1;
            }
        }
        // Ensure r1 is 0->100
        r1 = r1 > 100 ? 100 : r1;
        r1 = r1 < 0 ? 0 : r1;

        for(int i = 0; i < rotRead2.size(); i++){
            char dir = rotRead2[i];
            if(dir == 'r'){
                r2 += 1;
            }
            else{
                r2 += -1;
            }
        }
        // Ensure r2 is 0->100
        r2 = r2 > 100 ? 100 : r2;
        r2 = r2 < 0 ? 0 : r2;

        led.pwm(r1);
        led2.pwm(r2);

        fifoOut << "f " << (r1 * 10) << std::endl;
        fifoOut << "p0 " << (r2 / 10.f) << std::endl;
    }
    return 0;
}