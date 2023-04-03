#include "LED_Driver.h"

int main(int argc, char **argv)
{
  const char* chipName = "gpiochip1";
  LED led(chipName, 98);
  led.open();
//   int i = 0;
//   while (true) {
//     led.set((i & 1) != 0);
//     usleep(100000);
//     i++;
//   }
    void X(){
        std::thread messageThread(led.pwm);

        // Inject messages into the loop
        sendMessage("Hello");
        sendMessage("World");

        messageThread.join();
    }
  led.release();
  return 0;
}