// The LED_Driver IPC Manager
// IPC via FIFO pipe at /tmp/led_fifo
// First do
// $ mkfio /tmp/led_fifo

#include "GPIO.h"
#include <iostream>
#include <map>

GPIO led("gpiochip1", 91);// chip name, line number

int main(int argc, char **argv)
{
  std::cout << "Starting LED Driver Manager" << std::endl;
  std::cout << "===========================" << std::endl;
  led.startIPCWatcher("/tmp/led_fifo");
  return 0;
}
