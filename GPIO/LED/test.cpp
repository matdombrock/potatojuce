#include "LED_Driver.h"
#include <iostream>

int main(int argc, char **argv)
{
  std::cout << "Test...\n";
  const char* chipName = "gpiochip1";
  LED led(chipName, 98);
  led.startIPCWatcher();
  return 0;
}