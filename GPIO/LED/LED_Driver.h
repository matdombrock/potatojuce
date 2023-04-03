#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
//
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>

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
    void pwm(){
        int i = 0;
        while (true) {
            set((i & 1) != 0);
            usleep(100000);
            i++;
        }
    }
private:
    const char *chipName;
    struct gpiod_chip *chip;
    struct gpiod_line *lineLED;
    int lineNum = 0; 
};
