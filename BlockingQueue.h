#ifndef TP2_BLOCKINGQUEUE_H
#define TP2_BLOCKINGQUEUE_H
#include "TargetLoader.h"
#include "Url.h"
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
    std::queue<Url> urls;
    std::mutex queueMutex;
    std::condition_variable cv;
public:
    explicit BlockingQueue(TargetLoader& targetLoader);

    BlockingQueue() = delete;
    BlockingQueue(const BlockingQueue& other) = delete;
    BlockingQueue& operator=(const BlockingQueue& other) = delete;

    bool isNotClosedOrNotEmpty();
    void signalClosed();

    Url pop();
    void push(Url url);
};


#endif
