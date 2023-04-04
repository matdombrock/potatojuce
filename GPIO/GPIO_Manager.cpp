// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
// First do
// $ mkfio /tmp/led_fifo

#include "GPIO.h"
#include <iostream>
#include <map>
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
    void addDevice(std::string newpinName, const char* newChipName, int newLineNum, bool writeMode){
        if(deviceExists(newpinName)){
            std::cout << "Error: pinName Exists: "+newpinName;
            return;
        }
        if(writeMode){
            pinsW[newpinName] = GPIO(newChipName, newLineNum);
        }
        else{
            pinsR[newpinName] = GPIO(newChipName, newLineNum);
        }
    }
    int start(std::string newFifoPath="/tmp/pgpio-fifo"){
        std::cout << "Starting IPC Watcher @ " + newFifoPath << std::endl;
        std::string fifoPath = newFifoPath;
        std::ifstream fifo(fifoPath);
        if (!fifo.is_open()) {
            std::cerr << "Failed to open FIFO: " << fifoPath << std::endl;
            return 1;
        }

        initialize();
        
        std::string line;
        bool run = true;
        int i;
        while(run){
            while (getline_async(fifo, line)) {
                std::cout << "Received message: " << line << std::endl;
                std::vector<std::string> split = splitLine(line);
                if(split.size() == 0){
                    std::cout << "Error: Bad Input" << std::endl;
                    continue;
                }
                std::string pinName = split[0];
                // Ensure we have a valid device
                if(!deviceExists(pinName)){
                    std::cout << "Error: Bad pinName" << std::endl;
                    std::cout << pinName << std::endl;
                    continue;
                }
                if(isWriteMode(pinName)){
                    //
                    // Write mode
                    //
                    // Val will be -1 if unset
                    std::string valStr = split.size() > 1 ? split[1] : "-1";

                    int valInt = std::stoi(valStr);
                    // Check if we have a bool
                    if(valInt == 0 || valInt == 1){
                        pinsW[pinName].set((bool)valInt);
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
                else{
                    //
                    // Read mode
                    //
                }
            }
        }
        close(&fifo);
        return 0;
    }
private:
    void initialize(){
        for (auto it = pinsW.begin(); it != pinsW.end(); ++it) {
            std::string pinName = it->first;
            std::cout <<"Opening GPIO Device: " << pinName << std::endl;
            pinsW[pinName].open();
        }
        for (auto it = pinsR.begin(); it != pinsR.end(); ++it) {
            std::string pinName = it->first;
            std::cout <<"Opening GPIO Device: " << pinName << std::endl;
            pinsR[pinName].open();
        }
        std::cout <<"Opened all GPIO Devices" << std::endl;
    }
    void close(std::ifstream * fifo){
        for (auto it = pinsW.begin(); it != pinsW.end(); ++it) {
            std::string pinName = it->first;
            std::cout <<"Releasing GPIO Device: " << pinName << std::endl;
            pinsW[pinName].release();
        }
        for (auto it = pinsR.begin(); it != pinsR.end(); ++it) {
            std::string pinName = it->first;
            std::cout <<"Releasing GPIO Device: " << pinName << std::endl;
            pinsR[pinName].release();
        }
        fifo->close();
    }
    bool deviceExists(std::string pinName){
        if(pinsR.count(pinName)){
            return true;
        }
        if(pinsW.count(pinName)){
            return true;
        }
        return false;
    }
    bool isWriteMode(std::string pinName){
        if(pinsR.count(pinName)){
            return false;
        }
        if(pinsW.count(pinName)){
            return true;
        }
        std::cout << "Error: Pin not open";
        // Default to read
        return false;
    }
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

    std::map<std::string, GPIO> pinsR = {};
    std::map<std::string, GPIO> pinsW = {};
};

int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  IPCWatcher watcher;
  watcher.addDevice("led1","gpiochip1", 91, 0);// chip name, line number
  watcher.addDevice("led2","gpiochip1", 98, 0);// chip name, line number
  watcher.start("/tmp/pgpio-fifo");
  return 0;
}
