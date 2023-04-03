// C++ 11
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>

std::queue<std::string> messageQueue;
std::mutex queueMutex;

void sendMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(queueMutex);
    messageQueue.push(message);
}

void processMessages() {
    while (true) {
        std::lock_guard<std::mutex> lock(queueMutex);
        while (!messageQueue.empty()) {
            std::string message = messageQueue.front();
            messageQueue.pop();
            // Process the message here
            std::cout << "Received message: " << message << std::endl;
        }
    }
}

int main() {
    std::thread messageThread(processMessages);

    // Inject messages into the loop
    sendMessage("Hello");
    sendMessage("World");

    messageThread.join();
    return 0;
}
