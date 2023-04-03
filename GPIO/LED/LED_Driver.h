#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

// Source: https://stackoverflow.com/a/57809972
bool getline_async(std::istream& is, std::string& str, char delim = '\n') {    
    static std::string lineSoFar;
    char inChar;
    int charsRead = 0;
    bool lineRead = false;
    str = "";

    do {
        charsRead = is.readsome(&inChar, 1);
        if (charsRead == 1) {
            // if the delimiter is read then return the string so far
            if (inChar == delim) {
                str = lineSoFar;
                lineSoFar = "";
                lineRead = true;
            } else {  // otherwise add it to the string so far
                lineSoFar.append(1, inChar);
            }
        }
    } while (charsRead != 0 && !lineRead);

    return lineRead;
}

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
    int startIPCWatcher(){
        open();
        // $ mkfio /tmp/led_fifo
        std::string fifo_path = "/tmp/led_fifo"; // replace with your FIFO path
        std::ifstream fifo(fifo_path);
        if (!fifo.is_open()) {
            std::cerr << "Failed to open FIFO: " << fifo_path << std::endl;
            return 1;
        }
        std::string line;
        bool run = true;
        int pwm = 0;
        int i;
        while(run){
            while (getline_async(fifo, line)) {
                std::cout << "Received message: " << line << std::endl;
                int num;
                num = std::stoi(line);
                if(num < 0){
                    run = false;
                    std::cout << "Break " << line << std::endl;
                    continue;
                }
                else if(num > 1){
                    pwm = num;
                    std::cout << "PWM " << line << " microseconds" << std::endl;
                    continue;
                }
                else{
                    pwm = 0;
                    std::cout << "Set " << line << std::endl;
                    set((bool)num);
                }
            }
            if(pwm){
                set((i & 1) != 0);
                i++;
                usleep(pwm);//microseconds
            }
        }
        release();
        fifo.close();
        return 0;
    }
private:
    const char *chipName;
    struct gpiod_chip *chip;
    struct gpiod_line *lineLED;
    int lineNum = 0; 
};
