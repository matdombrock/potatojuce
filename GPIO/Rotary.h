#include <chrono>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unistd.h>
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
        std::cout << "Starting Read Thread" << std::endl;
        m_readThread = std::thread(&Rotary::readThreadFunc, this);
        // Join the threads so main doesnt die
        //m_readThread.join();
    }
    ~Rotary(){
        gpioSW.release();
        gpioDT.release();
        gpioCLK.release();
        m_readThread.join();
    }
    std::string read(){
        std::string out = outQueue;
        // Check if we actually have a queue
        // and should clear
        if(out.size() > 0){
            // Lock the mutex before accessing the message queue
            std::unique_lock<std::mutex> lock(m_messageQueueMutex);
            // Add the message to the queue
            std::string message = "clear";
            m_messageQueue.push(message);
            // Notify the PWM thread that there's a new message
            m_messageQueueCV.notify_one();
        }
        return out;
    }
    bool m_readThreadRunning = true;
    std::thread m_readThread;
    std::queue<std::string> m_messageQueue;
    std::mutex m_messageQueueMutex;
    std::condition_variable m_messageQueueCV;
private:
    void readThreadFunc(){
        bool wasHit = false;
        int64_t ts;
        char lastDir = 'n';
        std::cout << "Enter Read Thread Loop" << std::endl;
        while(m_readThreadRunning){
            usleep(1000);// Optimization
            std::unique_lock<std::mutex> lock(m_messageQueueMutex);
            if (!m_messageQueue.empty()) {
                m_messageQueueCV.wait(lock, [this] { return !m_messageQueue.empty(); });
                // Get the next message from the queue
                std::string message = m_messageQueue.front();
                m_messageQueue.pop();
                // Process the message
                //std::cout << "Received Rotary message: " << message << std::endl;
                if(message == "clear"){
                    // Clear out queue
                    outQueue = "";
                }
            }
            // Release the lock and continue the loop
            lock.unlock();
            // Read CLK & DT
            bool clk = gpioCLK.get();
            bool dt = gpioDT.get();
            //std::cout << "READ: " << clk << dt << std::endl;
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
                    // One char for each speed lvl
                    //for(int s; s < speed; s++){
                        outQueue += "l";
                    //}
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
                    // One char for each speed lvl
                    //for(int s; s < speed; s++){
                        outQueue += "r";
                    //}
                }
                ts = timestamp();
                lastDir = 'r';
                wasHit = false;
                continue;
            }
            
            //std::cout << outQueue << std::endl;
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