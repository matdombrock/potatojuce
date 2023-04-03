#include "LED_Driver.h"
#include <iostream>

int main(int argc, char **argv)
{
  std::cout << "Test...\n";
  const char* chipName = "gpiochip1";
  LED led(chipName, 98);
  led.open();
  int i = 0;
  std::cout << "Blinking...\n";
  while (true) {
    led.set((i & 1) != 0);
    usleep(100000);
    i++;
  }
  led.release();
  return 0;
}