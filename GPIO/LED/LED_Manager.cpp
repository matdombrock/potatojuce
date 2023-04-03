#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include "LED_Driver.h"

std::queue<bool> messageQueue;
std::mutex queueMutex;

void sendMessage(const bool& message) {
    std::lock_guard<std::mutex> lock(queueMutex);
    messageQueue.push(message);
}

void processMessages() {
    LED led("gpiochip1");
    while (true) {
        std::lock_guard<std::mutex> lock(queueMutex);
        while (!messageQueue.empty()) {
            bool message = messageQueue.front();
            messageQueue.pop();
            // Process the message here
            std::cout << "Received message: " << message << std::endl;
            led.set(message);
        }
    }
}

int main() {
    std::thread messageThread(processMessages);

    // Inject messages into the loop
    sendMessage(0);
    usleep(100000);
    sendMessage(1);
    usleep(100000);
    sendMessage(0);

    messageThread.join();
    return 0;
}
