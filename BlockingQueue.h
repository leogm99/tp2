#ifndef TP2_BLOCKINGQUEUE_H
#define TP2_BLOCKINGQUEUE_H
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

class BlockingQueue {
private:
    bool closed;
    queue<string>& urls;
    mutex queueMutex;
    condition_variable cv;
public:
    explicit BlockingQueue(queue<string>& urls);

    BlockingQueue() = delete;
    BlockingQueue(const BlockingQueue& other) = delete;
    BlockingQueue& operator=(const BlockingQueue& other) = delete;

    //BlockingQueue(BlockingQueue&& other);
    //BlockingQueue& operator= (BlockingQueue&& other) noexcept;

    bool isClosed();
    void signalClosed();

    string pop();
    void push(string&& url);
};


#endif //TP2_BLOCKINGQUEUE_H
