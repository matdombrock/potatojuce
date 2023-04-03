// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "LED_Driver.h"

// Source: https://stackoverflow.com/a/57809972
bool getline_async(std::istream& is, std::string& str, char delim = '\n') {    
    static std::string lineSoFar;
    char inChar;
    int charsRead = 0;
    bool lineRead = false;
    str = "";

    do {
        charsRead = is.readsome(&inChar, 1);
        if (charsRead == 1) {
            // if the delimiter is read then return the string so far
            if (inChar == delim) {
                str = lineSoFar;
                lineSoFar = "";
                lineRead = true;
            } else {  // otherwise add it to the string so far
                lineSoFar.append(1, inChar);
            }
        }
    } while (charsRead != 0 && !lineRead);

    return lineRead;
}

int main() {
    const char* chipName = "gpiochip1";
    LED led(chipName, 98);
    led.open();

    // $ mkfio /tmp/led_fifo
    std::string fifo_path = "/tmp/led_fifo"; // replace with your FIFO path
    std::ifstream fifo(fifo_path);

    if (!fifo.is_open()) {
        std::cerr << "Failed to open FIFO: " << fifo_path << std::endl;
        return 1;
    }

    std::string line;
    bool run = true;
    int pwm = 0;
    int i;
    while(run){
        while (getline_async(fifo, line)) {
            std::cout << "Received message: " << line << std::endl;
            int num;
            num = std::stoi(line);
            if(num < 0){
                run = false;
                std::cout << "Break " << line << std::endl;
                continue;
            }
            else if(num > 1){
                pwm = num;
                std::cout << "PWM " << line << std::endl;
                continue;
            }
            else{
                pwm = 0;
                led.set((bool)num);
            }
        }
        if(pwm){
            led.set((i & 1) != 0);
            i++;
            usleep(pwm);//microseconds
        }
    }
    
    led.release();
    fifo.close();
    return 0;
}
