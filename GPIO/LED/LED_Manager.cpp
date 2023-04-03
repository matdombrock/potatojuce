// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
// First do
// $ mkfio /tmp/led_fifo

#include "LED_Driver.h"
#include <iostream>
#include <map>

LED led[2] = {LED("gpiochip1", 91), LED("gpiochip1",98)};

int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  for(int ledIndex = 0; ledIndex < 2; ledIndex++){
    led[ledIndex].startIPCWatcher("/tmp/led"+std::to_string(ledIndex)+"_fifo");
  }
  return 0;
}
