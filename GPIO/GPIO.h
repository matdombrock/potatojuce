/*
    This is basically just a wrapper for gpiod
*/

#include <gpiod.h>
#include <stdio.h>
#include <iostream>


class GPIO{
public:
    GPIO(const char* newChipName, int newLineNum){
        chipName = newChipName;
        lineNum = newLineNum;
    }
    void open(){
        log("Opening GPIO");
        // Open GPIO chip
        chip = gpiod_chip_open_by_name(chipName);
        // Open GPIO lines
        lineLED = gpiod_chip_get_line(chip, lineNum);
        // Open LED lines for output
        gpiod_line_request_output(lineLED, "example1", 0);
    }
    void release(){
        log("Releasing GPIO");
        // Release lines and chip
        gpiod_line_release(lineLED);
        gpiod_chip_close(chip);
    }
    void set(bool val){
        gpiod_line_set_value(lineLED, val);
    }
    void log(std::string msg){
        std::cout << "---------------------------" << std::endl;
        std::cout << msg << std::endl;
        std::cout << "Chip Name: " << chipName << std::endl;
        std::cout << "Line Number: " << lineNum << std::endl;
        std::cout << "---------------------------" << std::endl;
    } 
private:
    const char *chipName;
    int lineNum = 0; 
    struct gpiod_chip *chip;
    struct gpiod_line *lineLED;
};
