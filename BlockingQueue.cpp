#include "BlockingQueue.h"
#include <queue>
#include <string>

BlockingQueue::BlockingQueue(std::queue<std::string>& urls)
    :closed(false), urls(urls){
}

std::string BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    while (urls.empty()){
        if (closed){
            return std::string();
        }
        cv.wait(lock1);
    }
    std::string url = std::move(urls.front());
    urls.pop(); // bye bye
    return url;
}

void BlockingQueue::push(std::string&& url) {
    std::unique_lock<std::mutex> lock1(queueMutex);
    urls.push(url);
    cv.notify_all();
}

bool BlockingQueue::isClosed() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    return closed;
}

void BlockingQueue::signalClosed() {
    std::unique_lock<std::mutex> lock1(queueMutex);
    cv.notify_all();
    closed = true;
}
