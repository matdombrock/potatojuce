// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
// First do
// $ mkfio /tmp/led_fifo

#include "GPIO.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

class IPCWatcher{
public:
    IPCWatcher(std::string newFifoPath="/tmp/pgpio"){
        fifoPath = newFifoPath;
        std::cout << "Starting IPC Watcher @ " + newFifoPath << std::endl;
    }

    void addDevice(GPIO * gpio, bool writeMode){
        if(writeMode){
            std::cout << "Adding write pin: " << pinsW.size() + 1 << std::endl;
            std::cout << gpio->getChipName() << " " << gpio->getLineNum() << std::endl;  
            pinsW.push_back(gpio);
        }
        else{
            std::cout << "Adding read pin: " << pinsR.size() + 1 << std::endl;
            std::cout << gpio->getChipName() << " " << gpio->getLineNum() << std::endl;  
            pinsR.push_back(gpio);
        }
    }

    // Statrt the main watcher loop
    int watch(){
        std::cout << "Starting watcher..." << std::endl;
        openGPIO();
        openFIFO();
        std::cout << "Listening for messages..." << std::endl;
        bool run = true;
        while(run){
            processInput();
            processOutput();
        }
        closeFIFO();
        closeGPIO();
        return 0;
    }

private:
    // Handles reading FIFO input
    // and writing data to pins
    void processInput(){
        // Read loop
        std::string line;
        while (getline_async(fifoIn, line)) {
            std::cout << "Received message: " << line << std::endl;

            // See if we have a command
            std::vector<std::string> split = splitLine(line);
            if(split.size() >= 2){
                // We have a command
                std::string cmd = split[0];
                int pinIndex = std::stoi(split[1]);
                int pinVal = std::stoi(split[2]);
                if(cmd == "add"){
                    //addDevice(&);
                    continue;
                }
                if(cmd == "set"){
                    set(pinIndex, (bool)pinVal);
                    continue;
                }
                if(cmd == "pwm"){
                    if(validIndex(pinIndex)){
                        pinsW[pinIndex]->pwm(pinVal);
                    }
                    continue;
                }
                else{
                    std::cout << "Error: unknown cmd: " << cmd << std::endl;
                    continue;
                }
            }
            // We have serial input
            if(line.size()>pinsW.size()){
                std::cerr << "Input too long" << std::endl;
                continue;
            }
            if(line.size()<1){
                std::cerr << "Input too short" << std::endl;
                continue;
            }
            // Process serial input
            for(int i = 0; i < line.size(); i++){
                //int valInt = std::stoi(c);
                char ch = line[i];
                // Check if we have an ignore
                if(ch == '-'){
                    continue;
                }
                int valInt = int(ch) - 48;// ctoi
                // Check if we have a bool
                if(valInt == 0 || valInt == 1){
                    set(i, (bool)valInt);
                }
                else{
                    std::cout << "Errror: At index: " << i << ". All pins must be set to a bit value (bool)!" << std::endl;
                    continue;
                }
            }
        }
    } 

    // Handles FIFO output
    // and reading data from pins
    void processOutput(){
        std::string readOut;
        bool valChanged = false;
        for (int i = 0; i < pinsR.size(); i++) {
            int pinStateCache = pinsR[i]->getState();
            int pinVal = pinsR[i]->get();// Not a bool!
            std::string pinString = std::to_string(pinVal);
            readOut+=pinString;
            if(pinStateCache != pinVal){
                // We have a new value
                valChanged = true;
            }
        }
        // Only write when we have new values
        if(valChanged){
            fifoOut << readOut << std::endl; // write the string to the file
            std::cout << readOut << std::endl;
        }
    }

    void openFIFO(){
        std::string fifoPathIn = fifoPath + "-in";
        std::string fifoPathOut = fifoPath + "-out";

        std::cout << "Setting up IPC FIFO" << std::endl;
        std::cout << "In path: " << fifoPathIn << std::endl;
        std::cout << "Out path: " << fifoPathOut << std::endl;

        std::cout << "Waiting on input FIFO..." << std::endl;
        fifoIn.open(fifoPathIn);
        if (!fifoIn.is_open()) {
            std::cerr << "Failed to open input FIFO: "+fifoPathIn << std::endl;
            return;
        }
        else{
            std::cout << "Opened Input FIFO" << std::endl;
        }
        
        std::cout << "Waiting on output FIFO..." << std::endl;
        fifoOut.open(fifoPathOut, std::ofstream::out);
        if (!fifoOut.is_open()) { // check if the file is open
            std::cerr << "Failed to open output File: "+fifoPathOut << std::endl;
            return;
        }
        else{
            std::cout << "Opened Output FIFO" << std::endl;
        }
    }
    // Opens the GPIO pins for read/write
    void openGPIO(){
        for (int i = 0; i < pinsW.size(); i++) {
            std::cout <<"Opening GPIO Device: " << i << std::endl;
            pinsW[i]->open();
        }
        for (int i = 0; i < pinsR.size(); i++) {
            std::cout <<"Opening GPIO Device: " << i << std::endl;
            pinsR[i]->open();
        }
        std::cout <<"Opened all GPIO Devices" << std::endl;
    }
    // Closes FIFO pipes
    void closeFIFO(){
        std::cout << "Closing FIFO Pipes" << std::endl;
        fifoIn.close();
        fifoOut.close();
    }

    // Floses the GPIO pins
    void closeGPIO(){
        for (int i = 0; i < pinsW.size(); i++) {
            std::cout <<"Releasing GPIO Device: " << i << std::endl;
            pinsW[i]->release();
        }
        for (int i = 0; i < pinsR.size(); i++) {
            std::cout <<"Releasing GPIO Device: " << i << std::endl;
            pinsR[i]->release();
        }
    }

    //
    // Helper methods
    //

    // Wraps GPIO Set
    void set(int pinIndex, bool val){
        if(validIndex(pinIndex)){
            pinsW[pinIndex]->set(val);
        }
    }

    bool validIndex(int pinIndex){
        if(pinsW.size() >= pinIndex){
            return true;
        }
        std::cout << "Error: unknown write pin: " << pinIndex << std::endl;
        return false;
    }

    // Reads line if available or continues instantly if not
    // Source: https://stackoverflow.com/a/57809972
    static bool getline_async(std::istream& is, std::string& str, char delim = '\n') {    
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
    
    // Split a line by space
    static std::vector<std::string> splitLine(std::string str) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, ' ')) {
            tokens.push_back(token);
        }

        for (int i = 0; i < tokens.size(); i++) {
            std::cout << tokens[i] << std::endl;
        }

        return tokens;
    }
    std::string fifoPath;
    std::ifstream fifoIn;
    std::ofstream fifoOut;
    std::vector<GPIO*> pinsR = {};
    std::vector<GPIO*> pinsW = {};
};


int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  IPCWatcher watcher("/tmp/pgpio");

  GPIO c1l91("gpiochip1", 91);
  GPIO c1l93("gpiochip1", 93);
  GPIO c1l94("gpiochip1", 94);
  GPIO c1l98("gpiochip1", 98);
  
  // chip name, line number, writeMode
  watcher.addDevice(&c1l91, true);
  watcher.addDevice(&c1l98, true);
  watcher.addDevice(&c1l93, false);
  watcher.addDevice(&c1l94, false);

  watcher.watch();
  return 0;
}

