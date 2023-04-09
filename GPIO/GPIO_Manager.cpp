// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
// First do
// $ mkfio /tmp/led_fifo

#include "GPIO.h"
#include <iostream>
//#include <map>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

// Maybe move all of the meta up into the NodeJS code
// class GPIOMeta : public GPIO{
// public:
//     GPIOMeta(const char* newChipName, int newLineNum, bool newWriteMode) 
//         : GPIO(newChipName, newLineNum)
//     {
//         writeMode = newWriteMode;
//     }
//     void set(bool val) override{
//         gpiod_line_set_value(lineLED, val);
//         state = 0;
//     }
//     bool get() override{
//         state = gpiod_line_get_value(lineLED);
//         return state;
//     }
//     int state = 0;
//     bool writeMode = false;
// private:
// };

class IPCWatcher{
public:
    // Getting add to work from NODE:
    // Add needs a constant
    // So I need to register all possible GPIO pins
    // And let the Node user decide which to use
    // Setting some for read and some for write
    void addDevice(GPIO * gpio, bool writeMode){
        if(writeMode){
            std::cout << "Adding write pin: " << pinsW.size() + 1 << std::endl;
            //std::cout << newChipName << " " << newLineNum << std::endl;  
            pinsW.push_back(gpio);
        }
        else{
            std::cout << "Adding read pin: " << pinsR.size() + 1 << std::endl;
            //std::cout << newChipName << " " << newLineNum << std::endl; 
            pinsR.push_back(gpio);
        }
    }
    int start(std::string newFifoPath="/tmp/pgpio"){
        std::cout << "Starting IPC Watcher @ " + newFifoPath << std::endl;

        std::string fifoPathIn = newFifoPath + "-in";
        std::string fifoPathOut = newFifoPath + "-out";

        std::cout << "Setting up IPC FIFO" << std::endl;
        std::cout << "In path: " << fifoPathIn << std::endl;
        std::cout << "Out path: " << fifoPathOut << std::endl;

        std::cout << "Waiting on input FIFO..." << std::endl;
        std::ifstream fifoIn(fifoPathIn);
        if (!fifoIn.is_open()) {
            std::cerr << "Failed to open input FIFO: "+fifoPathIn << std::endl;
            return 1;
        }
        else{
            std::cout << "Opened Input FIFO" << std::endl;
        }
        
        std::cout << "Waiting on output FIFO..." << std::endl;
        std::ofstream fifoOut(fifoPathOut, std::ofstream::out);
        if (!fifoOut.is_open()) { // check if the file is open
            std::cerr << "Failed to open output File: "+fifoPathOut << std::endl;
            return 1;
        }
        else{
            std::cout << "Opened Output FIFO" << std::endl;
        }

        initialize();
        
        std::string line;
        bool run = true;
        int i;
        while(run){
            // Read loop
            while (getline_async(fifoIn, line)) {
                std::cout << "Received message: " << line << std::endl;
                // std::vector<std::string> split = splitLine(line);
                // if(split.size() == 0){
                //     std::cout << "Error: Bad Input" << std::endl;
                //     continue;
                // }
                //std::string pinName = split[0];
                // Ensure we have a valid device
                // if(!deviceExists(pinName)){
                //     std::cout << "Error: Bad pinName" << std::endl;
                //     std::cout << pinName << std::endl;
                //     continue;
                // }
                // Val will be -1 if unset
                if(line.size()>pinsW.size()){
                    std::cerr << "Input too long" << std::endl;
                    continue;
                }
                if(line.size()<1){
                    std::cerr << "Input too short" << std::endl;
                    continue;
                }
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
                        pinsW[i]->set((bool)valInt);
                    }
                    else{
                        // run special setup
                        // if(pwm){
                        //     led.set((i & 1) != 0);
                        //     i++;
                        //     usleep(pwm);//microseconds
                        // }
                    }
                }
            }
            //
            // Read mode
            //
            std::string readOut;
            bool valChanged = false;
            for (int i = 0; i < pinsR.size(); i++) {
                int pinStateCache = pinsR[i].state;
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
        close(&fifoIn, &fifoOut);
        return 0;
    }
private:
    void initialize(){
        for (int i = 0; i < pinsW.size(); i++) {
            std::cout <<"Opening GPIO Device: " << i << std::endl;
            pinsW[i].open();
        }
        for (int i = 0; i < pinsR.size(); i++) {
            std::cout <<"Opening GPIO Device: " << i << std::endl;
            pinsR[i].open();
        }
        std::cout <<"Opened all GPIO Devices" << std::endl;
    }
    void close(std::ifstream * fifoIn,std::ofstream * fifoOut){
        for (int i = 0; i < pinsW.size(); i++) {
            std::cout <<"Releasing GPIO Device: " << i << std::endl;
            pinsW[i].release();
        }
        for (int i = 0; i < pinsR.size(); i++) {
            std::cout <<"Releasing GPIO Device: " << i << std::endl;
            pinsR[i].release();
        }
        fifoIn->close();
        fifoOut->close(); // close the file
    }
    // Deprecated
    // bool deviceExists(std::string pinName){
    //     if(pinsR.count(pinName)){
    //         return true;
    //     }
    //     if(pinsW.count(pinName)){
    //         return true;
    //     }
    //     return false;
    // }
    // Deprecated
    // bool isWriteMode(std::string pinName){
    //     if(pinsR.count(pinName)){
    //         return false;
    //     }
    //     if(pinsW.count(pinName)){
    //         return true;
    //     }
    //     std::cout << "Error: Pin not open";
    //     // Default to read
    //     return false;
    // }
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

    //std::map<std::string, GPIOMeta> pinsR = {};
    //std::map<std::string, GPIOMeta> pinsW = {};
    std::vector<GPIO*> pinsR = {};
    std::vector<GPIO*> pinsW = {};
};

int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  IPCWatcher watcher;

  GPIO c1l91("gpiochip1", 91);
  GPIO c1l93("gpiochip1", 93);
  GPIO c1l94("gpiochip1", 94);
  GPIO c1l98("gpiochip1", 98);
  
  // chip name, line number, writeMode
  watcher.addDevice(&c1l91, true);
  watcher.addDevice(&c1l98, true);
  watcher.addDevice(&c1l93, false);
  watcher.addDevice(&c1l94, false);
  watcher.start("/tmp/pgpio");
  return 0;
}
