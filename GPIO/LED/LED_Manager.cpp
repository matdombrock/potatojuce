#include <iostream>
#include <fstream>
#include <string>
#include "LED_Driver.h"

int main() {
    const char* chipName = "gpiochip1";
    LED led(chipName, 98);
    led.open();

    std::string fifo_path = "/tmp/led_fifo"; // replace with your FIFO path
    std::ifstream fifo(fifo_path);

    if (!fifo.is_open()) {
        std::cerr << "Failed to open FIFO: " << fifo_path << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(fifo, line)) {
        int num;
        num = std::stoi(line);
        std::cout << "Received message: " << line << std::endl;
        led.set((bool)num);
    }
    led.close();
    fifo.close();
    return 0;
}
