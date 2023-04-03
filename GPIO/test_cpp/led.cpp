#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

class LED{
public:
    LED(const char* newChipName, int newLineNum){
        chipName = newChipName;
        lineNum = newLineNum;
    }
    void open(){
        // Open GPIO chip
        chip = gpiod_chip_open_by_name(chipName);

        // Open GPIO lines
        lineLED = gpiod_chip_get_line(chip, lineNum);

        // Open LED lines for output
        gpiod_line_request_output(lineLED, "example1", 0);
    }
    void release(){
        // Release lines and chip
        gpiod_line_release(lineLED);
        gpiod_chip_close(chip);
    }
    void set(bool val){
        gpiod_line_set_value(lineLED, val);
    }
private:
    const char *chipName;
    struct gpiod_chip *chip;
    struct gpiod_line *lineLED;
    int lineNum = 0;
};


int main(int argc, char **argv)
{
  const char* chipName = "gpiochip1";
  LED led(chipName, 98);
  int i = 0;
  while (true) {
    led.set((i & 1) != 0);
    usleep(100000);
    i++;
  }
  return 0;
}