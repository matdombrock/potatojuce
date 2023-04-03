// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
#include "LED_Driver.h"
#include <iostream>

// Config
const char* chipName = "gpiochip1";
const int lineNum = 98;

int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  std::cout << "Chip Name: " << chipName << std::endl;
  std::cout << "Line Number: " << lineNum << std::endl;
  std::cout << "---------------------------" << std::endl;
  LED led(chipName, lineNum);
  // First do
  // $ mkfio /tmp/led_fifo
  led.startIPCWatcher("/tmp/led_fifo");
  return 0;
}
