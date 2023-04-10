/*
    This is basically just a wrapper for gpiod
*/

#include <gpiod.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unistd.h>

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
    int getState(){
        return state;
    }
    void pwm(int usecs){
        if(pwmEnabled == false){
            log("Starting PWM Thread");
            pwmEnabled = true;
            m_pwmThreadRunning = true;
            m_pwmThread = std::thread(&GPIO::pwmThreadFunc, this);
        }
        if(usecs == 0){
            log("Stopping PWM Thread");
            pwmEnabled = false;
            m_pwmThreadRunning = false;
        }
        // Lock the mutex before accessing the message queue
        std::unique_lock<std::mutex> lock(m_messageQueueMutex);
        // Add the message to the queue
        std::string message = std::to_string(usecs);
        m_messageQueue.push(message);
        // Notify the PWM thread that there's a new message
        m_messageQueueCV.notify_one();
    }
    std::string getChipName(){
        return chipName;
    }
    int getLineNum(){
        return lineNum;
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
    bool pwmEnabled = false;
    bool m_pwmThreadRunning = false;
    std::thread m_pwmThread;
    std::queue<std::string> m_messageQueue;
    std::mutex m_messageQueueMutex;
    std::condition_variable m_messageQueueCV;
private:
    void pwmThreadFunc(){
        int pwmUSecs = 0;
        int iteration = 0;
        while(m_pwmThreadRunning){
            std::unique_lock<std::mutex> lock(m_messageQueueMutex);
            if (!m_messageQueue.empty()) {
                m_messageQueueCV.wait(lock, [this] { return !m_messageQueue.empty(); });
                // Get the next message from the queue
                std::string message = m_messageQueue.front();
                m_messageQueue.pop();
                // Process the message
                std::cout << "Received PWM message: " << message << std::endl;
                if(message == "0"){
                    // Sending 0 will stop the PWM
                    return;
                }
                pwmUSecs = std::stoi(message);
            }
            // Release the lock and continue the loop
            lock.unlock();
            //std::this_thread::yield();
            set((iteration & 1) != 0);
            usleep(pwmUSecs);
            iteration++;
            continue;
        }
    }
};
