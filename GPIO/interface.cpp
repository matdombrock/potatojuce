#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "LED.h"
#include "RGBLED.h"
#include "Rotary.h"
#include "Button.h"

class State{
public:
    State(){

    }
    void iterate(){
        it = it <= 200 ? it+1 : 0;
    }
    bool utilMode = false;
    bool it = 0;
    int r1 = 0;
    int r2 = 0;
    int r3 = 0;
};

class Interface{
public:
    Interface()
    :
    rgb(
        "gpiochip1", 91,
        "gpiochip1", 98,
        "gpiochip1", 92
    ),
    btn("gpiochip1", 79),
    rot1(
        "gpiochip1", 91,//bad
        "gpiochip1", 84,
        "gpiochip1", 86
    ),
    rot2(
        "gpiochip1", 91,// bad
        "gpiochip1", 82,
        "gpiochip1", 83
    ),
    rot3(
        "gpiochip1", 91,// bad
        "gpiochip1", 93,
        "gpiochip1", 94
    )
    {
        btn.setToggleMode();
        startAni();
        initFifo();
    }
    int initFifo(){
        std::string fifoPathOut = "/tmp/pinp";
        fifoOut.open(fifoPathOut);
        std::cout << "Waiting on fifo..." << std::endl;
        if (!fifoOut.is_open()) {
            std::cerr << "Failed to open output FIFO: "+fifoPathOut << std::endl;
            return 1;
        }
        else{
            std::cout << "Opened output FIFO" << std::endl;
            return 0;
        }
    }
    void setLights(){
        float r = state.r1 / 100.0f;
        float g = state.r2 / 100.0f;
        float b = state.r3 / 100.0f;
        rgb.set(r,g,b);
    }
    void startAni(){
        //
        int sleepT = 500000;
        rgb.set(0,0,0);
        usleep(sleepT);
        rgb.set(1,0,0);
        usleep(sleepT);
        rgb.set(0,1,0);
        usleep(sleepT);
        rgb.set(0,0,1);
        usleep(sleepT);
        
        rgb.on();
        usleep(sleepT/2);
        rgb.off();
        usleep(sleepT/2);
        rgb.on();
        usleep(sleepT/2);
        rgb.off();
        usleep(sleepT/2);
        rgb.on();
        usleep(sleepT/2);
        rgb.off();
        usleep(sleepT/2);

        rgb.set(0,0,0);
    }
    void begin(){
        while(true){
            usleep(1000);// CPU Optimization
            std::string rotRead1 = rot1.read();
            std::string rotRead2 = rot2.read();
            std::string rotRead3 = rot3.read();
            
            if(btn.changed()){
                Util::echo("Mode changed!");
                bool val = btn.getToggled();
                state.utilMode = val;
                //std::string valS = val ? "on" : "off";
                //Util::echo(valS);
                if(state.utilMode){
                    rgb.set(1,1,1);
                    continue;
                }
                else{
                    // Reset
                    //rgb.set(0,0,0);
                    setLights();
                    continue;
                }
            }
            

            // Handle blink
            state.iterate();

            // Dont continue from here if the message is empty
            if(rotRead1.size() == 0 
                && rotRead2.size() == 0
                && rotRead3.size() == 0
            )
            {
                continue;
            }

            for(int i = 0; i < rotRead1.size(); i++){
                char dir = rotRead1[i];
                if(dir == 'r'){
                    state.r1 += 1;
                }
                else{
                    state.r1 += -1;
                }
            }
            // Ensure r1 is 0->100
            state.r1 = Util::bound(state.r1, 0, 100);

            for(int i = 0; i < rotRead2.size(); i++){
                char dir = rotRead2[i];
                if(dir == 'r'){
                    state.r2 += 1;
                }
                else{
                    state.r2 += -1;
                }
            }
            // Ensure r2 is 0->100
            state.r2 = Util::bound(state.r2, 0, 100);

            for(int i = 0; i < rotRead3.size(); i++){
                char dir = rotRead3[i];
                if(dir == 'r'){
                    state.r3 += 1;
                }
                else{
                    state.r3 += -1;
                }
            }
            // Ensure r3 is 0->100
            state.r3 = Util::bound(state.r3, 0, 100);

            setLights();

            fifoOut << "f " << (state.r1 * 10) << std::endl;
            fifoOut << "p0 " << (state.r2 / 10.f) << std::endl;
            fifoOut << "p1 " << (state.r3 / 10.f) << std::endl;
        }
    }
    
private:
    RGBLED rgb;
    Button btn;
    Rotary rot1;
    Rotary rot2;
    Rotary rot3;
    std::ofstream fifoOut;
    State state;
};

int main(int argc, char **argv)
{
    Interface interface;
    interface.begin();
    return 0;
}