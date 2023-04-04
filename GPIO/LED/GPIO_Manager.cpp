// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
// First do
// $ mkfio /tmp/led_fifo

#include "GPIO.h"
#include <iostream>
#include <map>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

GPIO led("gpiochip1", 91);// chip name, line number

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

int startIPCWatcher(std::string newFifoPath="/tmp/pgpio-fifo"){
    led.log("Starting IPC Watcher @ " + newFifoPath);
    led.open();
    std::string fifoPath = newFifoPath;
    std::ifstream fifo(fifoPath);
    if (!fifo.is_open()) {
        std::cerr << "Failed to open FIFO: " << fifoPath << std::endl;
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
                std::cout << "PWM " << line << " microseconds" << std::endl;
                continue;
            }
            else{
                pwm = 0;
                std::cout << "Set " << line << std::endl;
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

int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  startIPCWatcher("/tmp/pgpio-fifo");
  return 0;
}
