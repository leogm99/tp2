#ifndef TP2_BLOCKINGQUEUE_H
#define TP2_BLOCKINGQUEUE_H
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <utility>

class BlockingQueue {
private:
    bool closed;
    std::queue<std::pair<std::string,
                         std::string>>& urls;
    std::mutex queueMutex;
    std::condition_variable cv;
public:
    explicit BlockingQueue(std::queue<std::pair<std::string,
                           std::string>>& urls);

    BlockingQueue() = delete;
    BlockingQueue(const BlockingQueue& other) = delete;
    BlockingQueue& operator=(const BlockingQueue& other) = delete;

    bool isNotClosedOrNotEmpty();
    void signalClosed();

    std::string pop();
    void push(std::string&& url);
};


#endif
