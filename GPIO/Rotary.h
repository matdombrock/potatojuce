#include <chrono>
#include "GPIO.h"

class Rotary{
public:
    Rotary(
        const char* newChipNameSW, int newLineNumSW,
        const char* newChipNameDT, int newLineNumDT,
        const char* newChipNameCLK, int newLineNumCLK
    )
    : 
    gpioSW(newChipNameSW, newLineNumSW),
    gpioDT(newChipNameDT, newLineNumDT),
    gpioCLK(newChipNameCLK, newLineNumCLK)
    {
        gpioSW.open();
        gpioDT.open();
        gpioCLK.open();
        // Start loop
        readLoop();
    }
    ~Rotary(){
        gpioSW.release();
        gpioDT.release();
        gpioCLK.release();
    }
    std::string read(){
        std::string out = outQueue;
        outQueue = "";
        return out;
    }
private:
    void readLoop(){
        bool wasHit = false;
        int64_t ts;
        char lastDir = 'n';
        while(true){
            // Read CLK & DT
            bool clk = gpioCLK.get();
            bool dt = gpioDT.get();
            //std::cout << clk << dt << std::endl;
            if(clk && dt){
                //std::cout << "HIT" << std::endl;
                wasHit = true;
                continue;
            }
            if(wasHit && dt){
                int64_t speed = checkSpeed(ts);
                if(speed > 5 && lastDir == 'r'){
                    std::cout << "Debounce Left: " << speed << std::endl;
                }
                else{
                    std::cout << "Left: " << speed << std::endl; 
                    outQueue += "l";
                }
                ts = timestamp();
                lastDir = 'l';
                wasHit = false;
                continue;
            }
            if(wasHit && clk){
                int64_t speed = checkSpeed(ts);
                if(speed > 5 && lastDir == 'l'){
                    std::cout << "Debounce Right: " << speed << std::endl;
                }
                else{
                    std::cout << "Right: " << speed << std::endl;
                    outQueue += "r";
                }
                ts = timestamp();
                lastDir = 'r';
                wasHit = false;
                continue;
            }
            std::cout << outQueue << std::endl;
        }
    }

    // Returns a speed val 0->10
    int64_t checkSpeed(int64_t ts){
        auto delta = timestamp() - ts;
        delta = delta > 500 ? 500 : delta;
        // get a speed val 0->10
        auto speed = (500 - delta)/50;
        return speed;
    }
    int64_t timestamp(){
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto ts = now_ms.time_since_epoch().count();
        //std::cout << ts << std::endl;
        return ts;
    }
    std::string outQueue = "";
    int dir = 0;
    GPIO gpioSW;
    GPIO gpioDT;
    GPIO gpioCLK;
};