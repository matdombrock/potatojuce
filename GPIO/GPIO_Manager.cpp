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
    void addDevice(std::string newDeviceId, const char* newChipName, int newLineNum){
        if(devices.count(newDeviceId)){
            std::cout << "Error: Device ID Exists: "+newDeviceId;
            return;
        }
        devices[newDeviceId] = GPIO(newChipName, newLineNum);
    }
    void initialize(){
        for (auto it = devices.begin(); it != devices.end(); ++it) {
            std::string deviceId = it->first;
            std::cout <<"Opening GPIO Device: " << deviceId << std::endl;
            devices[deviceId].open();
        }
        std::cout <<"Opened all GPIO Devices" << std::endl;
    }
    void close(std::ifstream * fifo){
        for (auto it = devices.begin(); it != devices.end(); ++it) {
            std::string deviceId = it->first;
            std::cout <<"Releasing GPIO Device: " << deviceId << std::endl;
            devices[deviceId].release();
        }
        fifo->close();
    }
    int start(std::string newFifoPath="/tmp/pgpio-fifo"){
        std::cout << "Starting IPC Watcher @ " + newFifoPath << std::endl;
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
                std::vector<std::string> split = splitLine(line);
                if(split.size() == 0){
                    std::cout << "Error: Bad Input" << std::endl;
                    continue;
                }
                std::string deviceId = split[0];
                // Ensure we have a valid device
                if(!devices.count(deviceId)){
                    std::cout << "Error: Bad Device Id" << std::endl;
                    std::cout << deviceId << std::endl;
                    continue;
                }
                // val will be -1 if unset
                std::string valStr = split.size() > 1 ? split[2] : "-1";
                int valInt = std::stoi(valStr);
                // Check if we have a bool
                if(valInt == 0 || valInt == 1){
                    devices[deviceId].set((bool)valInt);
                }
                else{
                    // run special setup
                }
                // Set
                // <deviceId> <val>
                // Get
                // <deviceId>

                // int num;
                // num = std::stoi(line);
                // if(num < 0){
                //     run = false;
                //     std::cout << "Break " << line << std::endl;
                //     continue;
                // }
                // else if(num > 1){
                //     pwm = num;
                //     std::cout << "PWM " << line << " microseconds" << std::endl;
                //     continue;
                // }
                // else{
                //     pwm = 0;
                //     std::cout << "Set " << line << std::endl;
                //     led.set((bool)num);
                // }
            }
            // if(pwm){
            //     led.set((i & 1) != 0);
            //     i++;
            //     usleep(pwm);//microseconds
            // }
        }
        close(&fifo);
        return 0;
    }
private:
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

    std::map<std::string, GPIO> devices;
};

int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  IPCWatcher watcher;
  watcher.addDevice("led1","gpiochip1", 91);// chip name, line number
  watcher.addDevice("led2","gpiochip1", 98);// chip name, line number
  watcher.start("/tmp/pgpio-fifo");
  return 0;
}
