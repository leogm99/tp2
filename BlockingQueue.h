#ifndef TP2_BLOCKINGQUEUE_H
#define TP2_BLOCKINGQUEUE_H
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>

class BlockingQueue {
private:
    bool closed;
    std::queue<std::string>& urls;
    std::mutex queueMutex;
    std::condition_variable cv;
public:
    explicit BlockingQueue(std::queue<std::string>& urls);

    BlockingQueue() = delete;
    BlockingQueue(const BlockingQueue& other) = delete;
    BlockingQueue& operator=(const BlockingQueue& other) = delete;

    bool isClosed();
    void signalClosed();

    std::string pop();
    void push(std::string&& url);
};


#endif //TP2_BLOCKINGQUEUE_H
