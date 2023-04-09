/*
    This is basically just a wrapper for gpiod
*/

#include <gpiod.h>
#include <stdio.h>
#include <iostream>


class GPIO{
public:
    GPIO(const char* newChipName="gpiochip1", int newLineNum=98){
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
    virtual void set(bool val){
        gpiod_line_set_value(lineLED, val);
        state = val;
    }
    virtual bool get(){
        bool val = gpiod_line_get_value(lineLED);
        state = val;
        return val;
    }
    virtual int getState(){
        return state;
    }
    void log(std::string msg){
        std::cout << "---------------------------" << std::endl;
        std::cout << msg << std::endl;
        std::cout << "Chip Name: " << chipName << std::endl;
        std::cout << "Line Number: " << lineNum << std::endl;
        std::cout << "---------------------------" << std::endl;
    } 
    const char *chipName;
    int lineNum = 0; 
    int state = 0;// Cache the pin state
    struct gpiod_chip *chip;
    struct gpiod_line *lineLED;
private:
    
};
