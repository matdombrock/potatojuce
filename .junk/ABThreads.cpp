#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class Alice {
public:
    Alice() {
        // Start the Bob thread
        m_bobThread = std::thread(&Alice::bobThreadFunc, this);
    }

    ~Alice() {
        // Stop the Bob thread
        m_bobThreadRunning = false;
        m_bobThread.join();
    }

    void sendMessage(const std::string& message) {
        // Lock the mutex before accessing the message queue
        std::unique_lock<std::mutex> lock(m_messageQueueMutex);

        // Add the message to the queue
        m_messageQueue.push(message);

        // Notify the Bob thread that there's a new message
        m_messageQueueCV.notify_one();
    }

private:
    void bobThreadFunc() {
        while (m_bobThreadRunning) {
            // Wait for a message to arrive
            std::unique_lock<std::mutex> lock(m_messageQueueMutex);
            if (!m_messageQueue.empty()) {
                m_messageQueueCV.wait(lock, [this] { return !m_messageQueue.empty(); });
                // Get the next message from the queue
                std::string message = m_messageQueue.front();
                m_messageQueue.pop();
                // Process the message
                std::cout << "Bob received message: " << message << std::endl;
                if(message == "STOP"){
                    return;
                }
            }
            else{
                // Release the lock and continue the loop
                std::cout << "LOOP" << std::endl;
                lock.unlock();
                std::this_thread::yield();
                continue;
            }
        }
    }

    std::thread m_bobThread;
    bool m_bobThreadRunning = true;

    std::queue<std::string> m_messageQueue;
    std::mutex m_messageQueueMutex;
    std::condition_variable m_messageQueueCV;
};

int main() {
    Alice alice;

    // Send some messages to Bob
    alice.sendMessage("Hello, Bob!");
    alice.sendMessage("How are you?");
    alice.sendMessage("I'm fine, thanks!");
    
    

    // Wait for a bit to let Bob process the messages
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Slept" << std::endl;

    alice.sendMessage("STOP");

    return 0;
}
