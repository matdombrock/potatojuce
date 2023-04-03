// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
#include <iostream>
#include <fstream>
#include <string>
#include "LED_Driver.h"

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
    while (getline_async(fifo, line)) {
        int num;
        num = std::stoi(line);
        std::cout << "Received message: " << line << std::endl;
        led.set((bool)num);
    }
    led.release();
    fifo.close();
    return 0;
}
