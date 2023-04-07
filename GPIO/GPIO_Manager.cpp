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

class GPIOMeta : public GPIO{
public:
    GPIOMeta(const char* newChipName="gpiochip1", int newLineNum=98) 
        : GPIO(newChipName, newLineNum)
    {

    }
    void set(bool val) override{
        gpiod_line_set_value(lineLED, val);
        state = 0;
    }
    int state = 0;
private:
}

class IPCWatcher{
public:
    void addDevice(std::string newPinName, const char* newChipName, int newLineNum, bool writeMode){
        if(deviceExists(newPinName)){
            std::cout << "Error: pinName Exists: "+newPinName;
            return;
        }
        if(writeMode){
            std::cout << "Adding write pin: " << newPinName << std::endl;
            std::cout << newChipName << " " << newLineNum << std::endl;  
            pinsW[newPinName] = GPIO(newChipName, newLineNum);
        }
        else{
            pinsR[newPinName] = GPIO(newChipName, newLineNum);
            std::cout << "Adding write pin: " << newPinName << std::endl;
            std::cout << newChipName << " " << newLineNum << std::endl; 
        }
    }
    int start(std::string newFifoPath="/tmp/pgpio-fifo"){
        std::cout << "Starting IPC Watcher @ " + newFifoPath << std::endl;
        std::string fifoPath = newFifoPath;
        std::ifstream fifo(fifoPath);
        if (!fifo.is_open()) {
            std::cerr << "Failed to open input FIFO: " << fifoPath << std::endl;
            return 1;
        }
        //
        // THIS SHOULD BE A PIPE!!!
        // Write bits
        //
        std::ofstream outFile; // create an output file stream
        outFile.open("/tmp/pgpio-out", std::ofstream::trunc);
        if (!outFile.is_open()) { // check if the file is open
            std::cerr << "Failed to open output File: " << std::endl;
            return 1;
            outFile.close();// Close for now
        }

        initialize();
        
        std::string line;
        bool run = true;
        int i;
        while(run){
            // Read loop
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
                    std::cout << "Error: Pin is not in write mode!" << std::endl;
                }
            }
            //
            // Read mode
            //
            for (auto it = pinsR.begin(); it != pinsR.end(); ++it) {
                std::string pinName = it->first;
                int pinVal = pinsR[pinName].get();// Not a bool!
                std::string outString = std::to_string(pinVal);
                outFile.open("/tmp/pgpio-out", std::ofstream::trunc);
                outFile << outString; // write the string to the file
                outFile.close();
            }
        }
        close(&fifo, &outFile);
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
    void close(std::ifstream * fifo, std::ofstream * outFile){
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
        outFile->close(); // close the file
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

    std::map<std::string, GPIOMeta> pinsR = {};
    std::map<std::string, GPIOMeta> pinsW = {};
};

int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  IPCWatcher watcher;
  watcher.addDevice("led1","gpiochip1", 91, 1);// chip name, line number
  watcher.addDevice("led2","gpiochip1", 98, 1);// chip name, line number
  watcher.addDevice("readTest","gpiochip1", 92, 0);// chip name, line number
  watcher.start("/tmp/pgpio-fifo");
  return 0;
}
